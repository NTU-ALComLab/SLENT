from __future__ import print_function
import sys
import time
import re
import csv
import numpy as np
import subprocess, threading, os, signal
from subprocess import call
#from subprocess32 import call
#from subprocess32 import TimeoutExpired
from os import listdir
from os.path import isdir, isfile, join

############################## Experimental Parameters ######################################
TIMEOUT             = 20    # Time Out (unit: seconds)
DPI                 = 400   # Dots Per Inch for figures
tmp_dir             = 'tmp' # Directory for temporary files
dbg_dir             = 'dbg' # Directory for debugging information
exp_dir             = 'exp' # Directory for experimental results
NDEBUG              = 1     # No debugging flag; debug-mode only for single file

############################## Benchmark Directory ##########################################
# Each subdirectory in benchmark_dir contains only one Length Automaton file
# Default LAUT file name is "laut"
# The capitals in benchmark name are reserved for option
# Please use non-repeating benchmark names and options
benchmark_dir       = {'Kaluza'  : ['benchmark/laut/kaluza'] ,
                       'Testing' : ['benchmark/laut/testing/sat','benchmark/laut/testing/unsat'],
                       #'Pisa'    : ['benchmark/laut/pisa'   ] ,
                       #'Appscan' : ['benchmark/laut/appscan'] ,
                       #'Slog'    : ['benchmark/laut/slog'] ,
                       #'R_pisa'  : ['benchmark/laut/r_pisa'],
                       #'V-lan-rep' : ['benchmark/laut/v-lan-rep'] ,
                       #'W-lan-rep' : ['benchmark/laut/w-lan-rep'] ,
                       #'X-str-rep' : ['benchmark/laut/x-str-rep'] ,
                       'C_str_rep_nto':['benchmark/laut/c_str_rep_nt_original'],
                       'D_str_rep_nt':['benchmark/laut/d_str_rep_nt'],
                       #'E-str-rep'   :['benchmark/laut/e-str-rep-new'],
                       'E_str_rep'   :['benchmark/laut/e_str_rep'],
                       'F_lan_rep_nt':['benchmark/laut/f_lan_rep_nt'],
                       'G_lan_rep'   :['benchmark/laut/g_lan_rep_t']}
benchmark_option    = {} # Don't need to be initialized

############################## Binary Directory #############################################
slender_dir         = 'bin/slender'
regex2dot_dir       = 'bin/regex2blif/target/regex2blif-0.0.1-SNAPSHOT.jar'
abc70930_dir        = 'bin/abc70930/abc'

solver_dir          = { #'cvc4' : './bin/cvc4-2017-06-27/builds/bin/cvc4',
                        #'norn' : './bin/norn/norn'                      ,
                        #'z3'   : './bin/z3/build/z3'                    , 
                        #'s3p'  : './run.py'                             ,
                        's3p'  : './bin/bin-rel-S3/run.py'              ,
                        'ic3ia': './bin/ic3ia/build/ic3ia'              ,
                        #'abc'  : './bin/abc/bin/abc'                    ,
                        #'sloth': './bin/sloth-1.0/sloth'                ,
                        #'ABC'  : 'abc'                                  ,
                        'trau'  : './bin/TRAU/Trau'                     }

solver_opt          = { 'cvc4' : '--strings-exp'                        ,
                        'norn' : ''                                     ,
                        'z3'   : ''                                     ,
                        's3p'  : '-f'                                   ,
                        'ic3ia': '-w -v 2'                              ,
                        'abc'  : '-f'                                   ,
                        'sloth': ''                                     ,
                        'trau' : ''                                     }
                        #'ABC'  : '-i'                                   ,
                        #'fat'  : '-model'                               }
#solver_list         = ['ic3ia','abc','z3','norn','s3p','fat','ABC','cvc4']
solver_list         = ['ic3ia','s3p','trau']

solver_ext          = { 'cvc4' :'smt2',
                        'norn' :'smt2',
                        'z3'   :'smt2',
                        's3p'  :'s3'  ,
                        'ic3ia':'vmt' ,
                        'abc'  :'abc' ,
                        'sloth':'smt2',
                        'trau' :'trau'}
                        #'ABC'  :'smt2',
                        #'fat'  :'s3'  }

##############################################################################
# [ Class Name  ] Command
# [ Description ] construct command with timeout specified
# [  Arguments  ] 
##############################################################################
class Command(object):
    def __init__(self, cmd, to, path):
        self.cmd     = cmd
        self.timeout = to
        self.stdout  = (lambda : open(path,'w') if path else subprocess.PIPE)()
        self.process = None
    
    # return 0 if normally exit
    def run(self):
        def target():
            self.process = subprocess.Popen(self.cmd, stdout=self.stdout, shell=True, preexec_fn=os.setsid)
            self.process.communicate()

        thread = threading.Thread(target=target)
        thread.start()
        thread.join(self.timeout)
        if thread.is_alive():
            os.killpg(os.getpgid(self.process.pid), signal.SIGTERM)  # Send the signal to all the process groups
            thread.join()
        return self.process.returncode

##############################################################################
# [Function Name] file2lines
# [ Description ] parse file into list of lines
# [  Arguments  ] fileName : name of the file
##############################################################################
def file2lines(fileName) :
    out = open(fileName)
    lines = out.read().splitlines()
    out.close()
    return lines

##############################################################################
# [Function Name] init
# [ Description ] create directories for experiments
# [  Arguments  ]
##############################################################################
def init(benchmark_dir) :
    call('mkdir -p %s %s' %(tmp_dir,dbg_dir),shell=True)
    tmp = {}
    for benchmark in benchmark_dir : 
        call('mkdir -p %s/%s' %(exp_dir,benchmark.lower()),shell=True)
        opt = ''
        for c in benchmark :
            if c.isupper() :
                opt += c.lower()
        opt  = '-' + opt
        benchmark_option[opt]  = benchmark.lower()
        tmp[benchmark.lower()] = benchmark_dir[benchmark]
    return tmp

##############################################################################
# [Function Name] clear
# [ Description ] clear intermediate files generated during construction
# [  Arguments  ] benchmark
##############################################################################
def clear(benchmark) :
    for f in getLAUT(benchmark) :
        call('rm %s/*.dot %s/*.blif %s/*.vmt %s/*.pfx %s/*.abc %s/cmd %s/aut %s/pred' %(f,f,f,f,f,f,f,f),shell=True)

##############################################################################
# [Function Name] isreach
# [ Description ] returns True if it is a reachability solver
# [  Arguments  ] solver
##############################################################################
def isreach(solver) :
    return solver == 'ic3ia' or solver == 'abc'

##############################################################################
# [Function Name] getLAUT
# [ Description ] return length automaton files of the benchmark
# [  Arguments  ] benchmark
##############################################################################
def getLAUT(benchmark) :
    lauts = []
    for d in benchmark_dir[benchmark] :
        lauts += [join(d,sp) for sp in listdir(d) if isdir(join(d,sp))]
    return lauts

##############################################################################
# [Function Name] getExpResult
# [ Description ] return experimental results of solvers and timeout
# [  Arguments  ] r_dir : result directory
##############################################################################
def getExpResult(r_dir) :
    #r_dir   = '%s/%s' %(exp_dir,benchmark)
    solvers = [ f[0:f.find('.')] for f in listdir(r_dir) if isfile(join(r_dir,f)) and f[f.rfind('.')+1:]=='csv']
    print (solvers)
    data    = {}
    for solver in solvers :
        if isreach(solver) : 
            data[solver] = {'path':[],'ans':[],'time':[],'step':[]}
        else :
            data[solver] = {'path':[],'ans':[],'time':[]}
        
        #pisa_mute_set    = (['1','2','3','4','5','6'])
        #appscan_mute_set = (['43','45','47','52','65','66','72'])
        with open('%s/%s.csv' %(r_dir,solver),'r') as csvfile :
            csvreader = csv.reader(csvfile)
            cnt = 1
            for row in csvreader :
                #if benchmark == 'pisa' :
                    #if row[0] in pisa_mute_set : continue
                #elif benchmark == 'appscan' :
                    #if row[0] in appscan_mute_set : continue
                if cnt == 1 : 
                    cnt += 1
                    TO = row[2][row[2].find('=')+1:]
                    continue
                elif cnt == 2 :
                    cnt += 1
                    continue
                data[solver]['path'].append(row[0])
                data[solver]['ans'].append(row[1])
                data[solver]['time'].append(row[2])
                if isreach(solver) : data[solver]['step'].append(row[3])
    return data,TO

##############################################################################
# [Function Name] regex2dot
# [ Description ] convert regex to dot file
# [  Arguments  ] lauts : list of length automaton files
##############################################################################
def regex2dot(lauts) :
    dbg = None
    if not NDEBUG :
        dbg = '%s/r2d.log' %(dbg_dir)
        print ('[INFO::regex2dot] debug info = %s' %(dbg))
    for i in range(len(lauts)) :
        f = lauts[i]
        print ('fileCnt = %-6d fileName = %s'%(i,f))
        lines = file2lines( join(f,'laut') )
        for line in lines :
            if line == ';' : break
            name = line[ 0 : line.find(' ') ]
            drkt = join( f , name )
            fchr = line[ line.find(' ') + 1 ]
            if   fchr == "\\" : regex = line[ line.find("\\")  : line.rfind(')') + 1 ]
            elif fchr == '\"' : regex = line[ line.find('\"') : line.rfind('\"') + 1 ]
            elif fchr == "\'" : regex = line[ line.find("\'") : line.rfind("\'") + 1 ]
            else              : regex = line[ line.find('~' ) : line.rfind(')' ) + 1 ]
            print ('aut_name = %-4s regex = %s' %(name,regex))
            cmd = Command('java -jar %s -r %s -d %s.dot -o %s/garbage -l FATAL' %(regex2dot_dir,regex,drkt,tmp_dir), None, dbg)
            ret = cmd.run()
            #ret = call('java -jar %s -r %s -d %s.dot -o %s/garbage -l FATAL' 
	   #%(regex2dot_dir,regex,drkt,tmp_dir),stdout=open(dbg,'w'),shell=True)
            if ret != 0 : sys.exit('[ERROR::regex2dot] fails file=%s' %(drkt))
    print ('[INFO::regex2dot] %d cases pass' %(len(lauts)))

##############################################################################
# [Function Name] dot2blif
# [ Description ] convert dot file to blif file
# [  Arguments  ] lauts : list of length automaton files
##############################################################################
def dot2blif(lauts) :
    dbg1,dbg2 = None,None
    if not NDEBUG :
        dbg1 = '%s/d2b.log' %(dbg_dir)
        dbg2 = '%s/abc.log' %(dbg_dir)
        print ('[INFO::dot2blif] debug info = %s , %s' %(dbg1,dbg2))
    for i in range(len(lauts)) :
        f = lauts[i]
        print ('fileCnt = %-6d fileName = %s'%(i,f))
        lines = file2lines( join(f,'laut') )
        for line in lines :
            if line == ';' : break
            name = line[ 0 : line.find(' ') ]
            drkt = join( f , name )
            fchr = line[ line.find(' ') + 1 ]
            if   fchr == "\\" : regex = line[ line.find("\\")  : line.rfind(')') + 1 ]
            elif fchr == '\"' : regex = line[ line.find('\"') : line.rfind('\"') + 1 ]
            elif fchr == "\'" : regex = line[ line.find("\'") : line.rfind("\'") + 1 ]
            else              : regex = line[ line.find('~' ) : line.rfind(')' ) + 1 ]
            print ('aut_name = %-4s regex = %s' %(name,regex))
            cmd = Command('./%s --dot2blif %s.dot %s.blif' %(slender_dir,drkt,drkt), None, dbg1)
            ret = cmd.run()
            #ret = call('./%s --dot2blif %s.dot %s.blif' %(slender_dir,drkt,drkt)
            #,stdout=open(dbg1,'w'),shell=True)
            if ret != 0 : sys.exit('[ERROR::dot2blif] dot2blif fails file=%s' %(drkt))
            if regex == '".*"' or regex == '"~\\(".*"\\)"' or regex == '""' or regex == '~\\(""\\)' or regex == '\\(.*\\)' or regex == '\\(~\\(.*\\)\\)' or regex == '\\(""\\)' or regex == '\\(~\\(""\\)\\)' : continue
            if regex == '\\(~\\(\\(.*\\)\\(\\(.\\)\\&\\(~\\(\\(\\(\\(\\(\\(\\(\\(\\(\\("\\u0041"\\)\\|\\("\\u0053"\\)\\)\\|\\("\\u0043"\\)\\)\\|\\("\\u0052"\\)\\)\\|\\("\\u0020"\\)\\)\\|\\("\\u002e"\\)\\)\\|\\("\\u002d"\\)\\)\\|\\("\\u0040"\\)\\)\\|\\("\\u003a"\\)\\)\\|\\("\\u002f"\\)\\)\\)\\)\\(.*\\)\\)\\)' : continue
            if regex == '".*"' or regex == '"~\\(".*"\\)"' or regex == '""' or regex == '~\\(""\\)' : continue
            if regex == '\\(~\\(\\(.*\\)\\("a*b*"\\)\\(.*\\)\\)\\)' : continue
            abc_cmd = '%s/abc_cmd_%s' %(tmp_dir,drkt.replace('/','_'))
            abcCmdFile = open(abc_cmd,'w')
            abcCmdFile.write('read %s.blif' %(drkt))
            abcCmdFile.write('\nespresso')
            abcCmdFile.write('\nwrite %s.blif' %(drkt))
            abcCmdFile.close()
            cmd = Command('./%s -f %s' %(abc70930_dir,abc_cmd), None, dbg2)
            ret = cmd.run()
            #ret = call('./%s -f %s/abc_cmd' %(abc70930_dir,tmp_dir)
            #,stdout=open(dbg2,'w'),shell=True)
            if ret != 0 : sys.exit('[ERROR::dot2blif] abc minimization fails file=%s' %(drkt))
    print ('[INFO::dot2blif] %d cases pass' %(len(lauts)))

##############################################################################
# [Function Name] blif2vmt
# [ Description ] convert blif file to vmt file
# [  Arguments  ] lauts : list of length automaton files
##############################################################################
def blif2vmt(lauts) :
    dbg = None
    if not NDEBUG :
        dbg = '%s/b2v.log' %(dbg_dir)
        print ('[INFO::blif2vmt] debug info = %s' %(dbg))
    for i in range(len(lauts)) :
        f = lauts[i]
        print ('fileCnt = %-6d fileName = %s'%(i,f))
        lines = file2lines( join(f,'laut') )
        for line in lines :
            if line == ';' : break
            drkt = join( f , line[ 0 : line.find(' ')] )
            cmd = Command('./%s --blif2vmt %s.blif %s.vmt' %(slender_dir,drkt,drkt), None, dbg)
            ret = cmd.run()
            #ret  = call('./%s --blif2vmt %s.blif %s.vmt' %(slender_dir,drkt,drkt)
            #,stdout=open(dbg,'w'),shell=True)
            if ret != 0 : sys.exit('[ERROR::blif2vmt] blif2vmt fails file=%s' %(drkt))
    print ('[INFO::blif2vmt] %d cases pass' %(len(lauts)))

##############################################################################
# [Function Name] readCmd
# [ Description ] construct laut through commands
# [  Arguments  ] lauts : list of length automaton files
##############################################################################
def readCmd(lauts) :
    dbg = None
    if not NDEBUG :
        dbg = '%s/readcmd.log' %(dbg_dir)
        print ('[INFO::readCmd] debug info = %s' %(dbg))
    for i in range(len(lauts)) :
        f = lauts[i]
        print ('fileCnt = %-6d fileName = %s'%(i,f))
        cmdFile = join(f,'laut')
        cmd = Command('./%s --readCmd %s' %(slender_dir,cmdFile), None, dbg)
        ret = cmd.run()
        #ret = call('./%s --readCmd %s' %(slender_dir,cmdFile)
        #,stdout=open(dbg,'w'),shell=True)
        if ret != 0 : sys.exit('[ERROR::readCmd] readCmd fails file=%s' %(cmdFile))
    print ('[INFO::readCmd] %d cases pass' %(len(lauts)))

##############################################################################
# [Function Name] solve
# [ Description ] conduct experiments
# [  Arguments  ] benchmark
#                 lauts   : list of legnth automaton files
#                 solver  : name of solver
#                 TO      : time out seconds
#                 1 = sat ; 0 = unsat ; x = error ; t = timeout 
##############################################################################
def solve(benchmark,lauts,solver,TO) :
    if isreach(solver) : header = 'path,sat,time,step'
    else               : header = 'path,sat,time'
    if benchmark == None :
        record = open('%s/%s.csv' %(lauts[0],solver),'w')
        record.write('benchmark=%s,solver=%s,TO=%.6f\n%s' %(lauts[0],solver,TO,header))
    else :
        record = open('%s/%s/%s.csv' %(exp_dir,benchmark,solver),'w')
        record.write('benchmark=%s,solver=%s,TO=%.6f\n%s' %(benchmark,solver,TO,header))
    for i in range(len(lauts)) : 
        f = lauts[i]
        print ('fileCnt = %-6d fileName = %s'%(i,f))
        solve_sub(solver,lauts[i],record,TO)
    record.close()

def solve_sub(solver,laut,record,TO) :
    target  = join(laut,'sink.%s' %(solver_ext[solver]))
    exePath = solver_dir[solver]
    option  = solver_opt[solver]
    tmpPath = '%s/exp_%s_%s' %(tmp_dir,solver,laut.replace('/','_'))

    #if not isfile(target) : 
        #sys.exit('[ERROR::solve_sub] file=%s does not exist' %(target))
    
    #TODO: ABC dump information into tmp/abc.INFO
    if solver == 'ABC' :
        tmpPath = 'tmp/abc.INFO'
        cmd = Command('%s %s %s --log-dir tmp' %(exePath,option,target), TO, None)
    else :
        cmd = Command('%s %s %s' %(exePath,option,target), TO, tmpPath)
    
    ts  = time.time()
    ret = cmd.run()
    dt  = time.time() - ts

    if ret != 0 :
        if   isreach(solver) : record.write('\n%s,t,0.0,0' %(target))
        elif dt*10 < TO*8    : record.write('\n%s,x,0.0' %(target))
        else                 : record.write('\n%s,t,0.0'   %(target))
    else :
        lines = file2lines(tmpPath)
        # handle core dump / segmentation fault condition
        if not lines :
            if isreach(solver) : 
                record.write('\n%s,x,%.6f,0' %(target,0.0))
            else : 
                record.write('\n%s,x,%.6f' %(target,0.0))
            return  
        if   solver == 'cvc4' : exp_cvc4 (target,dt,lines,record)
        elif solver == 'norn' : exp_norn (target,dt,lines,record)
        elif solver == 'z3'   : exp_z3   (target,dt,lines,record)
        elif solver == 's3p'  : exp_s3p  (target,dt,lines,record)
        elif solver == 'ABC'  : exp_ABC  (target,dt,lines,record)
        elif solver == 'trau' : exp_trau (target,dt,lines,record)
        elif solver == 'abc'  : exp_abc  (target,dt,lines,record)
        elif solver == 'ic3ia': exp_ic3ia(target,dt,lines,record)
        elif solver == 'sloth': exp_sloth(target,dt,lines,record)
    '''
    try :
        if solver == 'ABC' :
            tmpPath = 'tmp/abc.INFO'
            ts  = time.time()
            call('%s %s %s --log-dir tmp' %(exePath,option,target),timeout=TO,shell=True)
            te  = time.time()
        else :
            ts  = time.time()
            call('%s %s %s' %(exePath,option,target)
            ,timeout=TO,stdout=open(tmpPath,'w'),shell=True)
            te  = time.time()
    except TimeoutExpired :
        if   isreach(solver) : record.write('\n%s,t,0.0,0' %(target))
        else                 : record.write('\n%s,t,0.0'   %(target))
    else :
        lines = file2lines(tmpPath)
        # handle core dump / segmentation fault condition
        if not lines :
            if isreach(solver) : 
                record.write('\n%s,x,%.6f,0' %(target,0.0))
            else : 
                record.write('\n%s,x,%.6f' %(target,0.0))
            return  
        if   solver == 'cvc4' : exp_cvc4 (target,te-ts,lines,record)
        elif solver == 'norn' : exp_norn (target,te-ts,lines,record)
        elif solver == 'z3'   : exp_z3   (target,te-ts,lines,record)
        elif solver == 's3p'  : exp_s3p  (target,te-ts,lines,record)
        elif solver == 'ABC'  : exp_ABC  (target,te-ts,lines,record)
        elif solver == 'fat'  : exp_fat  (target,te-ts,lines,record)
        elif solver == 'abc'  : exp_abc  (target,te-ts,lines,record)
        elif solver == 'ic3ia': exp_ic3ia(target,te-ts,lines,record)
    '''

def exp_cvc4(target,dt,lines,record) :
    if   lines[0] == 'sat'   : sat = '1'
    elif lines[0] == 'unsat' : sat = '0'
    else                     : sat = 'x';\
                               dt  = 0.0
    record.write('\n%s,%s,%.6f' %(target,sat,dt))

def exp_norn(target,dt,lines,record) :
    if   lines[0] == 'sat'   : sat = '1'
    elif lines[0] == 'unsat' : sat = '0'
    else                     : sat = 'x';\
                               dt  = 0.0
    record.write('\n%s,%s,%.6f' %(target,sat,dt))

def exp_z3(target,dt,lines,record) :
    sat = 'x'
    for line in lines :
        if   line == 'sat'   : sat = '1'
        elif line == 'unsat' : sat = '0'
    if sat == 'x' : dt = 0.0
    record.write('\n%s,%s,%.6f' %(target,sat,dt))

def exp_s3p(target,dt,lines,record) :
    sat = 'x'
    vok = False
    for line in lines :
        if line == '* v-ok' : 
            vok = True
        if line == '>> SAT' :
            if vok == True :
                sat = '1'
                break
        if line == '>> UNSAT':
            sat = '0'
            break
    if sat == 'x' : dt = 0.0
    record.write('\n%s,%s,%.6f' %(target,sat,dt))

#deal cases when ret not zero
#def exp_s3p_ret(target,dt,lines,record) :
#    sat = 't'
#    for line in lines :
#        if line.find('traceback') != -1 : sat = 'x'
#    dt = 0.0
#    record.write('\n%s,%s,%.6f' %(target,sat,dt))

def exp_sloth(target,dt,lines,record) :
    sat = 'x'
    for line in lines :
        if line == 'unsat' : sat = '0'
        if line == 'sat'   : sat = '1'
    if sat == 'x' : dt = 0.0
    record.write('\n%s,%s,%.6f' %(target,sat,dt))

def exp_ABC(target,dt,lines,record) :
    sat = 'x'
    for line in lines :
        if   line.find('is_sat: SAT'  ) != -1 : sat = '1'
        elif line.find('is_sat: UNSAT') != -1 : sat = '0'
    if sat == 'x' : dt = 0.0
    record.write('\n%s,%s,%.6f' %(target,sat,dt))

def exp_trau(target,dt,lines,record) :
    sat = 'x'
    #splitlinefound = False
    #for i in range(1,len(lines)+1) :
    #    if lines[-i].find('Approximation') != -1 :
    #        splitlinefound = True
    #    if splitlinefound :
    #        if   lines[-i].find('>> SAT')   != -1 : sat = '1';\
    #                                                break
    #        elif lines[-i].find('>> UNSAT') != -1 : sat = '0';\
    #                                                break
    for i in range(1,len(lines)+1) :
        if lines[-i].find('UNSAT') != -1 : sat = '0'
        elif lines[-i].find('SAT') != -1 : sat = '1'
    ##print(lines[0])
    ##if   lines[0].find('UNSAT') != -1 : sat = '1'
    ##elif lines[0].find('SAT'  ) != -1 : sat = '0'
    if sat == 'x' : dt = 0.0
    record.write('\n%s,%s,%.6f' %(target,sat,dt))

#def exp_trau_ret(target,dt,lines,record) :
#    sat = 't'
#    for i in range(1,len(lines)+1) :
#        if lines[-i].find('Segmentation fault') != -1 : sat = 'x'
#    dt = 0.0
#    record.write('\n%s,%s,%.6f' %(target,sat,dt))

def exp_abc(target,dt,lines,record) :
    if   lines[-1].find('Output 0 of miter \"sink\" was asserted in frame') != -1 :
        sat = '1'
        bpos = lines[-1].find('frame') + 6
        step = lines[-1][bpos:]
        step = step[:step.find('.')]
    elif lines[-1].find('Property proved.') != -1 :
        sat = '0'
        for line in lines :
            v = line.split()
            if not v : continue
            if v[0] == 'Invariant' :
                bpos = v[1].find('[') + 1
                epos = v[1].rfind(']')
                step = v[1][bpos:epos]
    else : 
        sat  = 'x'
        step = 'x'
        dt   = 0.0
    record.write('\n%s,%s,%.6f,%s' %(target,sat,dt,step))

def exp_ic3ia(target,dt,lines,record) :
    if   lines[-1] == 'safe'   : sat = '0'
    elif lines[-1] == 'unsafe' : sat = '1'
    else                       : sat = 'x';\
                                 dt  = 0.0
    #TODO safe <-> frame , unsafe <-> step
    step = 'x'
    if sat == '1' :
        for i in range(1,len(lines)+1) :
            if lines[-i][0:2] == ';;' :
                v = lines[-i].split()
                if v[1] != 'step' :
                    sys.exit('[ERROR::expRecord] \';;\' not followed by \'step\'')
                else :
                    step = v[2]
                break
    elif sat == '0' :
        for i in range(1,len(lines)+1) :
            if (lines[-i][0:8] == 'fixpoint') :
                v = lines[-i].split()
                step = v[4]
                break
    record.write('\n%s,%s,%.6f,%s' %(target,sat,dt,step))

##############################################################################
# [Function Name] stats
# [ Description ] statistics
# [  Arguments  ] benchmark
##############################################################################
def stats(benchmark,lauts) :
    if benchmark : r_dir = '%s/%s' %(exp_dir,benchmark)
    else         : r_dir = '%s' %(lauts[0])
    
    data,TO = getExpResult(r_dir)
    #data,TO = getExpResult(benchmark)
    
    # Default reference set to z3, otherwise pick the first one as reference
    if 'z3' in data : ref = 'z3'
    else            : ref = data.keys()[0]
    
    # Case Number Checking
    num = set()
    for solver in data : num.add(len(data[solver]['path']))
    if len(num) != 1 : sys.exit("[ERROR::CC] number of cases not match")
    num = list(num)[0]

    # Abort/Timeout/Answer/Cumulative Time
    # For each case, if the reference solves it in time limit w/o error(i.e., 0 or 1), 
    # other solvers take the result as the answer;
    # if the reference exceeds timeout or aborts(i.e., t or x), other solvers take 
    # their own results as the answer
    
    for solver in data :
        data[solver]['abt']   = 0
        data[solver]['to']    = 0
        data[solver]['sat']   = 0
        data[solver]['unsat'] = 0
        data[solver]['ic']    = 0
        data[solver]['crt']   = 0

        for i in range(num) :
            rsat = data[ref]['ans'][i]
            sat  = data[solver]['ans'][i]
            if   sat == 'x' : data[solver]['abt'] += 1
            elif sat == 't' : data[solver]['to']  += 1
            elif sat == '1' :
                if rsat == '0' : data[solver]['ic']  += 1
                else           : 
                    data[solver]['sat'] += 1
                    data[solver]['crt'] += float(data[solver]['time'][i])
            else :
                if rsat == '1' : data[solver]['ic']    += 1
                else           :
                    data[solver]['unsat'] += 1
                    data[solver]['crt'] += float(data[solver]['time'][i])

    # Exclusive Solving
    for i in range(num) :
        zero,one,i0,i1 = 0,0,0,0
        for solver in data :
            data[solver]['ex'] = 0
            sat = data[solver]['ans'][i]
            if   sat == '1' : 
                one  += 1
                i1   =  solver
            elif sat == '0' : 
                zero += 1
                i0   =  solver
        if   one == 1 and zero == 0 : data[i1]['ex'] += 1
        elif one == 0 and zero == 1 : data[i0]['ex'] += 1
    
    # Write Statistics
    if benchmark : sfile = open('%s/%s/stats' %(exp_dir,benchmark),'w');\
                   sfile.write('benchmark       : %s\n' %(benchmark))
    else         : sfile = open('%s/stats' %(r_dir),'w');\
                   sfile.write('file            : %s\n' %(r_dir))
    sfile.write('timeout         : %s (s)\n' %(TO))
    sfile.write('number of cases : %d\n\n' %(num))
    sfile.write('%-8s %-10s %-10s %-10s %-10s %-15s %-10s %-10s\n' 
    %('solver','Abort','Timeout','SAT','UNSAT','Inconsistent','Exclusive','Cumulative Time'))
    for s in data :
        if s == ref : solver = s + '*'
        else        : solver = s
        sfile.write('%-8s %-10s %-10s %-10s %-10s %-15s %-10s %-10s\n' 
        %(solver,data[s]['abt'],data[s]['to'],data[s]['sat'],data[s]['unsat'],
                 data[s]['ic'] ,data[s]['ex'],data[s]['crt']))
    sfile.write('\n* : reference solver')
    sfile.close()

##############################################################################
# [Function Name] plot
# [ Description ] plot
# [  Arguments  ] benchmark
##############################################################################
def plot(benchmark) :
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    import matplotlib.patches as mpat
    from matplotlib.font_manager import FontProperties
    
    r_dir = '%s/%s' %(exp_dir,benchmark)
    
    data,TO = getExpResult(r_dir)
    #data,TO = getExpResult(benchmark)
    
    # Default reference set to z3, otherwise pick the first one as reference
    if 'z3' in data : ref = 'z3'
    else            : ref = data.keys()[0]
    
    # trim and sort
    # Eliminate the TO/Aborted/Inconsistent cases then sort non-decreasingly
    trim = {}
    for solver in data :
        time = []
        for i in range(len(data[solver]['path'])) :
            rsat = data[ref]['ans'][i]
            sat  = data[solver]['ans'][i]
            if sat != 't' and sat != 'x' :
                if sat == '1' and rsat == '0' or sat == '0' and rsat == '1' : continue
                time.append(float(data[solver]['time'][i]))
        time.sort()
        trim[solver] = time
    
    plotCumTime(benchmark,trim,plt,mpat)
    
    for solver in data :
        if isreach(solver) :
            plotScatter(benchmark,solver,data[solver],plt,mpat)

def plotScatter(benchmark,solver,data,plt,mpat) :
    unsat,sat = [ [] for x in range(2) ] , [ [] for x in range(2) ]
    for i in range(len(data['ans'])) :
        if   data['ans'][i] == '0' :
            unsat[0].append(data['step'][i])
            unsat[1].append(data['time'][i])
        elif data['ans'][i] == '1' :
            sat[0].append(data['step'][i])
            sat[1].append(data['time'][i])

    plt.scatter(sat[0],sat[1],c='b')
    plt.title('%s (SAT Cases) %s : Time vs Step Scatter Plot (# of case = %d)' %(benchmark.title(),solver.title(),len(sat[0])))
    plt.xlabel('Step Count')
    plt.ylabel('Run Time (s)')
    plt.savefig('%s/%s/sat_step_%s.jpg' %(exp_dir,benchmark,solver),dpi=DPI)
    plt.cla()
    plt.clf()
    
    plt.scatter(unsat[0],unsat[1],c='b')
    plt.title('%s (UNSAT Cases) %s : Time vs Frame Scatter Plot (# of case = %d)' %(benchmark.title(),solver.title(),len(unsat[0])))
    plt.xlabel('Frame Index')
    plt.ylabel('Run Time (s)')
    plt.savefig('%s/%s/unsat_frame_%s.jpg' %(exp_dir,benchmark,solver),dpi=DPI)
    plt.cla()
    plt.clf()

def plotCumTime(benchmark,data,plt,mpat) :
    # plot cumulative time vs case index
    solver_list = ['ic3ia','abc','z3','norn','s3p','trau','ABC','cvc4','sloth']

    k = 256.0
    color_map = { 'ic3ia' : (96/k,104/k,128/k), 
                  'abc'   : (220/k,20/k,60/k),
                  'z3'    : (121/k,128/k,32/k),
                  'norn'  : (204/k,0/k,255/k),
                  's3p'   : (0/k,255/k,170/k),
                  'trau'  : (255/k,136/k,0/k),
                  'ABC'   : (0/k,204/k,255/k),
                  'cvc4'  : (34/k,139/k,34/k),
                  'sloth' : (255/k,255/k,255/k)}

    label_map = { 'ic3ia' : 'Slender',
                  'abc'   : 'Slender-b',
                  'z3'    : 'Z3STR3'   ,
                  'norn'  : 'Norn'     ,
                  's3p'   : 'S3P'      ,
                  'trau'  : 'TRAU'     ,
                  'ABC'   : 'ABC'      ,
                  'cvc4'  : 'CVC4'     ,
                  'sloth' : 'Sloth'    }
    
    fig = plt.figure()
    ax  = plt.subplot(111)
    for solver in solver_list :
        if solver not in data : continue
        d = np.array(data[solver])
        np.cumsum(d,out=d)
        ax.plot(np.arange(len(d)),d,color=color_map[solver],label=label_map[solver])
    box = ax.get_position()
    ax.set_position([box.x0,box.y0,box.width*0.8,box.height])
    ax.legend(loc='center left',bbox_to_anchor=(1,0.5))
    plt.ylabel('Time (s)')
    plt.xlabel('Number of Solved Instances')
    plt.savefig('%s/%s/crt.jpg' %(exp_dir,benchmark),dpi=DPI)
    plt.cla()
    plt.clf()

##############################################################################

def parse(argv) :
    if   len(argv) < 1 or len(argv) > 6 :
        sys.exit('[ERROR::parse] invalid argc=%d, type -h for help' %(len(argv)))
    elif argv[0] == '-h' : opt_help(argv)
    elif argv[0] == 'single':
        if len(argv) == 4 : opt_single(argv)
        else              : opt_single(argv,float(argv[4]))
    elif len(argv) == 2   : opt2(argv)
    else                  : opt4(argv)

def opt_help(argv) :
    bs = 'benchmark_option: '
    for opt in benchmark_option :
        bs += '%s=%s ' %(opt,benchmark_option[opt])
    print ('''
    %s
    
    remove intermediate files: clear < benchmark_option >
    statistics               : stats < benchmark_option >
    plot figures             : plot  < benchmark_option >

    construction  : build  < benchmark_option > < -r2d | -d2b | -b2v | -cmd | -all >
    
    solve         : solve  < benchmark_option >
                           < -ic3ia | -cvc4 | -z3 | -s3p | -abc | -norn | -trau | -all >
                           [ time out (default=%.1fs) ]

    single file   : single < folder_path > 
                           < -r2d   | -d2b  | -b2v | -cmd | -all | -pass >
                           < -ic3ia | -cvc4 | -z3  | -s3p | -abc | -norn | -trau | -all | -pass >
                           [ time out (default=%.1fs) ]
    
    for command "single", please enter a path of a folder containing only one laut file
              ''' %(bs,TIMEOUT,TIMEOUT))

def opt2(argv) :
    if argv[1] not in benchmark_option :
        sys.exit('[ERROR::opt2] invalid opt=%s, type -h for help' %(argv[1]))
    t = benchmark_option[argv[1]]
    if   argv[0] == 'clear'   : clear(t)
    elif argv[0] == 'stats'   : stats(t,None)
    elif argv[0] == 'plot'    : plot(t)
    else                      : 
        sys.exit('[ERROR::opt2] invalid opt=%s, type -h for help' %(argv[0]))

def opt4(argv) :
    if   argv[0] == 'build' : lauts = getLAUT(benchmark_option[argv[1]]);\
                              opt_build(argv[2],lauts)
    elif argv[0] == 'solve' : 
        t = benchmark_option[argv[1]]
        if len(argv) == 3 : opt_solve(t,argv[2])
        else              : opt_solve(t,argv[2],float(argv[3]))
    else : 
        sys.exit('[ERROR::opt3] invalid opt=%s, type -h for help' %(argv[0]))

def opt_build(opt,lauts) :
    if   opt == '-r2d' : regex2dot(lauts)
    elif opt == '-d2b' : dot2blif(lauts)
    elif opt == '-b2v' : blif2vmt(lauts)
    elif opt == '-cmd' : readCmd(lauts)
    elif opt == '-all' : regex2dot(lauts);\
                         dot2blif(lauts); \
                         blif2vmt(lauts); \
                         readCmd(lauts)
    else : 
        sys.exit('[ERROR:opt_build] invalid opt=%s, type -h for help' %(opt))

def opt_solve(benchmark,opt,TO=TIMEOUT) :
    if   opt == '-all' : solvers = solver_list
    else               : solvers = [ opt[ opt.rfind('-') + 1 : ] ]
    lauts = getLAUT(benchmark)
    for solver in solvers : solve(benchmark,lauts,solver,TO)

def opt_single(argv,TO=TIMEOUT) :
    laut = [argv[1]]
    if   argv[2] == '-r2d' : regex2dot(laut)
    elif argv[2] == '-d2b' : dot2blif(laut)
    elif argv[2] == '-b2v' : blif2vmt(laut)
    elif argv[2] == '-cmd' : readCmd(laut)
    elif argv[2] == '-all' : regex2dot(laut);\
                             dot2blif(laut); \
                             blif2vmt(laut); \
                             readCmd(laut)
    elif argv[2] == '-pass': pass
    
    if   argv[3] == '-pass': return
    elif argv[3] == '-all' : solvers = solver_list
    else                   : solvers = [ argv[3][ argv[3].rfind('-') + 1 : ] ]
    
    for solver in solvers  : solve(None,laut,solver,TO)
    stats(None,laut)

if __name__ == '__main__' :
    benchmark_dir = init(benchmark_dir)
    parse(sys.argv[1:])
