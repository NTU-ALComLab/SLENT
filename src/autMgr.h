#ifndef _AUT_MGR_
#define _AUT_MGR_
#define FORCE_ASSIGNED
//#define UTF16_ENCODE
#ifdef  UTF16_ENCODE
    #define INPUT_ENCODE_BIT_NUM    16
    #define MAX_ENCODE              65535
#else
    #define INPUT_ENCODE_BIT_NUM    7
    #define MAX_ENCODE              127
#endif
#define PI_NUM                      8
#define LVAR_BIT_NUM                9
#define MODULE_TYPE_NUM             3
#define FORCE_SIGNED_BIT_NUM        3
#define EPSILON_ENCODE              0
#define LEFT_ANGLE_BRACKET_ENCODE   1
#define RIGHT_ANGLE_BRACKET_ENCODE  2
#define MAX_SPECIAL_ALPHABET_ENCODE 2
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <assert.h>
#include <bitset>
#include <cstdio>
#include <climits>
#include "utility.h"
#include "typedef.h"
#define TGRAPH_NDEBUG
#define VMTNODE_NDEBUG
//#define AUT_NDEBUG
#define AUT_PARAM_NDEBUG
#define AUT_OP_NDEBUG
#define AUTMGR_NDEBUG

namespace aut {

class TGEdge{
    friend class AutMgr;
    friend class TGraph;
    public:
        TGEdge(const size_t& sIdx, const size_t& eIdx): _sIdx(sIdx), _eIdx(eIdx) {}
        void print() const;
        void write(const CubeMap&, const CubeMap&, const vector<string>&, ofstream&);
        void writeRangeMinterm(const size_t&, const size_t&, ofstream&);
    private:
        size_t         _sIdx;
        size_t         _eIdx;
        string         _tBitString;
        vector<size_t> _labels;
};

class TGraph{
    friend class AutMgr;
    public:
        TGraph(const char* fileName) {init();parse(fileName);}
        // I/O
        void parse(const char*);
        void write(const char*);
        void write(const string&);
        void print() const;
    private:
        void init();
        bool isAccepting(const string&);
        void parseLabels(const string&, vector<size_t>&);
        size_t label2Decimal(const string&);
        
        set<char>          _numbers;
        map<char,size_t>   _h2dMap;
        CubeMap            _downCubeMap;
        CubeMap            _rangeCubeMap;
        vector<string>     _stateBitStringList;
        size_t             _stateBitNum;
        size_t             _initStateIdx;
        vector<size_t>     _oList;
        TGEdgeList         _tList;
};

class VmtNode{
    friend class Aut;
    friend class AutMgr;
    public:
        
        VmtNode (const string& name,const size_t& idx=0): _name(name), _idx(idx) 
        { _paramList.assign(PI_NUM,VmtNodeList()); _type = getType(name) ; 
          _bname = ""; _bit = 0; _pad = 0; _source=0; _flag = 0; }

        VmtNode (const string& name,VmtNode* source) : _name(name), _source(source)
        { _paramList.assign(PI_NUM,VmtNodeList()); _type = PARAM;
          _bname = ""; _bit = 0; _pad = 0; _idx = 0 ; _flag = 0; }
        
        void           print   (const size_t&)           const;
        void           printPARAM   ()           const;
        void           write   (const size_t&,ofstream&) const;
        void           setType (const VmtType& type)           {_type = type;}
        VmtType        getType (const string& name)      const;
        const string&  getName ()                        const  {return _name;}
    private:
        bool           hasParam() const;
        bool           haveSameParam(VmtNode*) const;
        void           addChild(VmtNode*);
        void           clearParam(const size_t&);
        void           buildParam(const size_t&);
        void           collectPARAM(VmtNodeList&);
        void           spotNEG();
        void           writeParamHead(ofstream&) const;
        void           writeParamBody(const string&,ofstream&) const;
        void           shiftStateVar(const size_t&);
        size_t         getBit() const;
        void           setBit(const size_t&,bool&);
        //TODO construct sequential circuit
        void           buildBLIF(int&);
        void           setBitNum(); 
        void           setBitNumUP(bool&);
        void           setBitNumDN(bool&);
        void           writeMODEL (ofstream&,vector<set<size_t> >&,const bool& isPred=0);
        void           writeSUBCKT(ofstream&,vector<set<size_t> >&,bool&,bool&,const bool& isPred=0);
        string         _name;
        string         _bname;     // name for BLIF
        size_t         _idx;
        size_t         _bit;       // number of fanout bits for type=OP for BLIF
        size_t         _pad;       // number of padding zeros for type=(LEN|NUM)
        VmtType        _type;
        VmtNodeList    _children;
        VmtNode*       _source;    // used for _type == PARAM
        VarList        _paramList; // 0-7 follow the order of VmtType
        mutable size_t _flag;
};

class Aut{
    friend class VmtNode;
    friend class AutMgr;
    public :
        Aut(){
            init();
        }
        Aut(const char* fileName){ 
            init(fileName);
            parse(fileName);
        }
        Aut(const string& fileName){ 
            init(fileName);
            parse(fileName.c_str()); 
        }
        Aut ( const string& fileName, const string& lvarIdxStr, const AutOpType& type ){
            assert( (type == TRKLEN || type == TRKSTR || type == PREFIX || type == SUFFIX) );
            init(fileName);
            parse(fileName.c_str());
            if      (type == TRKLEN) addlen(lvarIdxStr);
            else if (type == TRKSTR) trkstr(lvarIdxStr);
            else if (type == PREFIX) prefix(lvarIdxStr);
            else                     suffix(lvarIdxStr);
        }
        Aut ( Aut* a1, Aut* a2, const size_t& alpha, const AutOpType& type) {
            assert( (type == REPLACE) );
            init();
            replace(a1,a2,alpha);
        }
        Aut ( Aut* a1, Aut* a2, const AutOpType& type ){
            assert( (type == INTERSECT || type == UNION || type == CONCATE || type == REPLACE_A4) );
            init();
            if      (type == INTERSECT) intersect(a1,a2);
            else if (type == UNION    ) aut_union(a1,a2);
            else if (type == CONCATE  ) concate(a1,a2);
            else                        replace_A4(a1,a2);
        }

        // Static Member Function
        // static member function cannot have const qualifier (don't have this)
        static string   BLIFIndent;
        static STRList  initPISymbolS();
        static CHRList  initPISymbolC();
        static VmtNode* initConst(const VmtType&);
        static VmtNode* initSpecialAlphabet(const AType&);
        static VarList  initPIList();
        static void     printStaticDataMember();
        static bool     isPI(const VmtType&);
        static bool     isCurPI(const VmtType&);
        static bool     isLEAF(const VmtType&);
        static bool     isCONST(const VmtType&);
        static bool     isINT(const VmtType&);
        static bool     isPREDINT(const VmtType&);
        static bool     isPARAMINT(const VmtType&);
        static bool     isPARAMBOOL(const VmtType&);
        static bool     isRETBOOL(const VmtType&);
        static bool     isPREDRETBOOL(const VmtType&);
        static bool     isPREDRETINT(const VmtType&);
        static bool     isIMD(const VmtType&);
        static bool     isOP(const VmtType&);
        static bool     isARITH(const VmtType&);
        static bool     isSO(const VmtType&);
        static bool     isMO(const VmtType&);
        static bool     isPISymbol(const char&);
        static bool     isSpecialString(const string&);
        static string   getTypeStr(const VmtType&);
        static string   getTypeStr(const size_t&);
        static VmtType  getPITypeByName(const string&);
        // Non-Static Member Function
        // I/O
        void            test();
        void            print() const;
        void            printPARAMList() const;
        void            printPIList() const;
        void            parse(const char*);
        void            write(const char*) const;
        void            write(const string&) const;
        // Operations
        string          CSNSEquiv(const VmtType&) const;
        // Unary 
        void            addlen(const string&);
        void            trkstr(const string&);
        size_t          mark();
        void            prefix(const string&);
        void            suffix(const string&);
        void            addpred(const STRList&, const STRList&);
        void            isempty(const string&);
        // Binary
        void            intersect(Aut*,Aut*);
        void            aut_union(Aut*,Aut*);
        void            concate(Aut*,Aut*);
        void            replace(Aut*,Aut*,const size_t&);
        void            replace_A4(Aut*,Aut*);
    private:
        // Static Member
        static void     expandPIList(const VmtType&,const size_t&);
        static void     vmtTokenize(const string&,vector<string>&,vector<string>&);
        static VmtNode* buildVmtNode(const string&,size_t,size_t,Str2VmtNodeMap&);
        static bool     isReservedString(const string&);
        static void     check(Aut*);
        static void     check(Aut*, const size_t&);
        static void     check(Aut*, Aut*);
        static void     check(Aut*, Aut*, const size_t&);
        static void     writeFAList(const size_t&,const string&,const string&,ofstream&);
        static void     writeFAList(const size_t&,const string&,VmtNode*,ofstream&);
        static void     writeFAList(bool&,bool&,const size_t&,const string&,VmtNode*,ofstream&);
        static void     writeFAListARITH(bool&,bool&,const size_t&,const string&,VmtNode*,ofstream&);
        static void     writeINPUTList(const size_t&,const VarList&,ofstream&);
        static void     writeUSFA(ofstream&);
        static void     writeUHA(ofstream&);
        static void     writeSFA(const size_t&,ofstream&);
        static void     writeEQ(const size_t&,ofstream&);
        static void     writeINC1(const size_t&,ofstream&);
        
        static size_t   inputBitNum;
        static STRList  piSymbolS;
        static CHRList  piSymbolC;
        static VarList  piList; // 0-7 follows the order of VmtType
        static VmtNode* const0;
        static VmtNode* const1;
        static VmtNode* epsilon;
        static VmtNode* leftAngle;
        static VmtNode* rightAngle;
        // Non-Static Member Function
        void            init(const string& fileName="NONAME");
        void            initVMap();
        void            buildVMap(const VmtType&);
        void            buildVMap(const VmtNodeList&);
        void            assignGlobalPIList(const VmtType&);
        void            clearParam();
        void            buildParam();
        void            collectPARAM();
        void            spotNEG();
        void            renameDef();
        void            shiftStateVar(const size_t&);
        void            parseDef(const string&, Str2VmtNodeMap&);
        void            parsePred(const string&, size_t&, Str2VmtNodeMap&);
        void            defineFun(const string&, const string&, VmtNodeList&);
        void            defineFun(const string&, const string&, VmtNodeList&, Str2VmtNodeMap&);
        void            defineFun(const string&, const VmtType&, const string&, VmtNodeList&, Str2VmtNodeMap&);
        void            defineFun(const string&, const string&, VmtNodeList&, void (Aut::*)(VmtNode*));
        void            addParamNode(const string&, VmtNode*);
        void            addParamNode(const string&, VmtNode*, const size_t&, const bool&);
        void            writeDeclareFun(const VmtType&, ofstream&) const;
        void            writeNextFun(const VmtType&, int&, ofstream&) const;
        void            writeDefineFun(VmtNode*,ofstream&,const bool& needParam=1) const;
        void            renameITO(const string&, VmtNode*);
        void            renameITO1Aut();
        void            renameITOs2Aut(Aut*,Aut*);
        void            integrate(Aut*, Aut*);
        void            integrateMerge(const VmtType&,Aut*,Aut*);
        size_t          addEVar(const size_t&);
        size_t          addStateVar(const size_t&);
        void            addLVar(const size_t&);
        VmtNode*        getI() const;
        VmtNode*        getO() const;
        VmtNode*        getT() const;
        void            setI(VmtNode*);
        void            setO(VmtNode*);
        void            setT(VmtNode*);
        // Non-Static Data Member
        VarList         _piList;
        VmtNodeList     _imdList;
        VmtNodeList     _itoList;
        VmtNodeList     _predList;
        VmtNodeList     _PARAMList;
        Str2VmtNodeMap  _vmap;
        size_t          _stateVarNum;
        string          _name;
};

class AutMgr{
    public :
        AutMgr() {_gflag = 0;}
        // Converter
        void    dot2blif(const char*,const char*);
        void    blif2vmt(const char*,const char*);
        void    aig2vmt (const char*,const char*);
        //void    vmt2blif (const char*,const char*);
        // Automata Operation
        void    readCmdFile(const char*);
        size_t& getGFlag() {return _gflag;}
    private:
        // Converter
        void    parseCubeList(Aut*, const string&, const vector<string>&, const VmtNodeList&, int&);
        string  cube2vmt(const string& cube, const VmtNodeList&);
        // Data Member
        size_t  _gflag;
};

};
#endif
