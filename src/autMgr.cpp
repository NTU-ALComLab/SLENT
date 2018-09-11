#include "autMgr.h"

namespace aut {

void AutMgr::dot2blif(const char* inFileName, const char* outFileName)
{
    #ifndef AUTMGR_NDEBUG
        cout << ">> dot2blif::inFile=" << inFileName << " outFile=" << outFileName << endl;
    #endif
    TGraph* tgraph = new TGraph(inFileName);
    tgraph->write(outFileName);
}

void AutMgr::blif2vmt(const char* inFileName, const char* outFileName)
{
    string line;
    ifstream inFile(inFileName);
    
    // Read Input
    if (!inFile) {
        cout << "fail open file=" << inFileName << endl;
        return;
    }
    cout << ">> blif2vmt:: inFile=" << inFileName << " outFile=" << outFileName << endl;
    
    int sCnt = -1, stateVarNum = 0;
    vector<vector<string> > iotCubeList (3,vector<string>());
    vector<string> tokenList;
    while (getline(inFile,line)) {
        if (line.empty() || line[0] == '#') continue;
        tokenList.clear();
        str2tokens(line,tokenList);
        if (tokenList[0] == ".inputs" || tokenList[0] == ".names") {
            // in blif : x=input s=current state n=next state
            if ( *(tokenList.rbegin()) == "\\") {
                tokenList.pop_back();
                getline(inFile,line);
                vector<string> extraTokens;
                str2tokens(line,extraTokens);
                for (size_t i = 0, size = extraTokens.size(); i < size; ++i)
                    tokenList.push_back(extraTokens[i]);
            }
            #ifndef AUTMGR_NDEBUG
                cout << "[AutMgr::blif2vmt] tokenList:";
                for (size_t i = 0, size = tokenList.size(); i < size; ++i)
                    cout << " " << tokenList[i];
                cout << endl;
            #endif
            if (tokenList[0] == ".inputs") {
                for (size_t i = 1, size = tokenList.size(); i < size; ++i)
                    if (tokenList[i][0] == 's') ++stateVarNum;
            }
            else {
                ++sCnt;
            }
        }
        else if (line[0] == '0' || line[0] == '1' || line[0] == '-') {
            #ifndef AUTMGR_NDEBUG
                cout << "[AutMgr::blif2vmt] tokenList: " << tokenList[0] << " " << tokenList[1] << endl;
            #endif
            iotCubeList[sCnt].push_back(tokenList[0]);
        }
        else {
            //cout << "[WARNING] ignored input=" << line << endl;
        }
    }
    inFile.close();
    
    int dCnt = -1;
    Aut* a = new Aut();
    a->addStateVar(stateVarNum);

    VmtNodeList pList0;
    VmtNodeList pList1;
    for (size_t i = 0; i < stateVarNum; ++i) pList0.push_back(Aut::piList[STATE][i]);
    for (size_t i = 0; i < Aut::inputBitNum; ++i) pList1.push_back(Aut::piList[INPUT][i]);
    for (size_t i = 0; i < stateVarNum; ++i) pList1.push_back(Aut::piList[STATE][i]);
    for (size_t i = 0; i < stateVarNum; ++i) pList1.push_back(Aut::piList[STATE_N][i]);

    for (size_t i = 0, size = pList0.size(); i < size; ++i) cout << pList0[i]->_name << endl;
    for (size_t i = 0, size = pList1.size(); i < size; ++i) cout << pList1[i]->_name << endl;

    // Initial State
    assert( (iotCubeList[0].size() == 1) );
    parseCubeList(a,"i",iotCubeList[0],pList0,dCnt);
    // Transition Relation
    parseCubeList(a,"t",iotCubeList[2],pList1,dCnt);
    // Accepting States
    if (iotCubeList[1].size() == 1) {
        bool isDC = 1;
        for (size_t i = 0, size = iotCubeList[1][0].size(); i < size; ++i)
            if (iotCubeList[1][0][i] != '-') {
                isDC = 0;
                break;
            }
        if (isDC) {
            iotCubeList[1].clear();
            size_t max = 1;
            for (size_t i = 0; i < stateVarNum; ++i) max *= 2;
            for (size_t i = 0; i < max; ++i)
                iotCubeList[1].push_back(Uint2BitString(i,stateVarNum));
        }
    }
    parseCubeList(a,"o",iotCubeList[1],pList0,dCnt);
    a->defineFun( "I", "(! i :init true)"             , a->_itoList);
    a->defineFun( "T", "(! t :trans true)"            , a->_itoList);
    a->defineFun( "O", "(! (not o) :invar-property 0)", a->_itoList);
    //Write Output
    //a->print();
    a->renameDef();
    a->write(outFileName);
}

void AutMgr::parseCubeList(Aut* a, const string& name, const vector<string>& cubeList, const VmtNodeList& pList, int& dCnt)
{
    #ifndef AUTMGR_NDEBUG
        cout << "[AutMgr::parseCubeList] name=" << name << " len(cubeList)=" << cubeList.size() << " dCnt=" << dCnt << endl;
    #endif
    if (cubeList.empty()) {
        // TODO : make sure no accpeting states or no transition condition are correctly described
        a->defineFun( name, "false", a->_imdList);
        return;
    }
    assert( (pList.size() == cubeList[0].size()) );
    if (cubeList.size() == 1) {
        string body = cube2vmt(cubeList[0], pList);
        #ifndef AUTMGR_NDEBUG
            cout << "[AutMgr::parseCubeList] dCnt=" << dCnt << " body=" << body << endl;
        #endif
        a->defineFun( name, body, a->_imdList);
    }
    else {
        string ors = "(or";
        for (size_t i = 0, size = cubeList.size(); i < size; ++i) {
            string body = cube2vmt(cubeList[i], pList);
            #ifndef AUTMGR_NDEBUG
                cout << "[AutMgr::parseCubeList] dCnt=" << dCnt << " body=" << body << endl;
            #endif
            a->defineFun( "d" + itos(++dCnt), body, a->_imdList);
            ors += " d" + itos(dCnt);
        }
        ors += ")";
        a->defineFun( name, ors, a->_imdList);
    }
}

string AutMgr::cube2vmt(const string& cube, const VmtNodeList& pList)
{
    size_t varCnt = 0;
    string s;
    for (size_t i = 0, size = cube.size(); i < size; ++i) {
        if (cube[i] == '1') {
            ++varCnt;
            s += " " + pList[i]->_name;
        }
        else if (cube[i] == '0') {
            ++varCnt;
            s += " (not " + pList[i]->_name + ")";
        }
    }
    if (varCnt > 1)
        s = "(and" + s + ")";
    else
        s = s.substr(1);
    return s;
}

void AutMgr::readCmdFile(const char* fileName)
{
    string line;
    ifstream file(fileName);
    
    // Read Input
    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    #ifdef UTF16_ENCODE
        string sigma_star_dir = "special_regex/sigma_star_16.vmt";
        string alphabet_dir   = "special_regex/alphabet_16.vmt";
        string epsilon_dir    = "special_regex/epsilon_16.vmt";
    #else
        string sigma_star_dir = "special_regex/sigma_star_7.vmt";
        string alphabet_dir   = "special_regex/alphabet_7.vmt";
        string epsilon_dir    = "special_regex/epsilon_7.vmt";
    #endif
    string path(fileName);
    path = path.substr(0,path.find_last_of("/")) + "/";
    cout << ">> readCmd::dgDir = " << path << endl;
    size_t section = 0;
    Aut* cur = 0;
    vector<string> tokenList, declList, predList;
    while(getline(file,line)) {
        if (line == ";")  { ++section; continue; }
        if (section == 0) { continue; }
        else if (section == 1) { declList.push_back(line); continue; }
        else if (section == 2) { predList.push_back(line); continue; }
        
        tokenList.clear();
        str2tokens(line,tokenList);
        if (tokenList[0][0] == ';' || tokenList[0] == ";") continue;
        for (size_t i = 0, size = tokenList.size(); i < size; ++i)
            cout << tokenList[i] << " ";
        cout << endl;
        if (tokenList[0] == "intersect"       || tokenList[0] == "notcontains" ||
            tokenList[0] == "notprefixof_smt" || tokenList[0] == "notsuffixof_smt") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            cur     = new Aut( a1, a2, INTERSECT );
        }
        else if (tokenList[0] == "contains") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            Aut* a3 = new Aut( sigma_star_dir );
            Aut* a4 = new Aut( sigma_star_dir );
            Aut* a5 = new Aut( a3, a2, CONCATE );
            Aut* a6 = new Aut( a5, a4, CONCATE );
            cur     = new Aut( a1, a6, INTERSECT );
        }
        else if (tokenList[0] == "prefixof_smt") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            Aut* a3 = new Aut( sigma_star_dir );
            Aut* a4 = new Aut( a1, a3, CONCATE );
            cur     = new Aut( a4, a2, INTERSECT);
        }
        else if (tokenList[0] == "suffixof_smt"){
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            Aut* a3 = new Aut( sigma_star_dir );
            Aut* a4 = new Aut( a3, a1, CONCATE );
            cur     = new Aut( a4, a2, INTERSECT);
        }
        else if (tokenList[0] == "union") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            cur     = new Aut( a1, a2, UNION );
        }
        else if (tokenList[0] == "concate") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            cur     = new Aut( a1, a2, CONCATE );
        }
        else if (tokenList[0] == "trklen") {
            cur = new Aut( path + tokenList[1] + ".vmt", tokenList[2], TRKLEN );
        }
        else if (tokenList[0] == "trkstr") {
            cur = new Aut( path + tokenList[1] + ".vmt", tokenList[2], TRKSTR );
        }
        else if (tokenList[0] == "replace") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            Aut* a3 = new Aut( path + tokenList[3] + ".vmt" );
            Aut* ah = new Aut( path + tokenList[4] + ".vmt" );
            size_t alpha = a1->mark();
            Aut* a4 = new Aut( a2, ah, REPLACE_A4 );
            Aut* a5 = new Aut( a1, a4, INTERSECT );
            cur     = new Aut( a5, a3, alpha, REPLACE);
        }
        else if (tokenList[0] == "substr") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" , tokenList[3] , PREFIX );
            a1->write( path + tokenList[1] + "_prefix.pfx" );
            
            Aut* a2 = new Aut( epsilon_dir );
            Aut* a3 = new Aut( path + tokenList[1] + "_prefix.pfx" );
            Aut* a4 = new Aut( a2, a3, CONCATE );
            //a4->addpred( path + "pred" );
            //a4->write  ( path + "pfx.vmt" );
            //a4->isempty( path + "pfx.blif" );
            
            cur = new Aut( path + tokenList[1] + "_prefix.pfx" , tokenList[2], SUFFIX );
        }
        else if (tokenList[0] == "trkidx") {
            Aut* a1  = new Aut( path + tokenList[3] + ".vmt" );
            Aut* a2  = new Aut( alphabet_dir );
            //Aut* a3 = new Aut();
            //a3->concate(a1,a2);
            Aut* a3  = new Aut( a1, a2, CONCATE );
            Aut* a4  = new Aut( sigma_star_dir, tokenList[4], TRKLEN );
            //a4->addlen(tokenList[4]);
            Aut* a5  = new Aut( path + tokenList[2] + ".vmt" );
            //Aut* a6 = new Aut();
            //a6->concate(a4,a5);
            Aut* a6  = new Aut( a4, a5, CONCATE );
            //Aut* a7 = new Aut();
            //a7->intersect(a6,a3);
            Aut* a7  = new Aut( a6, a3, INTERSECT );
            Aut* a8  = new Aut( sigma_star_dir, tokenList[5], TRKLEN );
            //a8->addlen(tokenList[5]);
            //Aut* a9 = new Aut();
            //a9->concate(a8,a7);
            Aut* a9  = new Aut( a8, a7, CONCATE );
            Aut* a10 = new Aut( path + tokenList[1] + ".vmt" );
            cur      = new Aut( a10, a9, INTERSECT );
        }
        else if (tokenList[0] == "read") {
            cur = new Aut( path + tokenList[1] + ".vmt" );
        }
        else if (tokenList[0] == "isempty") {
            Aut* a1 = new Aut( epsilon_dir );
            Aut* a2 = new Aut( path + "before_addpred.vmt" );
            cur     = new Aut( a1, a2, CONCATE );
            cur->addpred( declList, predList );
            cur->write  ( path + "fin.vmt" );
            cur->isempty( path + "sink.blif" );
            
            string abcFileName = path + "sink.abc";
            ofstream abcFile( abcFileName.c_str() );
            
            // Modify ABC Operation here
            abcFile << "read " << path << "sink.blif\n"
                    << "strash\n"
                    << "pdr";
            abcFile.close();
        }
        else if (tokenList[0] == "addpred") {
            cur->write  ( path + "before_addpred.vmt" );
            cur->addpred( declList, predList );
        }
        else if (tokenList[0] == "write") {
            cur->write( path + tokenList[1] + ".vmt" );
        }
        else {
            cout << "[AutMgr::readCmdFile] ERROR: invalid command=" << tokenList[0] << endl;
            return;
        }
    }
}

}
