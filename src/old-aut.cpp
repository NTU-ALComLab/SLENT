#include "autMgr.h"

///////////////////////////////// Global Variable /////////////////////////////

extern aut::AutMgr* autmgr;
static size_t& gflag = autmgr->getGFlag();

namespace aut {

///////////////////////////////// Static Member ///////////////////////////////

//string   Aut::BLIFIndent  = "";
string   Aut::BLIFIndent  = " \\\n" + string(7,' ');
size_t   Aut::inputBitNum = INPUT_ENCODE_BIT_NUM + 1;
STRList  Aut::piSymbolS   = Aut::initPISymbolS();
CHRList  Aut::piSymbolC   = Aut::initPISymbolC();
VarList  Aut::piList      = Aut::initPIList();
VmtNode* Aut::const0      = Aut::initConst(CONST0);
VmtNode* Aut::const1      = Aut::initConst(CONST1);
VmtNode* Aut::epsilon     = Aut::initSpecialAlphabet(EPSILON);
VmtNode* Aut::leftAngle   = Aut::initSpecialAlphabet(LEFT_ANGLE);
VmtNode* Aut::rightAngle  = Aut::initSpecialAlphabet(RIGHT_ANGLE);


void Aut::test() {
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0, size = piList[LEN].size(); j < size; ++j)
            cout << piList[LEN][j]->_name << " " << piList[LEN][j] << endl;
        cout << "expandCnt " << i << endl;
        expandPIList(LEN,piList[LEN].size());
    }
}

STRList Aut::initPISymbolS()
{
    STRList v; v.resize(PI_NUM);
    v[INPUT ] = "x"; v[EXIST  ] = "y";
    v[STATE ] = "s"; v[STATE_N] = "s";
    v[LEN   ] = "n"; v[LEN_N  ] = "n";
    v[PREDBV] = "J"; v[PREDIV ] = "K";
    return v;
}

CHRList Aut::initPISymbolC()
{
    CHRList v; v.resize(PI_NUM);
    v[INPUT ] = 'x'; v[EXIST  ] = 'y';
    v[STATE ] = 's'; v[STATE_N] = 's';
    v[LEN   ] = 'n'; v[LEN_N  ] = 'n';
    v[PREDBV] = 'J'; v[PREDIV ] = 'K';
    return v;
}

VmtNode* Aut::initConst(const VmtType& type)
{
    assert( (type == CONST0 || type == CONST1) );
    if (type == CONST0) return new VmtNode("false");
    else                return new VmtNode("true" );
}

VmtNode* Aut::initSpecialAlphabet(const AType& type)
{
    string name;
    size_t encode;
    switch (type) {
        case EPSILON     : name   = "epsilon";
                           encode = EPSILON_ENCODE;
                           break;
        case LEFT_ANGLE  : name   = "leftAngle";
                           encode = LEFT_ANGLE_BRACKET_ENCODE;
                           break;
        case RIGHT_ANGLE : name   = "rightAngle";
                           encode = RIGHT_ANGLE_BRACKET_ENCODE;
                           break;
        default          : break;
    }
    #ifndef AUT_NDEBUG
        cout << "Aut::initSpecialAllphabet::" << name << endl;
    #endif
    VmtNode* n = new VmtNode(name);
    n->addChild(new VmtNode("and"));

    // special alphabets are encoded with x0
    const VmtNodeList& input = piList[INPUT];

    n->_children[0]->addChild(input[0]);
    n->_paramList[INPUT].push_back(input[0]);

    string bitstr = Uint2BitString(encode,INPUT_ENCODE_BIT_NUM);
    for (size_t i = 0; i < INPUT_ENCODE_BIT_NUM; ++i) {
        if (bitstr[i] == '0') {
            VmtNode* notNode = new VmtNode("not");
            notNode->addChild(input[i+1]);
            n->_children[0]->addChild(notNode);
        }
        else {
            assert( (bitstr[i] == '1') );
            n->_children[0]->addChild(input[i+1]);
        }
        n->_paramList[INPUT].push_back(input[i+1]);
    }
    return n;
}

VarList Aut::initPIList() 
{
    #ifndef AUT_NDEBUG
        cout << "[Aut::initPIList]";
    #endif
    VarList v(PI_NUM,VmtNodeList());
    for (size_t i = 0; i < inputBitNum; ++i) {
        v[INPUT].push_back( new VmtNode( piSymbolS[INPUT] + itos(i) , i ) );
        #ifndef AUT_NDEBUG
            cout << " " << v[INPUT][i]->_name;
        #endif
    }
    #ifndef AUT_NDEBUG
        cout << endl;
    #endif
    return v;
}

//TODO
void Aut::printStaticDataMember() 
{
    for (size_t i = 0; i < PI_NUM; ++i) {
        cout << "[Aut::printStaticDataMember] " << getTypeStr(i) << endl;
        for (size_t j = 0, size = piList[i].size(); j < size; ++j) 
            piList[i][j]->print(0);
    }
    cout << "[Aut::printStaticDataMember] const0\n";
    const0->print(0);
    cout << "[Aut::printStaticDataMember] const1\n";
    const1->print(0);
    cout << "[Aut::printStaticDataMember] epsilon\n";
    epsilon->print(0);
    cout << "[Aut::printStaticDataMember] leftAngle\n";
    leftAngle->print(0);
    cout << "[Aut::printStaticDataMember] rightAngle\n";
    rightAngle->print(0);
}

bool Aut::isPI(const VmtType& type)
{
    return (type >= 0 && type < PI_NUM);
}

bool Aut::isCurPI(const VmtType& type)
{
    return (isPI(type) && type != STATE_N && type != LEN_N);
}

bool Aut::isLEAF(const VmtType& type)
{
    return (type < 30);
}

bool Aut::isCONST(const VmtType& type)
{
    return (type == CONST0 || type == CONST1);
}

bool Aut::isINT(const VmtType& type)
{
    return (type == LEN || type == LEN_N || type == PREDIV || type == NUM);
}

bool Aut::isPREDINT(const VmtType& type)
{
    return (type == LEN || type == PREDIV || type == NUM);
}

bool Aut::isPARAMINT(const VmtType& type)
{
    return (type == LEN || type == LEN_N || type == PREDIV);
}

bool Aut::isPARAMBOOL(const VmtType& type)
{
    return (type == INPUT || type == EXIST || type == STATE || type == STATE_N || type == PREDBV);
}

bool Aut::isRETBOOL(const VmtType& type)
{
    return (type == PARAM || type == MODULE || type == NOT || type == AND   || type == OR ||
            type == LT    || type == LTOEQ  || type == EQ  || type == MTOEQ || type == MT  );
}

bool Aut::isPREDRETBOOL(const VmtType& type)
{
    return (type == PREDBV || type == CONST0 || type == CONST1 || type == NOT   || type == AND || type == OR ||
            type == LT     || type == LTOEQ  || type == EQ     || type == MTOEQ || type == MT                 );
}

bool Aut::isPREDRETINT(const VmtType& type)
{
    return (type == LEN || type == PREDIV || type == NUM || type == NEG || type == PLUS || type == MINUS);
}

bool Aut::isIMD(const VmtType& type)
{
    return (type >= 30);
}

bool Aut::isOP(const VmtType& type)
{
    return (isIMD(type) && type != EXCM && type != MODULE);
}

bool Aut::isARITH(const VmtType& type)
{
    return (type == PLUS || type == MINUS || type == NEG);
}

bool Aut::isSO(const VmtType& type)
{
    return (isRETBOOL(type) || isPARAMBOOL(type) || isCONST(type));
}

bool Aut::isMO(const VmtType& type)
{
    return (isPARAMINT(type) || type == NUM || isARITH(type));
}

bool Aut::isPISymbol(const char& c)
{
    return (c == piSymbolC[INPUT]  || c == piSymbolC[EXIST] ||
            c == piSymbolC[STATE]  || c == piSymbolC[LEN  ] ||
            c == piSymbolC[PREDBV] || c == piSymbolC[PREDIV]  );
}

bool Aut::isSpecialString(const string& name)
{
    return (name == ":trans" || name == ":init" || name == ":invar-property");
}

string Aut::getTypeStr(const size_t& idx) 
{
    return getTypeStr(static_cast<VmtType>(idx));
}

string Aut::getTypeStr(const VmtType& type) 
{
    switch (type) {
        // LEAF
        // PI
        case INPUT   : return "INPUT"  ;
        case EXIST   : return "EXIST"  ;
        case STATE   : return "STATE"  ;
        case LEN     : return "LEN"    ;
        case STATE_N : return "STATE_N";
        case LEN_N   : return "LEN_N"  ;
        case PREDBV  : return "PREDBV" ;
        case PREDIV  : return "PREDIV" ;
        // CONST
        case NUM     : return "NUM"    ;
        case CONST0  : return "CONST0" ;
        case CONST1  : return "CONST1" ;
        case PARAM   : return "PARAM"  ;
        case SPECIAL : return "SPECIAL";
        // IMD        
        // OP
        case NOT     : return "NOT"    ;
        case NEG     : return "NEG"    ;
        case AND     : return "AND"    ;
        case OR      : return "OR"     ;
        case PLUS    : return "PLUS"   ;
        case MINUS   : return "MINUS"  ;
        case LT      : return "LT"     ;
        case LTOEQ   : return "LTOEQ"  ;
        case EQ      : return "EQ"     ;
        case MTOEQ   : return "MTOEQ"  ;
        case MT      : return "MT"     ;
        // EXCM & MODULE
        case EXCM    : return "EXCM"   ;
        case MODULE  : return "MODULE" ;
        default      : return "INVALID";
    }
}

VmtType Aut::getPITypeByName(const string& name) 
{
    if      (name[0] == piSymbolC[INPUT] ) return INPUT;
    else if (name[0] == piSymbolC[EXIST] ) return EXIST;
    else if (name[0] == piSymbolC[PREDBV]) return PREDBV;
    else if (name[0] == piSymbolC[PREDIV]) return PREDIV;
    else if (name[0] == piSymbolC[STATE] ) 
    {
    if ( *(name.rbegin()) == 't' )         return STATE_N;
    else                                   return STATE;
    }
    else 
    {
    assert( (name[0] == piSymbolC[LEN]) );
    if ( *(name.rbegin()) == 't' )         return LEN_N;
    else                                   return LEN;
    }
}

void Aut::expandPIList(const VmtType& type,const size_t& maxIdx)
{
    assert( (isCurPI(type) && type != INPUT) );
    size_t origin = piList[type].size();
    size_t nsize  = maxIdx + 1;
    if ( nsize <= origin ) return;

    piList[type].resize(nsize);
    for (size_t i = origin; i < nsize; ++i)
        piList[type][i] = new VmtNode( piSymbolS[type] + itos(i) , i );

    if (type == STATE) {
        piList[STATE_N].resize(nsize);
        for (size_t i = origin; i < nsize; ++i) {
            piList[STATE_N][i] = new VmtNode( piSymbolS[STATE_N] + itos(i) + ".next" , i );
        }
    }
    else if (type == LEN) {
        piList[LEN_N].resize(nsize);
        for (size_t i = origin; i < nsize; ++i)
            piList[LEN_N][i] = new VmtNode( piSymbolS[LEN_N] + itos(i) + ".next" , i );
    }
}

void Aut::vmtTokenize(const string& s,vector<string>& paramList, vector<string>& tokenList)
{
    size_t size = s.size();
    size_t state = 0;
    for (size_t i = 0; i < size; ++i) {
        if (state == 0) {
            if (s[i] != ' ') continue;
            else {
                state = 1;
            }
        }
        else if (state == 1) {
            size_t j = i;
            while (s[j] != ' ') ++j;
            tokenList.push_back(s.substr(i,j-i));
            assert(s[++j] == '(');
            int dCnt = 1;
            while(dCnt != 0) {
                ++j;
                if (s[j] == '(') {
                    size_t k = ++j;
                    while (s[j] != ' ') ++j;
                    paramList.push_back(s.substr(k,j-k));
                    while (s[j] != ')') ++j;
                }
                else if (s[j] == ')') --dCnt;
            }
            j += 2;
            if (s[j] == 'I') tokenList.push_back("");
            else if (s[j] == 'B') {
                j += 4;
                if (s[j] == ')') tokenList.push_back("");
                else {
                    assert((s[j] == ' '));
                    size_t k = ++j;
                    if (s[j] == '(') {
                        dCnt = 1;
                        while(dCnt != 0) {
                            ++j;
                            if (s[j] == ')') --dCnt;
                            else if(s[j] == '(') ++dCnt;
                        }
                        tokenList.push_back(s.substr(k,j+1-k));
                    }
                    else {
                        while (s[j] != ')') ++j;
                        tokenList.push_back(s.substr(k,j-k));
                    }
                }
            }
            /*
            #ifndef AUT_NDEBUG
                cout << "[Aut::vmtTokenize] "<<tokenList[0] << "|" << tokenList[1] << "|" << endl;
                for (size_t i = 0, s = paramList.size(); i < s; ++i)
                    cout << paramList[i] << " ";
                cout << endl;
            #endif
            */
            return;
        }
    }
}

//TODO::refactor this function
VmtNode* Aut::buildVmtNode(const string& s, size_t bpos, size_t epos, Str2VmtNodeMap& vmap)
{
    #ifndef AUT_NDEBUG
        cout << "[Aut::buildVmtNode] line = " << s.substr(bpos,epos-bpos) << endl;
    #endif
    if (s[bpos] != '(') {
        string name = s.substr(bpos,epos-bpos);
        Str2VmtNodeMap::iterator it = vmap.find(name);
        if (it != vmap.end()) { return it->second;}
        else                  return new VmtNode(name);
    }
    else {
        size_t i = bpos + 1;
        while (s[i] != ' ') ++i;
        string root = s.substr(bpos+1,i-(bpos+1));
        #ifndef AUT_NDEBUG
            cout << "[Aut::buildVmtNode] root = " << root << endl;
        #endif
        if ( !isReservedString(root) ) {
            Str2VmtNodeMap::iterator it = vmap.find(root);
            assert((it != vmap.end())); // define-fun in topological order
            VmtNode* source = it->second;
            VmtNode* pNode  = new VmtNode(root,source);
            while(s[i] != ')') {
                size_t j = ++i;
                while (s[i] != ' ' && s[i] != ')') ++i;
                string param = s.substr(j,i-j);
                Str2VmtNodeMap::iterator jt = vmap.find(param);
                assert( (jt != vmap.end()) );
                VmtType type = jt->second->_type;
                assert( (isPI(type) || type == CONST0 || type == CONST1) );
                if  (type == CONST0 || type == CONST1) type = INPUT;
                pNode->_paramList[type].push_back(jt->second);
            }
            if (pNode->haveSameParam(source)) {
                //cout << "[Aut::buildVmtNode] " << root << " same" << endl;
                return source;
            }
            else {
                cout << "[Aut::buildVmtNode] " << root << " PARAM" << endl;
                return pNode;
            }
        }
        VmtNode* newNode = new VmtNode(root);
        for (++i; i < epos; ++i) {
            size_t j = i;
            if (s[i] == '(') {
                int diff = 0;
                while (diff != 1) {
                    ++j;
                    if (s[j] == '(') --diff;
                    else if (s[j] == ')') ++diff;
                }
                newNode->addChild(buildVmtNode(s,i,j+1,vmap));
                i = j+1;
            }
            else {
                while (s[j] != ' ' && j != epos-1) ++j;
                newNode->addChild(buildVmtNode(s,i,j,vmap));
                i = j;
            }
        }
        return newNode;
    }
}

// not include div
bool Aut::isReservedString(const string& s)
{
    if (s == "and" || s == "or" || s == "not" || s == "!"  || s == "="  || 
        s == "+"   || s == "-"  || s == ">="  || s == "<=" || s == "<"  || s == ">")
        return 1;
    else
        return 0;
}

void Aut::check(Aut* a)
{
    assert( (a->_piList[STATE].size()   == a->_stateVarNum) );
    assert( (a->_piList[STATE_N].size() == a->_stateVarNum) );
    assert( (a->_piList[LEN].size()     == a->_piList[LEN_N].size()) );
    for (size_t i = 0, size = a->_piList[STATE].size(); i < size; ++i)
        assert( (a->_piList[STATE][i]->_idx == a->_piList[STATE_N][i]->_idx) );
    for (size_t i = 0, size = a->_piList[LEN].size(); i < size; ++i)
        assert( (a->_piList[LEN][i]->_idx == a->_piList[LEN_N][i]->_idx) );
}

void Aut::check(Aut* a, const size_t& lvarIdx)
{
    check(a);
    for (size_t i = 0, size = a->_piList[LEN].size(); i < size; ++i)
        assert( (lvarIdx != a->_piList[LEN][i]->_idx) );
    for (size_t i = 0, size = a->_piList[LEN_N].size(); i < size; ++i)
        assert( (lvarIdx != a->_piList[LEN_N][i]->_idx) );
}

void Aut::check(Aut* a1, Aut* a2)
{
    check(a1);
    check(a2);
    for (size_t i = 0, size1 = a1->_piList[LEN].size(); i < size1; ++i)
        for (size_t j = 0, size2 = a2->_piList[LEN].size(); j < size2; ++j) {
            assert( (a1->_piList[LEN][i]->_idx != a2->_piList[LEN][j]->_idx) );
        }
}

void Aut::check(Aut* a1, Aut* a2, const size_t& lvarIdx)
{
    check(a1,a2);
    check(a1,lvarIdx);
    check(a2,lvarIdx);
}

///////////////////////////// Non-Static Member ///////////////////////////////

void Aut::init(const string& fileName)
{
    size_t bpos = fileName.find_last_of('/') + 1;
    size_t epos = fileName.find_last_of('.');
    _name = fileName.substr(bpos,epos-bpos);
    _stateVarNum = 0;
    _piList.assign(PI_NUM,VmtNodeList());
    initVMap();
}

void Aut::initVMap()
{
    _vmap.clear();
    _vmap.insert(Str2VmtNode("false"     ,const0    ));
    _vmap.insert(Str2VmtNode("true"      ,const1    ));
    _vmap.insert(Str2VmtNode("epsilon"   ,epsilon   ));
    _vmap.insert(Str2VmtNode("leftAngle" ,leftAngle ));
    _vmap.insert(Str2VmtNode("rightAngle",rightAngle));
    for (size_t i = 0; i < inputBitNum; ++i)
        _vmap.insert(Str2VmtNode(piList[INPUT][i]->_name,piList[INPUT][i]));
    _piList[INPUT].clear();
    for (size_t i = 0; i < inputBitNum; ++i)
        _piList[INPUT].push_back(piList[INPUT][i]);
}

void Aut::buildVMap(const VmtType& type)
{
    assert( (isPI(type)) );
    for (size_t i = 0, size = _piList[type].size(); i < size; ++i)
        _vmap.insert(Str2VmtNode(_piList[type][i]->_name,_piList[type][i]));
}

void Aut::buildVMap(const VmtNodeList& moduleList)
{
    for (size_t i = 0, size = moduleList.size(); i < size; ++i) {
        assert( (moduleList[i]->_type == MODULE) );
        _vmap.insert(Str2VmtNode(moduleList[i]->_name,moduleList[i]));
    }
}

void Aut::assignGlobalPIList(const VmtType& type)
{
    assert( (type == EXIST || type == PREDBV || type == PREDIV) );
    VmtNodeList& global =  piList[type];
    VmtNodeList& local  = _piList[type];
    const size_t gsize  = global.size();
    const size_t lsize  = local.size();
    global.resize( gsize + lsize );
    for (size_t i = 0; i < lsize; ++i) {
        // rename and assign
        size_t idx      = i + gsize;
        local[i]->_idx  = idx;
        local[i]->_name = piSymbolS[type] + itos(idx);
        global[idx]     = local[i];
    }
}

void Aut::clearParam()
{
    ++gflag;
    #ifndef AUT_NDEBUG
        cout << "[Aut::clearParam] ++gflag=" << gflag << endl;
    #endif
    for (size_t i = 0, size = _itoList.size(); i < size; ++i)
        _itoList[i]->clearParam(0);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            //cout << "[Aut::clearParam] unvisited node=" << _imdList[i]->_name << " " << _imdList[i] << " type=" << _imdList[i]->getTypeStr() << endl;
            _imdList[i]->clearParam(0);
        }
    for (size_t i = 0, size = _predList.size(); i < size; ++i)
        if (_predList[i]->_flag != gflag) {
            _predList[i]->clearParam(0);
        }
}

void Aut::buildParam()
{
    ++gflag;
    #ifndef AUT_NDEBUG
        cout << "[Aut::buildParam] ++gflag=" << gflag << endl;
    #endif
    for (size_t i = 0, size = _itoList.size(); i < size; ++i)
        _itoList[i]->buildParam(0);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            //cout << "[Aut::buildParam] unvisited node=" << _imdList[i]->_name << " " << _imdList[i] << " type=" << _imdList[i]->getTypeStr() << endl;
            _imdList[i]->buildParam(0);
        }
    for (size_t i = 0, size = _predList.size(); i < size; ++i)
        if (_predList[i]->_flag != gflag) {
            _predList[i]->buildParam(0);
        }
}

void Aut::collectPARAM()
{
    ++gflag;
    for (size_t i = 0, size = _itoList.size(); i < size; ++i)
        _itoList[i]->collectPARAM(_PARAMList);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        _imdList[i]->collectPARAM(_PARAMList);
    for (size_t i = 0, size = _predList.size(); i < size; ++i)
        _predList[i]->collectPARAM(_PARAMList);
}

void Aut::spotNEG()
{
    ++gflag;
    // NEGs only appear in predicates
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        cout <<  "Aut::spotneg\n";
        _predList[i]->spotNEG();
    }
}

void Aut::renameDef()
{
    #ifndef AUT_PARAM_NDEBUG
        cout << "[Aut::renameDef]\n";
        vector<string> PARAMname;
        vector<string> PARAMsourcebefore;
        for (size_t i = 0, size = _PARAMList.size(); i < size; ++i) {
            PARAMname.push_back(_PARAMList[i]->_name);
            PARAMsourcebefore.push_back(_PARAMList[i]->_source->_name);
        }
    #endif
    int dCnt = -1;
    for (size_t i = 0, size = _imdList.size(); i < size; ++i) {
        assert( (_imdList[i] != epsilon && _imdList[i] != leftAngle && _imdList[i] != rightAngle) );
        _imdList[i]->_name = "d" + itos(++dCnt);
    }
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        _predList[i]->_name = "d" + itos(++dCnt);
    }
    clearParam();
    buildParam();
    //print();
    //_eUsed = (   epsilon->_flag == gflag) ? 1:0;
    //_lUsed = ( leftAngle->_flag == gflag) ? 1:0;
    //_rUsed = (rightAngle->_flag == gflag) ? 1:0;
    #ifndef AUT_PARAM_NDEBUG
        for (size_t i = 0, size = _PARAMList.size(); i < size; ++i) {
            assert( (PARAMname[i] == _PARAMList[i]->_name) );
            cout << PARAMname[i] << " " << _PARAMList[i] << " " << PARAMsourcebefore[i] << " -> " << _PARAMList[i]->_source->_name << endl;
        }
    #endif
}

void Aut::shiftStateVar(const size_t& delta)
{
    expandPIList( STATE , _stateVarNum + delta - 1 );
    _piList[STATE  ].clear();
    _piList[STATE_N].clear();
    for (size_t i = delta; i < _stateVarNum + delta; ++i) {
        _piList[STATE  ].push_back(piList[STATE  ][i]);
        _piList[STATE_N].push_back(piList[STATE_N][i]);
    }
    ++gflag;
    for (size_t i = 0, size = _itoList.size(); i < size; ++i)
        _itoList[i]->shiftStateVar(delta);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            //cout << "[Aut::shiftStateVar] unvisited node=" << _imdList[i]->_name << " " << _imdList[i] << " type=" << _imdList[i]->getTypeStr() << endl;
            _imdList[i]->shiftStateVar(delta);
        }
    for (size_t i = 0, size = _predList.size(); i < size; ++i)
        if (_predList[i]->_flag != gflag) {
            _predList[i]->shiftStateVar(delta);
        }
    check(this);
    clearParam();
    buildParam();
}

void Aut::defineFun(const string& fname, const string& body, VmtNodeList& list)
{
    #ifndef AUT_NDEBUG
        cout << "[Aut::defineFun] name=" << fname << " body=" << body << endl;
    #endif
    VmtNode* n = new VmtNode(fname);
    _vmap.insert(Str2VmtNode(fname,n));
    n->addChild(buildVmtNode(body,0,body.size(),_vmap));
    list.push_back(n);
}

void Aut::defineFun(const string& fname, const string& body, VmtNodeList& list, Str2VmtNodeMap& vmap)
{
    VmtNode* n = new VmtNode(fname);
    vmap.insert(Str2VmtNode(fname,n));
    n->addChild(buildVmtNode(body,0,body.size(),vmap));
    list.push_back(n);
}

void Aut::defineFun(const string& fname, const VmtType& type,const string& body, VmtNodeList& list, Str2VmtNodeMap& vmap)
{
    VmtNode* n = new VmtNode(fname,type);
    vmap.insert(Str2VmtNode(fname,n));
    n->addChild(buildVmtNode(body,0,body.size(),vmap));
    list.push_back(n);
}

void Aut::defineFun(const string& fname, const string& body, VmtNodeList& list, void (Aut::*set) (VmtNode*))
{
    VmtNode* n = new VmtNode(fname);
    _vmap.insert(Str2VmtNode(fname,n));
    n->addChild(buildVmtNode(body,0,body.size(),_vmap));
    list.push_back(n);
    (this->*set)(n);
}

void Aut::addParamNode(const string& name, VmtNode* source)
{
    assert( (source->_paramList[STATE_N].empty()) );
    VmtNode* n = new VmtNode(name,source);
    #ifndef AUT_PARAM_NDEBUG
        _PARAMList.push_back(n);
    #endif
    _vmap.insert(Str2VmtNode(name,n));
    // copy paramList include INPUT,EXIST,STATE,LEN,STATE_N,LEN_N,PREDBV,PREDIV
    n->_paramList = source->_paramList;
    // clear STATE
    n->_paramList[STATE].clear();
    // move STATE to STATE_N
    for (size_t i = 0, size = source->_paramList[STATE].size(); i < size; ++i) {
        size_t idx = source->_paramList[STATE][i]->_idx;
        n->_paramList[STATE_N].push_back(piList[STATE_N][idx]);
    }
}

void Aut::addParamNode(const string& name, VmtNode* source, const size_t& encode, const bool& isExistential)
{
    VmtNode* n = new VmtNode(name,source);
    #ifndef AUT_PARAM_NDEBUG
        _PARAMList.push_back(n);
    #endif
    _vmap.insert(Str2VmtNode(name,n));
    // record INPUT
    VmtNodeList xList = source->_paramList[INPUT];
    // copy paramList include INPUT,EXIST,STATE,LEN,STATE_N,LEN_N,PREDBV,PREDIV
    n->_paramList = source->_paramList;
    // clear INPUT
    n->_paramList[INPUT].clear();

    if (isExistential) {
        // push EXIST to beginning
        VmtNodeList eList;
        const size_t& evbpos = encode;
        for (size_t i = evbpos, size = evbpos + xList.size(); i < size; ++i)
            eList.push_back(piList[EXIST][i]);
        for (size_t i = 0, size = n->_paramList[EXIST].size(); i < size; ++i)
            eList.push_back(n->_paramList[EXIST][i]);
        // substitute paramList[1]
        n->_paramList[EXIST] = eList;
    }
    else {
        string bitstr = "1" + Uint2BitString(encode,INPUT_ENCODE_BIT_NUM);
        for (size_t i = 0, size = xList.size(); i < size; ++i) {
            size_t idx = xList[i]->_idx;
            assert( (idx < inputBitNum) );
            if (bitstr[idx] == '0') n->_paramList[INPUT].push_back(const0);
            else                    n->_paramList[INPUT].push_back(const1);
        }
    }
}

void Aut::renameITO(const string& name, VmtNode* n)
{
    n->_name = name;
    _vmap.insert(Str2VmtNode(name, n));
}

void Aut::renameITO1Aut()
{
    renameITO("tmpI1", getI());
    renameITO("tmpT1", getT());
    renameITO("tmpO1", getO());
}

void Aut::renameITOs2Aut(Aut* a1, Aut* a2)
{
    renameITO("tmpI1", a1->getI());
    renameITO("tmpI2", a2->getI());
    renameITO("tmpT1", a1->getT());
    renameITO("tmpT2", a2->getT());
    renameITO("tmpO1", a1->getO());
    renameITO("tmpO2", a2->getO());
}

void Aut::integrate(Aut* a1, Aut* a2)
{
    assert( (_imdList.empty()) );
    assert( (_itoList.empty()) );
    // _evar , _lvar may already be non-empty
    
    // merge evar
    integrateMerge(EXIST,a1,a2);

    // State variable indexing is continuous, make sure _stateVarNum is already handled
    _piList[STATE].clear();
    _piList[STATE_N].clear();
    for (size_t i = 0; i < _stateVarNum; ++i) {
        _piList[STATE].push_back(piList[STATE][i]);
        _piList[STATE_N].push_back(piList[STATE_N][i]);
        _vmap.insert(Str2VmtNode(piList[STATE][i]->_name,piList[STATE][i]));
        _vmap.insert(Str2VmtNode(piList[STATE_N][i]->_name,piList[STATE_N][i]));
    }
    
    // merge _lvar
    integrateMerge(LEN  ,a1,a2);
    integrateMerge(LEN_N,a1,a2);

    // merge _predBV _predIV
    integrateMerge(PREDBV,a1,a2);
    integrateMerge(PREDIV,a1,a2);
    
    // merge _imdList
    _imdList = a1->_imdList;
    for (size_t i = 0, size = a2->_imdList.size(); i < size; ++i)
        _imdList.push_back(a2->_imdList[i]);

    // merge _predList
    _predList = a1->_predList;
    for (size_t i = 0, size = a2->_predList.size(); i < size; ++i)
        _predList.push_back(a2->_predList[i]);


    #ifndef AUT_PARAM_NDEBUG
        // merget _PARAMList
        assert( (_PARAMList.empty()) );
        for (size_t i = 0, size = a1->_PARAMList.size(); i < size; ++i)
            for (size_t j = 0, size1 = a2->_PARAMList.size(); j < size1; ++j)
                assert( (a1->_PARAMList[i] != a2->_PARAMList[j]) );
        _PARAMList = a1->_PARAMList;
        for (size_t i = 0, size = a2->_PARAMList.size(); i < size; ++i)
            _PARAMList.push_back(a2->_PARAMList[i]);
    #endif
}

void Aut::integrateMerge(const VmtType& type, Aut* a1, Aut* a2)
{
    assert( (isPI(type) && type != INPUT) );
    const VmtNodeList& v1 = a1->_piList[type];
    const VmtNodeList& v2 = a2->_piList[type];
    VmtNodeList& local  = _piList[type];
    VmtNodeList& global =  piList[type];

    set<size_t> idxSet;
    for (size_t i = 0, size = v1.size(); i < size; ++i)
        idxSet.insert(v1[i]->_idx);
    for (size_t i = 0, size = v2.size(); i < size; ++i)
        idxSet.insert(v2[i]->_idx);
    for (size_t i = 0, size = local.size(); i < size; ++i)
        idxSet.insert(local[i]->_idx);
    
    local.clear();

    for (set<size_t>::iterator it = idxSet.begin(); it != idxSet.end(); ++it) {
        local.push_back(global[*it]);
        _vmap.insert(Str2VmtNode(global[*it]->_name,global[*it]));
    }
}

size_t Aut::addEVar(const size_t& expandNum)
{
    size_t evbpos = piList[EXIST].size();
    expandPIList( EXIST , evbpos + expandNum - 1 );
    for (size_t i = evbpos, size = evbpos + expandNum; i < size; ++i) {
        _piList[EXIST].push_back(piList[EXIST][i]);
        _vmap.insert(Str2VmtNode(piList[EXIST][i]->_name,piList[EXIST][i]));
    }
    return evbpos;
}

size_t Aut::addStateVar(const size_t& expandNum)
{
    size_t svbpos = _stateVarNum;
    expandPIList( STATE , svbpos + expandNum - 1 );
    for (size_t i = svbpos, size = svbpos + expandNum; i < size; ++i) {
        _piList[STATE  ].push_back(piList[STATE  ][i]);
        _piList[STATE_N].push_back(piList[STATE_N][i]);
        _vmap.insert(Str2VmtNode(piList[STATE  ][i]->_name,piList[STATE  ][i]));
        _vmap.insert(Str2VmtNode(piList[STATE_N][i]->_name,piList[STATE_N][i]));
    }
    _stateVarNum += expandNum;
    return svbpos;
}

void Aut::addLVar(const size_t& lvarIdx)
{
    check(this,lvarIdx);
    expandPIList( LEN , lvarIdx );
    // maintain increasing order
    bool isCurPlace = 0, isNxtPlace = 0;
    for (VmtNodeList::iterator it = _piList[LEN].begin(); it != _piList[LEN].end(); ++it)
        if ((*it)->_idx > lvarIdx) {
            isCurPlace = 1;
            _piList[LEN].insert(it,piList[LEN][lvarIdx]);
            break;
        }
    for (VmtNodeList::iterator it = _piList[LEN_N].begin(); it != _piList[LEN_N].end(); ++it)
        if ((*it)->_idx > lvarIdx) {
            isNxtPlace = 1;
            _piList[LEN_N].insert(it,piList[LEN_N][lvarIdx]);
            break;
        }
    if (!isCurPlace) _piList[LEN].push_back(piList[LEN][lvarIdx]);
    if (!isNxtPlace) _piList[LEN_N].push_back(piList[LEN_N][lvarIdx]);

    _vmap.insert(Str2VmtNode(piList[LEN][lvarIdx]->_name,piList[LEN][lvarIdx]));
    _vmap.insert(Str2VmtNode(piList[LEN_N][lvarIdx]->_name,piList[LEN_N][lvarIdx]));
}

VmtNode* Aut::getI() const
{
    return _itoList[0]->_children[0]->_children[0];
}

VmtNode* Aut::getO() const
{
    return _itoList[2]->_children[0]->_children[0]->_children[0];
}

VmtNode* Aut::getT() const
{
    return _itoList[1]->_children[0]->_children[0];
}

void Aut::setI(VmtNode* n)
{
    _itoList[0]->_children[0]->_children[0] = n;
}

void Aut::setO(VmtNode* n)
{
    _itoList[2]->_children[0]->_children[0]->_children[0] = n;
}

void Aut::setT(VmtNode* n)
{
    _itoList[1]->_children[0]->_children[0] = n;
}

/**Function*************************************************************

  Name        []

  Description [I/O]

***********************************************************************/
void Aut::print() const 
{
    cout << "[Aut::print] itoList" << endl;
    for (size_t i = 0, s = _itoList.size(); i < s; ++i)
        _itoList[i]->print(0);
    cout << "[Aut::print] imdList" << endl;
    for (size_t i = 0, s = _imdList.size(); i < s; ++i)
        _imdList[i]->print(0);
    for (size_t i = 0, s = _predList.size(); i < s; ++i)
        _predList[i]->print(0);
}

void Aut::printPARAMList() const
{
    cout << "[Aut::printPARAMList]\n";
    for (size_t i = 0, size = _PARAMList.size(); i < size; ++i) {
        VmtNode* n = _PARAMList[i];
        assert( (n->_type == PARAM) );
        cout << n->_name << " source=" << n->_source->_name << " params=";
        for (size_t j = 0, size1 = n->_paramList.size(); j < size1; ++j)
            for (size_t k = 0, size2 = n->_paramList[j].size(); k < size2; ++k)
                cout << " " << n->_paramList[j][k]->_name;
        cout << endl;
    }
}

void Aut::printPIList() const
{
    cout << "[Aut::printPIList]\n";
    for (size_t i = 0; i < PI_NUM; ++i) {
        cout << getTypeStr(static_cast<VmtType>(i));
        for (size_t j = 0, size = _piList[i].size(); j < size; ++j)
            cout << " " << _piList[i][j]->_name;
        cout << endl;
    }
}

void Aut::parse(const char* fileName)
{
    string line;
    ifstream file(fileName);

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    
    vector<string> tokenList,paramList;
    int sCnt = 0,inputCnt = 0;
    while (getline(file,line)) {
        if (line[0] == ';') {
            ++sCnt;
            continue;
        }
        if (sCnt == 1) continue;
        tokenList.clear();
        paramList.clear();
        vmtTokenize(line,paramList,tokenList);
        #ifndef AUT_NDEBUG
            cout << "[Aut::parse] sCnt = " << sCnt << endl;
            cout << "[Aut::parse] line = \"" << line << "\"" << endl;
            cout << "[Aut::parse] tokenList = [ \"" << tokenList[0] << "\" , \"" << tokenList[1] << "\" ]" <<endl;
            cout << "[Aut::parse] paramList = [ ";
            for (size_t i = 0, s = paramList.size(); i < s; ++i) {
                if (i > 0) cout << ", ";
                cout << "\"" << paramList[i] << "\" ";
            }
            cout << "]" << endl;
        #endif
        if (tokenList[0] == "epsilon" || tokenList[0] == "leftAngle" || tokenList[0] == "rightAngle") continue;
        if (sCnt == 0) {
            char symbol = tokenList[0][0];
            // only parse current state variable
            if ( *(tokenList[0].rbegin()) == 't' ) continue;
            if ( symbol == piSymbolC[INPUT] ) { ++inputCnt; continue; }
            
            size_t idx = stoi(tokenList[0].substr(1));
            VmtType cType,nType;
            if      ( symbol == piSymbolC[STATE] ) { cType = STATE ; nType = STATE_N; ++_stateVarNum;}
            else if ( symbol == piSymbolC[LEN]   ) { cType = LEN   ; nType = LEN_N  ; }
            else if ( symbol == piSymbolC[EXIST] ) { cType = EXIST ; }
            else if ( symbol == piSymbolC[PREDBV]) { cType = PREDBV; }
            else    { assert( (symbol == piSymbolC[PREDIV]) ); cType = PREDIV; }
            
            if ( symbol == piSymbolC[STATE] || symbol == piSymbolC[LEN] ) {
                expandPIList(cType,idx);
                _piList[cType].push_back(piList[cType][idx]);
                _piList[nType].push_back(piList[nType][idx]);
                _vmap.insert(Str2VmtNode(piList[cType][idx]->_name,piList[cType][idx]));
                _vmap.insert(Str2VmtNode(piList[nType][idx]->_name,piList[nType][idx]));
            }
            else {
                VmtNode* n = new VmtNode( symbol + itos(idx) , idx );
                _piList[cType].push_back(n);
                _vmap.insert(Str2VmtNode(n->_name,n));
            }
        }
        else {
            // type is handled in the constructor
            VmtNode* module = new VmtNode(tokenList[0]);
            VmtNode* body   = buildVmtNode(tokenList[1],0,tokenList[1].size(),_vmap);
            module->addChild(body);
            for (size_t i = 0, s = paramList.size(); i < s; ++i) {
                Str2VmtNodeMap::iterator jt = _vmap.find(paramList[i]);
                assert( (jt != _vmap.end()) );
                const VmtType& type = jt->second->_type;
                assert( (isPI(type)) );
                module->_paramList[type].push_back(jt->second);
            }
            //cout << "n1=" << module->_name << " n2=" << body->_name << endl;
             _vmap.insert(Str2VmtNode(module->_name,module));
            if      (sCnt == 2) _imdList.push_back(module);
            else if (sCnt == 3) _predList.push_back(module);
            else if (sCnt == 4) _itoList.push_back(module);
        }
    }
    file.close();

    assert( (inputCnt == inputBitNum) );

    // rename local variables and assign to piList for EXIST , PREDBV , PREDIV
    assignGlobalPIList(EXIST);
    assignGlobalPIList(PREDBV);
    assignGlobalPIList(PREDIV);
    
    // rebuild _vmap
    initVMap();
    for (size_t i = 0; i < PI_NUM; ++i)
        buildVMap(static_cast<VmtType>(i));
    buildVMap(_imdList);
    buildVMap(_itoList);
    buildVMap(_predList);
    spotNEG();
    renameDef();
    #ifndef AUT_PARAM_NDEBUG
        collectPARAM();
        printPARAMList();
    #endif
}

void Aut::write(const string& fileName) const
{
    write(fileName.c_str());
}

void Aut::write(const char* fileName) const
{
    ofstream file(fileName);
    
    writeDeclareFun( INPUT  , file );
    writeDeclareFun( EXIST  , file );
    writeDeclareFun( STATE  , file );
    writeDeclareFun( LEN    , file );
    writeDeclareFun( PREDBV , file );
    writeDeclareFun( PREDIV , file );
    
    file << ";\n";
    
    int nxtCnt = -1;
    writeNextFun( INPUT , nxtCnt, file );
    writeNextFun( EXIST , nxtCnt, file );
    writeNextFun( STATE , nxtCnt, file );
    writeNextFun( LEN   , nxtCnt, file );
    writeNextFun( PREDBV, nxtCnt, file );
    writeNextFun( PREDIV, nxtCnt, file );
    
    file << ";\n";
    
    bool eUsed = 0, lUsed = 0, rUsed = 0;

    if (epsilon   ->_flag == gflag) eUsed = 1;
    if (leftAngle ->_flag == gflag) lUsed = 1;
    if (rightAngle->_flag == gflag) rUsed = 1;

    ++gflag;

    if (eUsed) writeDefineFun( epsilon   , file );
    if (lUsed) writeDefineFun( leftAngle , file );
    if (rUsed) writeDefineFun( rightAngle, file );

    for (size_t i = 0,size = _imdList.size(); i < size; ++i)
        writeDefineFun(_imdList[i],file);
    file << ";\n";
    
    for (size_t i = 0,size = _predList.size(); i < size; ++i)
        writeDefineFun(_predList[i],file);
    file << ";\n";

    // ITO cannot be specified their params
    for (size_t i = 0,size = _itoList.size(); i < size; ++i)
        writeDefineFun(_itoList[i],file,0);
    
    file.close();
}

void Aut::writeDeclareFun(const VmtType& type, ofstream& file) const
{
    assert( (isCurPI(type)) );
    string dtype = (type == LEN || type == PREDIV) ? "Int" : "Bool";
    for (size_t i = 0, size = _piList[type].size(); i < size; ++i) {
        file << "(declare-fun " << _piList[type][i]->_name << " () "      << dtype << ")\n" ;
        file << "(declare-fun " << _piList[type][i]->_name << ".next () " << dtype << ")\n" ;
    }
}

void Aut::writeNextFun(const VmtType& type, int& nxtCnt, ofstream& file) const
{
    assert( (isCurPI(type)) );
    for (size_t i = 0, size = _piList[type].size(); i < size; ++i)
        file << "(define-fun nxt" << ++nxtCnt << " () Bool " 
             << "(! " << _piList[type][i]->_name << " :next " 
             << _piList[type][i]->_name<< ".next))\n";
}

void Aut::writeDefineFun(VmtNode* n, ofstream& file, const bool& needParam) const
{
    #ifndef AUT_NDEBUG
        cout << "[Aut::writeDefineFun] gflag=" << gflag << endl;
        cout << "[Aut::writeDefineFun] " << n->_name << " " << n << " " << getTypeStr(n->_type) << " " << n->_flag << endl;
    #endif
    n->_flag = gflag;
    file << "(define-fun " << n->_name << " (";
    if (needParam) n->writeParamHead(file);
    file << ") Bool ";
    n->_children[0]->write(0,file);
    file << ")\n";
}

/**Function*************************************************************

  Name        []

  Description [Operations]

***********************************************************************/

string Aut::CSNSEquiv(const VmtType& type) const
{
    assert( (type == STATE || type == LEN) );
    string ret;
    if (type == STATE) {
        for (size_t i = 0; i < _stateVarNum; ++i)
            ret += " (= " + _piList[STATE_N][i]->_name + " " + _piList[STATE][i]->_name + ")";
    }
    else {
        for (size_t i = 0, size = _piList[LEN].size(); i < size; ++i)
            ret += " (= " + _piList[LEN_N][i]->_name + " " + _piList[LEN][i]->_name + ")";
    }
    return ret;
}

void Aut::addlen(const string& lvarIdxStr)
{
    // Add Length Variable
    size_t lvarIdx = stoi(lvarIdxStr);
    addLVar(lvarIdx);
    string lv  = piList[LEN  ][lvarIdx]->_name;
    string lvn = piList[LEN_N][lvarIdx]->_name;

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::addlen] " << _name << " lvar=" << lvarIdxStr << endl;
    #endif

    // Add Length Constraint
    renameITO("tmpT", getT());
    defineFun("tmp1", "(and (not epsilon) (= " + lvn + " (+ " + lv + " 1)))", _imdList);
    defineFun("tmp2", "(and epsilon (= " + lvn + " " + lv + "))", _imdList);
    defineFun("tmp3", "(and tmpT (or tmp1 tmp2))", _imdList, &Aut::setT);
    renameDef();
}

void Aut::intersect(Aut* a1, Aut* a2)
{
    check(a1,a2);
    
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::intersect] " << a1->_name << " " << a2->_name << endl;
        cout << "[Aut::intersect] origin  svar " << a1->_name << ":";
        for (size_t i = 0, size = a1->_piList[STATE].size(); i < size; ++i)
            cout << " " << a1->_piList[STATE][i]->_name;
        cout << " " << a2->_name << ":";
        for (size_t i = 0, size = a2->_piList[STATE].size(); i < size; ++i)
            cout << " " << a2->_piList[STATE][i]->_name;
    #endif

    // Shift State Variable of a2
    a2->shiftStateVar(a1->_stateVarNum);
    _stateVarNum = a1->_stateVarNum + a2->_stateVarNum;

    string csnseq1 = a1->CSNSEquiv(STATE) + a1->CSNSEquiv(LEN);
    string csnseq2 = a2->CSNSEquiv(STATE) + a2->CSNSEquiv(LEN);
    
    #ifndef AUT_OP_NDEBUG
        cout << "\n[Aut::intersect] shifted svar " << a1->_name << ":";
        for (size_t i = 0, size = a1->_piList[STATE].size(); i < size; ++i)
            cout << " " << a1->_piList[STATE][i]->_name;
        cout << " " << a2->_name << ":";
        for (size_t i = 0, size = a2->_piList[STATE].size(); i < size; ++i)
            cout << " " << a2->_piList[STATE][i]->_name;
        cout << endl;
    #endif

    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial
    defineFun( "tmp1", "(and tmpI1 tmpI2)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList );
    
    // Transition
    defineFun( "T1_epsilon", "(or tmpT1 (and epsilon" + csnseq1 + "))", _imdList);
    defineFun( "T2_epsilon", "(or tmpT2 (and epsilon" + csnseq2 + "))", _imdList);
    defineFun( "tmp2", "(and T1_epsilon T2_epsilon)", _imdList);
    defineFun( "T", "(! tmp2 :trans true)", _itoList);

    // Accepting
    defineFun( "tmp3", "(and tmpO1 tmpO2)", _imdList);
    defineFun( "O", "(! (not tmp3) :invar-property 0)", _itoList);

    renameDef();
}

void Aut::aut_union(Aut* a1,Aut* a2)
{
    check(a1,a2);
    
    // Build PARAM Node
    //addParamNode( "I2_PARAM", a2->getI());
    
    _stateVarNum = (a1->_stateVarNum > a2->_stateVarNum)? a1->_stateVarNum : a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    string sa  = piList[STATE]  [svbpos]->_name;
    string san = piList[STATE_N][svbpos]->_name;

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut:aut_union] " << a1->_name << " " << a2->_name << " alpha=" << svbpos << endl;
    #endif

    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial
    defineFun( "tmp1", "(or (and (not " + sa + ") tmpI1) (and " + sa + " tmpI2))", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);
    
    // Transition
    defineFun( "tmp2", "(and (not " + sa + ") (not " + san + ") tmpT1)", _imdList);
    defineFun( "tmp3", "(and " + sa + " " + san + " tmpT2)", _imdList);
    defineFun( "tmp4", "(or tmp2 tmp3)", _imdList);
    defineFun( "T", "(! tmp4 :trans true)", _itoList);

    // Accepting
    defineFun( "tmp5", "(or (and (not " + sa + ") tmpO1) (and " + sa + " tmpO2))", _imdList);
    defineFun( "O", "(! (not tmp5) :invar-property 0)", _itoList);

    renameDef();
}

void Aut::concate(Aut* a1,Aut* a2)
{
    check(a1,a2);
    
    // Build PARAM Node
    addParamNode( "I2_PARAM", a2->getI());
    
    _stateVarNum = (a1->_stateVarNum > a2->_stateVarNum)? a1->_stateVarNum : a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    string sa  = piList[STATE]  [svbpos]->_name;
    string san = piList[STATE_N][svbpos]->_name;

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut:concate] " << a1->_name << " " << a2->_name << " alpha=" << svbpos << endl;
    #endif

    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);
    
    // Transition
    defineFun( "tmp2", "(and (not " + sa + ") (not " + san + ") tmpT1" + a2->CSNSEquiv(LEN) + ")", _imdList);
    defineFun( "tmp3", "(and " + sa + " " + san + " tmpT2" + a1->CSNSEquiv(LEN) + ")", _imdList);
    defineFun( "tmp4", "(and epsilon (not " + sa + ") " + san + " tmpO1 I2_PARAM" + CSNSEquiv(LEN) + ")", _imdList);
    defineFun( "tmp5", "(or tmp2 tmp3 tmp4)", _imdList);
    defineFun( "T", "(! tmp5 :trans true)", _itoList);

    // Accepting
    defineFun( "tmp6", "(and " + sa + " tmpO2)", _imdList);
    defineFun( "O", "(! (not tmp6) :invar-property 0)", _itoList);

    renameDef();
}

size_t Aut::mark()
{
    check(this);
    // construct the equivalence before adding svar
    string sveq   = CSNSEquiv(STATE);
    string lveq   = CSNSEquiv(LEN);
    size_t svbpos = addStateVar(1);
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::mark] " << _name << " alpha=" << svbpos << endl;
    #endif
    string sa  = piList[STATE]  [svbpos]->_name;
    string san = piList[STATE_N][svbpos]->_name;
    
    renameITO1Aut();
    
    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList, &Aut::setI );

    // Transition Relation
    defineFun( "tmp2", "(and (= " + san + " " + sa + ") (not " + leftAngle->_name + ") (not " + rightAngle->_name + ") tmpT1)", _imdList);
    defineFun( "tmp3", "(or (and (not " + sa + ") " + san + " " + leftAngle->_name + ") (and " + sa + " (not " + san + ") " + rightAngle->_name + "))", _imdList);
    defineFun( "tmp4", "(and" + sveq + lveq + " tmp3)", _imdList);
    defineFun( "tmp5", "(or tmp2 tmp4)", _imdList, &Aut::setT);

    // Accepting States
    defineFun( "tmp5", "(and (not " + sa + ") tmpO1)", _imdList, &Aut::setO );

    renameDef();
    return svbpos;
}

void Aut::replace_A4(Aut* a1, Aut* a2)
{
    check(a1,a2);
    
    // Build PARAM Nodes
    addParamNode( "I1_PARAM" , a1->getI() );
    addParamNode( "I2_PARAM" , a2->getI() );

    _stateVarNum  = (a1->_stateVarNum > a2->_stateVarNum)? a1->_stateVarNum : a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::replace_A4] " << a1->_name << " " << a2->_name << " beta=" << svbpos << endl;
    #endif
    
    string sa  = piList[STATE]  [svbpos]->_name;
    string san = piList[STATE_N][svbpos]->_name;
    
    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI2)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);

    // Transition Relation
    defineFun( "tmp2", "(and (not " + sa + ") (not " + san + ") (not " + leftAngle->_name + ") (not " + rightAngle->_name + ") tmpT2)", _imdList);
    defineFun( "tmp3", "(and " + sa + " " + san + " (not " + leftAngle->_name + ") (not " + rightAngle->_name + ") tmpT1)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " " + leftAngle->_name + " tmpO2 I1_PARAM)", _imdList);
    defineFun( "tmp5", "(and " + sa + " (not " + san + ") " + rightAngle->_name + " tmpO1 I2_PARAM)", _imdList);
    defineFun( "tmp6", "(or tmp2 tmp3 tmp4 tmp5)", _imdList);
    defineFun( "T", "(! tmp6 :trans true)", _itoList);
    
    // Accepting States
    defineFun( "tmp7", "(and (not " + sa + ") tmpO2)", _imdList);
    defineFun( "O", "(! (not tmp7) :invar-property 0)", _itoList);
    
    renameDef();
}

void Aut::replace(Aut* a1, Aut* a2, const size_t& alpha)
{
    check(a1,a2);
    
    a2->shiftStateVar(a1->_stateVarNum);
    _stateVarNum = a1->_stateVarNum + a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    
    // Build PARAM Nodes
    size_t evbpos = addEVar(a1->getT()->_paramList[INPUT].size());
    addParamNode( "I2_PARAM"   , a2->getI());
    addParamNode( "T1_PARAM_1" , a1->getT() , LEFT_ANGLE_BRACKET_ENCODE,0);
    addParamNode( "T1_PARAM_2" , a1->getT() , evbpos,1);
    addParamNode( "T1_PARAM_3" , a1->getT() , RIGHT_ANGLE_BRACKET_ENCODE,0);
    
    string sa0  = piList[STATE  ][alpha ]->_name;
    string san0 = piList[STATE_N][alpha ]->_name;
    string sa1  = piList[STATE  ][svbpos]->_name;
    string san1 = piList[STATE_N][svbpos]->_name;
    
    string csnseq = a1->CSNSEquiv(STATE) + a1->CSNSEquiv(LEN);

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::replace] " << a1->_name << " " << a2->_name << " alpha=" << alpha << " gamma=" << svbpos << endl;
    #endif
    
    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial State
    defineFun( "tmp1", "(and (not " + sa1 + ") tmpI1 tmpI2)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);
    
    // Transition Relation
    defineFun( "tmp2", "(and (not " + sa0 + ") (not " + san0 + ") tmpT1 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp3", "(and (not " + sa0 + ") (not " + san0 + ") epsilon" + csnseq + " (not " + sa1 + ") " + san1 + " tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp4", "(and (not " + sa0 + ") (not " + san0 + ")" + csnseq + " " + sa1 + " " + san1 + " tmpT2)", _imdList);
    defineFun( "tmp5", "(and (not " + sa0 + ") " + san0 + " epsilon T1_PARAM_1 " + sa1 + " (not " + san1 + ") I2_PARAM tmpO2)", _imdList);
    defineFun( "tmp6", "(and " + sa0 + " " + san0 + " epsilon T1_PARAM_2 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp7", "(and " + sa0 + " (not " + san0 + ") epsilon T1_PARAM_3 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp8", "(or tmp2 tmp3 tmp4 tmp5 tmp6 tmp7)", _imdList);
    defineFun( "T", "(! tmp8 :trans true)", _itoList);
    
    // Accepting States
    defineFun( "tmp9", "(and (not " + sa1 + ") tmpO1 tmpI2)", _imdList);
    defineFun( "O", "(! (not tmp9) :invar-property 0)", _itoList);
    
    renameDef();
}

void Aut::prefix(const string& lvarIdxStr)
{
    // Build PARAM Node
    size_t evbpos  = addEVar(getT()->_paramList[INPUT].size());
    addParamNode("T1_PARAM",getT(),evbpos,1);
    
    string csnseq  = CSNSEquiv(STATE) + CSNSEquiv(LEN);
    size_t lvarIdx = stoi(lvarIdxStr);
    addLVar(lvarIdx);
    size_t svbpos = addStateVar(1);
    string sa  = piList[STATE  ][svbpos ]->_name;
    string san = piList[STATE_N][svbpos ]->_name;
    string lv  = piList[LEN    ][lvarIdx]->_name;
    string lvn = piList[LEN_N  ][lvarIdx]->_name;

    string lveq    = " (= " + lvn + " " + lv + ")";
    string lvincre = " (= " + lvn + " (+ " + lv + " 1))";

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::prefix] " << _name << " alpha=" << svbpos << " lvar=" << lvarIdxStr << endl;
    #endif

    renameITO1Aut();

    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList, &Aut::setI );

    // Transition Relation
    defineFun( "T1_epsilon", "(or tmpT1 (and epsilon" + csnseq + "))", _imdList);
    
    defineFun( "tmp20", "(and (not epsilon)" + lvincre + ")", _imdList);
    defineFun( "tmp21", "(and epsilon" + lveq + ")", _imdList);
    defineFun( "tmp22", "(or tmp20 tmp21)", _imdList);

    defineFun( "tmp3", "(and (not " + sa + ") (not " + san + ") T1_epsilon tmp22)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " epsilon" + csnseq + lveq + ")", _imdList);
    defineFun( "tmp5", "(and " + sa + " " + san + " epsilon T1_PARAM" + lveq + ")", _imdList);
    defineFun( "tmp6", "(or tmp3 tmp4 tmp5)", _imdList, &Aut::setT );
    
    // Accepting States
    defineFun( "tmp7", "(and " + sa + " tmpO1)", _imdList, &Aut::setO );
    renameDef();
}

void Aut::suffix(const string& lvarIdxStr)
{
    string csnseq  = CSNSEquiv(STATE) + CSNSEquiv(LEN);
    
    // Rename ITO
    renameITO1Aut();
    
    // Build PARAM Node
    size_t evbpos = addEVar(getT()->_paramList[INPUT].size());
    VmtNode* ye   = new VmtNode("YEPSILON");
    ye->addChild(new VmtNode("and"));
    
    string bitstr = "1" + Uint2BitString(EPSILON_ENCODE,INPUT_ENCODE_BIT_NUM);
    for (size_t i = 0, size = getT()->_paramList[INPUT].size(); i < size; ++i) {
        const size_t& idx = getT()->_paramList[INPUT][i]->_idx;
        if (bitstr[idx] == '0') {
            VmtNode* notNode = new VmtNode("not");
            notNode->addChild(piList[EXIST][evbpos+i]);
            ye->_children[0]->addChild(notNode);
        }
        else {
            ye->_children[0]->addChild(piList[EXIST][evbpos+i]);
        }
    }
    _imdList.push_back(ye);
    _vmap.insert(Str2VmtNode("YEPSILON",ye));

    defineFun( "T1_epsilon", "(or tmpT1 (and epsilon" + csnseq + "))", _imdList);
    VmtNode* T1EPSILON = _imdList.back();
    ++gflag;
    T1EPSILON->clearParam(0);
    ++gflag;
    T1EPSILON->buildParam(0);
    addParamNode("T1_PARAM",T1EPSILON,evbpos,1);

    // Add STATE LEN variables
    size_t lvarIdx = stoi(lvarIdxStr);
    addLVar(lvarIdx);
    size_t svbpos = addStateVar(1);
    string sa  = piList[STATE  ][svbpos ]->_name;
    string san = piList[STATE_N][svbpos ]->_name;
    string lv  = piList[LEN    ][lvarIdx]->_name;
    string lvn = piList[LEN_N  ][lvarIdx]->_name;
    
    string lveq    = " (= " + lvn + " " + lv + ")";
    string lvincre = " (= " + lvn + " (+ " + lv + " 1))";

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::suffix] " << _name << " alpha=" << svbpos << " lvar=" << lvarIdxStr << endl;
    #endif

    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList, &Aut::setI );
    
    // Transition Relation
    defineFun( "tmp20", "(and (not YEPSILON)" + lvincre + ")", _imdList);
    defineFun( "tmp21", "(and YEPSILON" + lveq + ")", _imdList);
    defineFun( "tmp23", "(or tmp20 tmp21)", _imdList);

    defineFun( "tmp3", "(and (not " + sa + ") (not " + san + ") epsilon T1_PARAM tmp23)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " epsilon" + csnseq + lveq + ")", _imdList);
    defineFun( "tmp5", "(and " + sa + " " + san + " tmpT1" + lveq + ")", _imdList);
    defineFun( "tmp6", "(or tmp3 tmp4 tmp5)", _imdList, &Aut::setT );
    
    // Accepting States
    defineFun( "tmp7", "(and " + sa + " tmpO1)", _imdList, &Aut::setO );
    
    renameDef();
}

void Aut::addpred(const STRList& declList, const STRList& predList)
{
    /*
    string line;
    ifstream file(fileName.c_str());

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    size_t sCnt = 0, pCnt = 0;
    // using a stand-alone vmap to prevent naming collision
    Str2VmtNodeMap predVMap;
    cout << "pvmap in addpred " << &predVMap << endl;
    while (getline(file,line)) {
        if (line == ";") {
            ++sCnt; continue;
        }
        if      (sCnt == 0) parseDef (line, predVMap);
        else if (sCnt == 1) parsePred(line, pCnt, predVMap);
        else {
            cout << "[ERROR::Aut::addpred] invalid sCnt=" << sCnt << endl;
        }
    }
    */
    size_t sCnt = 0, pCnt = 0;
    // using a stand-alone vmap to prevent naming collision
    Str2VmtNodeMap predVMap;
    for (size_t i = 0, size = declList.size(); i < size; ++i) parseDef (declList[i], predVMap);
    for (size_t i = 0, size = predList.size(); i < size; ++i) parsePred(predList[i], pCnt, predVMap);
    
    // renaming and replace global by local
    for (size_t i = 0, size = _piList[PREDBV].size(); i < size; ++i) {
        _piList[PREDBV][i]->_name = piSymbolS[PREDBV] + itos(i);
        _piList[PREDBV][i]->_idx  = i;
    }
    piList[PREDBV] = _piList[PREDBV];

    for (size_t i = 0, size = _piList[PREDIV].size(); i < size; ++i) {
        _piList[PREDIV][i]->_name = piSymbolS[PREDIV] + itos(i);
        _piList[PREDIV][i]->_idx  = i;
    }                                  
    piList[PREDIV] = _piList[PREDIV];

    // and predicates together
    VmtNode* n0   = new VmtNode("O_addpred");
    VmtNode* and0 = new VmtNode("and");
    n0->addChild(and0);
    and0->addChild(getO());
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        and0->addChild(_predList[i]);
    }
    _predList.push_back(n0);
    setO(n0);
    
    // add initial condiditon for length variables (i.e. (= nk 0) )
    VmtNode* n1   = new VmtNode("I_addpred");
    VmtNode* and1 = new VmtNode("and");
    n1->addChild(and1);
    and1->addChild(getI());
    for (size_t i = 0, size = _piList[LEN].size(); i < size; ++i) {
        VmtNode* eq = new VmtNode("=");
        eq->addChild(new VmtNode("0"));
        eq->addChild(_piList[LEN][i]);
        and1->addChild(eq);
    }
    _predList.push_back(n1);
    setI(n1);
    cout << "g1\n";
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        _predList[i]->print(0);
        _predList[i]->printPARAM();
    }
    cout << "ADDPRED BEGIN\n";
    spotNEG();
    renameDef();
    cout << "ADDPRED END\n";
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        _predList[i]->print(0);
        _predList[i]->printPARAM();
    }
    //print();
}

void Aut::parseDef(const string& line, Str2VmtNodeMap& vmap)
{
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::parseDef] line = \"" << line << "\"" << endl;
    #endif
    /*
    size_t i = 0, j = 0;
    string name, type;
    while (line[i] != ' ') ++i;
    j = ++i;
    while (line[i] != ' ') ++i;
    name = line.substr(j,i-j);
    i += 4;
    j = i;
    while (line[i] != ')') ++i;
    type = line.substr(j,i-j);
    */
    STRList tokenList;
    str2tokens(line," ",tokenList);
    const string& name = tokenList[0];
    const string& type = tokenList[1];
    
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::parseDef] name=" << name << " type=" << type << endl;
    #endif

    VmtNode* n = new VmtNode(name);
    cout << "insert " << name << endl;
    vmap.insert(Str2VmtNode(name,n));

    assert( (type == "Bool" || type == "Int") );

    if (type == "Bool") {
        n->_type = PREDBV;
        _piList[PREDBV].push_back(n);
    }
    else {
        n->_type = PREDIV;
        _piList[PREDIV].push_back(n);
    }
}

void Aut::parsePred(const string& line, size_t& pCnt, Str2VmtNodeMap& vmap)
{
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::parsePred] line = \"" << line << "\"" << endl;
    #endif
    if (line[0] != '(') {
        size_t i = 0;
        while (line[i] != ')') ++i;
        string var = line.substr(0,i-0);
        Str2VmtNodeMap::iterator it = vmap.find(var);
        assert((it != vmap.end()));
        defineFun( "p" + itos(pCnt++), var, _predList, vmap);
    }
    else if ( line.size() >= 7) {
        string op = line.substr(1,6);
        STRList tokenList;
        str2tokens(line," ()",tokenList);

        if ( op == "trklen") {
            size_t lvarIdx = stoi(tokenList[2]);
            VmtNode* pNode = new VmtNode( "p" + itos(pCnt++) );
            _predList.push_back(pNode);
            vmap.insert(Str2VmtNode(pNode->_name,pNode));

            const string& aname = tokenList[1];

            VmtNode* alias = 0;
            Str2VmtNodeMap::iterator it = vmap.find(aname);
            if (it == vmap.end()) {alias = new VmtNode(aname); cout << "not found " << aname << endl;}
            else                  {alias = it->second; cout << "find " << aname << endl;}

            VmtNode* eq = new VmtNode("=");
            eq->addChild(alias);
            eq->addChild(piList[LEN][lvarIdx]);
            pNode->addChild(eq);
        }
        else if ( op == "trkidx") {
            assert( (tokenList.size() == 5) );
            size_t n0 = stoi(tokenList[3]);
            size_t n1 = stoi(tokenList[4]);
            VmtNode* pNode0 = new VmtNode( "p" + itos(pCnt++) );
            VmtNode* pNode1 = new VmtNode( "p" + itos(pCnt++) );
            _predList.push_back(pNode0);
            _predList.push_back(pNode1);
            vmap.insert(Str2VmtNode(pNode0->_name,pNode0));
            vmap.insert(Str2VmtNode(pNode1->_name,pNode1));
            
            const string& aname = tokenList[2];
            const string& kname = tokenList[1];

            VmtNode* alias = 0;
            VmtNode* kNode = 0;
            Str2VmtNodeMap::iterator it0 = vmap.find(aname);
            assert( (it0 != vmap.end()) );
            alias = it0->second;
            Str2VmtNodeMap::iterator it1 = vmap.find(kname);
            if (it1 == vmap.end()) kNode = new VmtNode(kname);
            else                   kNode = it1->second;
            
            VmtNode* eq0   = new VmtNode("=");
            VmtNode* plus0 = new VmtNode("+");
            plus0->addChild(piList[LEN][n0]);
            plus0->addChild(piList[LEN][n1]);
            eq0->addChild(alias);
            eq0->addChild(plus0);
            pNode0->addChild(eq0);

            VmtNode* eq1   = new VmtNode("=");
            eq1->addChild(kNode);
            eq1->addChild(piList[LEN][n1]);
            pNode1->addChild(eq1);
        }
        else if ( op == "substr") {
            assert( (tokenList.size() == 5) );
            size_t n0 = stoi(tokenList[3]);
            size_t n1 = stoi(tokenList[4]);
            VmtNode* pNode0 = new VmtNode( "p" + itos(pCnt++) );
            VmtNode* pNode1 = new VmtNode( "p" + itos(pCnt++) );
            _predList.push_back(pNode0);
            _predList.push_back(pNode1);
            vmap.insert(Str2VmtNode(pNode0->_name,pNode0));
            vmap.insert(Str2VmtNode(pNode1->_name,pNode1));

            VmtNode* i1Node  = 0;
            VmtNode* i12Node = 0;
            VmtNode* i2Node  = 0;
            Str2VmtNodeMap::iterator it0  = vmap.find(tokenList[1]);
            if (it0 == vmap.end()) {
                assert( (isNumber(tokenList[1])) );
                i1Node  = new VmtNode(tokenList[1]);
                i12Node = new VmtNode(tokenList[1]);
            }
            else {
                i1Node  = it0->second;
                i12Node = i1Node;
            }
            Str2VmtNodeMap::iterator it1  = vmap.find(tokenList[2]);
            if (it1 == vmap.end()) i2Node = new VmtNode(tokenList[2]);
            else                   i2Node = it1->second;

            VmtNode* eq0   = new VmtNode("=");
            eq0->addChild(piList[LEN][n0]);
            eq0->addChild(i1Node);
            pNode0->addChild(eq0);
            
            VmtNode* eq1   = new VmtNode("=");
            VmtNode* plus1 = new VmtNode("+");
            plus1->addChild(i12Node);
            plus1->addChild(i2Node);
            eq1->addChild(piList[LEN][n1]);
            eq1->addChild(plus1);
            pNode1->addChild(eq1);
        }
        else {
            string body = line.substr(0,line.size()-0);
            defineFun( "p" + itos(pCnt++), body, _predList, vmap);
        }
    }
    else {
        string body = line.substr(0,line.size()-0);
        defineFun( "p" + itos(pCnt++), body, _predList, vmap);
    }
}

void Aut::isempty(const string& fileName)
{
    ofstream file(fileName.c_str());
    
    // set _bname of special VmtNodes and PIs
    for (size_t i = 0; i < PI_NUM; ++i)
        for (size_t j = 0, size = _piList[i].size(); j < size; ++j)
            _piList[i][j]->_bname = _piList[i][j]->_name;
    const0->_bname     = "const0"    ;
    const1->_bname     = "const1"    ;
    epsilon->_bname    = "epsilon"   ;
    leftAngle->_bname  = "leftAngle" ;
    rightAngle->_bname = "rightAngle";
    _imdList.push_back(epsilon);
    _imdList.push_back(leftAngle);
    _imdList.push_back(rightAngle);
    
    // label transitive fanin of O & T ; set _bname ; set _bit to LEN/LEN_N/NUM
    ++gflag;
    int tCnt = -1;
    getO()->buildBLIF(tCnt);
    getT()->buildBLIF(tCnt);

    #ifndef FORCE_ASSIGNED
    cout << "[setBitNum]\n";

    // no cyclic dependency issue for _imdList
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag == gflag) {
            cout << "imdList no" << i << endl;
            _imdList[i]->_children[0]->setBitNum();
        }

    // assume no cyclic dependency of Int variables in _predList
    for (size_t i = 0; i < PI_NUM; ++i) {
        if ( i != LEN && i != LEN_N && i != PREDIV) continue;
        cout << getTypeStr(i) << " :";
        for (size_t j = 0, size = _piList[i].size(); j < size; ++j)
            cout << " " << _piList[i][j]->_name << "=" << _piList[i][j]->getBit();
        cout << endl;
    }
    size_t cnt = 0;
    bool isUpdate = 1;
    while (isUpdate) {
        //assert((cnt < 5));
        ++cnt;
        isUpdate = 0;
        // bottom-up
        cout << "bottom-up\n";
        for (size_t i = 0, size = _predList.size(); i < size; ++i)
            if (_predList[i]->_flag == gflag) {
                _predList[i]->_children[0]->setBitNumUP(isUpdate);
            }
        // top-down
        cout << "top-down\n";
        for (size_t i = 0, size = _predList.size(); i < size; ++i)
            if (_predList[i]->_flag == gflag) {
                _predList[i]->_children[0]->setBitNumDN(isUpdate);
            }
    }
    cout << "ggout\n";
    #endif

    vector<set<size_t> > sizeMap(MODULE_TYPE_NUM,set<size_t>());

    // write sequential circuit
    
    file << ".model sink\n"
         << ".inputs";
    for (size_t i = 0; i < PI_NUM; ++i) {
        // inputs : INPUT, EXIST, STATE_N, LEN_N, PREDBV, PREDIV
        if ( i == STATE || i == LEN ) continue;
        for (size_t j = 0, size = _piList[i].size(); j < size; ++j) {
            if ( i == LEN_N || i == PREDIV ) {
                for (size_t k = 0; k < _piList[i][j]->_bit; ++k)
                    file << " " << _piList[i][j]->_bname << "_" << k;
                file << BLIFIndent;
            }
            else {
                file << " " << _piList[i][j]->_bname;
            }
        }
        if ( i != LEN_N && i != PREDIV && !_piList[i].empty() )
            file << BLIFIndent;
    }
    file << "\n.outputs Oisempty"
         << "\n.subckt " << getT()->_bname << BLIFIndent;
    assert( (getT()->_paramList[PREDBV].empty()) );
    assert( (getT()->_paramList[PREDIV].empty()) );
    for (size_t i = 0; i < PI_NUM; ++i) {
        const VmtNodeList& v = getT()->_paramList[i];
        for (size_t j = 0, size = v.size(); j < size; ++j) {
            if ( i == LEN || i == LEN_N ) {
                for (size_t k = 0; k < v[j]->_bit; ++k)
                    file << " " << v[j]->_bname << "_" << k
                         << "=" << v[j]->_bname << "_" << k;
                file << BLIFIndent;
            }
            else
                file << " " << v[j]->_bname << "=" << v[j]->_bname;
        }
        if ( i != LEN && i != LEN_N && !v.empty() )
            file << BLIFIndent;
    }
    file << " " << getT()->_bname << "_0" << "=Tori\n"
         << "\n# overflow condidion for LEN , LEN_N\n";
    
    assert( (_piList[LEN].size() == _piList[LEN_N].size()) );
    if ( !_piList[LEN].empty() ) {
        sizeMap[M_EQ].insert(LVAR_BIT_NUM);
        file << "\n.names const1\n1";
    }
    cout << "numOfLEN=" << _piList[LEN].size() << endl;

    for (size_t i = 0, size = _piList[LEN].size(); i < size; ++i) {
        const size_t bit = LVAR_BIT_NUM;
        file << "\n.subckt " << bit << "bEQ" << BLIFIndent;
        writeFAList(bit,"a",_piList[LEN][i],file);
        writeFAList(bit,"b","const1",file);
        file << " out=e" << i << "_0";
    }
    
    for (size_t i = 0, size = _piList[LEN_N].size(); i < size; ++i) {
        const size_t bit = LVAR_BIT_NUM;
        file << "\n.subckt " << bit << "bEQ";
        writeFAList(bit,"a",_piList[LEN_N][i],file);
        writeFAList(bit,"b","const1",file);
        file << " out=e" << i << "_1";
    }
    for (size_t i = 0, size = _piList[LEN].size(); i < size; ++i) {
        file << "\n.names e" << i << "_0 e" << i << "_1 e" << i
             << "\n00 1"
             << "\n01 1"
             << "\n11 1";
    }
    file << "\n.names";
    for (size_t i = 0, size = _piList[LEN].size(); i < size; ++i)
        file << " e" << i;
    file << " Tori T\n"
         << string(_piList[LEN].size(),'1') << "1 1";

    assert( (getO()->_paramList[INPUT].empty()) );
    assert( (getO()->_paramList[EXIST].empty()) );
    assert( (getO()->_paramList[STATE_N].empty()) );
    assert( (getO()->_paramList[LEN_N].empty()) );
    file << "\n.subckt " << getO()->_bname << BLIFIndent;
    for (size_t i = 0; i < PI_NUM; ++i) {
        const VmtNodeList& v = getO()->_paramList[i];
        for (size_t j = 0, size = v.size(); j < size; ++j) {
            if ( i == LEN || i == PREDIV ) {
                for (size_t k = 0; k < v[j]->_bit; ++k)
                    file << " " << v[j]->_bname << "_" << k
                         << "=" << v[j]->_bname << "_" << k;
                file << BLIFIndent;
            }
            else
                file << " " << v[j]->_bname << "=" << v[j]->_bname;
        }
        if ( i != LEN && i !=PREDIV && !v.empty() )
            file << BLIFIndent;
    }
    file << " " << getO()->_bname << "_0=O";
    for (size_t i = 0, size = _piList[STATE].size(); i < size; ++i)
        file << "\n.latch " << _piList[STATE_N][i]->_bname 
             << " "         << _piList[STATE][i]->_bname
             << " 0";
    for (size_t i = 0, size = _piList[LEN].size(); i < size; ++i)
        for (size_t j = 0; j < _piList[LEN][i]->_bit; ++j)
            file << "\n.latch " << _piList[LEN_N][i]->_bname << "_" << j
                 << " "         << _piList[LEN][i]->_bname   << "_" << j
                 << " 0";
    file << "\n.latch K Z 1"
         << "\n.names T Z K"
         << "\n11 1"
         << "\n.names O Z Oisempty"
         << "\n11 1"
         << "\n.end\n"
         << "\n# imdList\n";
    // write submodels
    for (size_t i = 0, size = _imdList.size(); i < size; ++i) {
        if ( _imdList[i]->_flag == gflag ) {
            _imdList[i]->writeMODEL( file , sizeMap );
        }
    }
    file << "# predList\n";
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        if ( _predList[i]->_flag == gflag ) {
            cout << "predList no." << i << endl;
            _predList[i]->writeMODEL( file , sizeMap, 1 );
        }
    }
    
    file << "# modules\n";
    bool needUSFA = 0, needUHA = 0;
    if ( !sizeMap[M_SFA].empty() ) needUSFA = 1;
    if ( !sizeMap[M_EQ].empty()  ) 
    if ( !sizeMap[M_INC1].empty()) needUHA  = 1;


    cout << "needUSFA=" << needUSFA << " needUHA=" << needUHA << endl;
    cout << "SFA :";
    for (set<size_t>::iterator it = sizeMap[M_SFA].begin(); it != sizeMap[M_SFA].end(); ++it)
        cout << " " << *it;
    cout << "\nEQ :";
    for (set<size_t>::iterator it = sizeMap[M_EQ].begin(); it != sizeMap[M_EQ].end(); ++it)
        cout << " " << *it;
    cout << "\nINC1 :";
    for (set<size_t>::iterator it = sizeMap[M_INC1].begin(); it != sizeMap[M_INC1].end(); ++it)
        cout << " " << *it;
    cout << endl;

    if ( needUSFA ) writeUSFA(file);
    if ( needUHA  ) writeUHA(file);

    for (set<size_t>::iterator it = sizeMap[M_SFA].begin(); it != sizeMap[M_SFA].end(); ++it)
        writeSFA(*it,file);
    for (set<size_t>::iterator it = sizeMap[M_EQ].begin(); it != sizeMap[M_EQ].end(); ++it)
        writeEQ(*it,file);
    for (set<size_t>::iterator it = sizeMap[M_INC1].begin(); it != sizeMap[M_INC1].end(); ++it)
        writeINC1(*it,file);
    
    file.close();
}

void Aut::writeFAList(const size_t& bitNum, const string& lhs, const string& rhs, ofstream& file)
{
    // rhs is CONST0 or CONST1
    for (int i = bitNum - 1; i >= 0; --i)
        file << " " << lhs << i << "=" << rhs;
    file << BLIFIndent;
}

void Aut::writeFAList(const size_t& bitNum, const string& lhs, VmtNode* rhsNode, ofstream& file)
{
    // rhs no need to be padded
    for (int i = bitNum - 1; i >= 0; --i)
        file << " " << lhs << i << "=" << rhsNode->_bname << "_" << i;
    file << BLIFIndent;
}

void Aut::writeFAList(bool& tUsed,bool& fUsed,const size_t& bitNum, const string& lhs, VmtNode* rhsNode, ofstream& file)
{
    // rhs needed to be padded with CONST0 or CONST1
    for (int i = bitNum - 1; i >= 0; --i) {
        if ( i >= rhsNode->_bit ) {
            fUsed = 1;
            file << " " << lhs << i << "=const0";
        }
        else {
            if ( rhsNode->_type == NUM ){
                file << " " << lhs << i;
                if ( rhsNode->_bname[ rhsNode->_bit-i-1 ] == '1') {tUsed = 1; file << "=const1";}
                else                                              {fUsed = 1; file << "=const0";}
            }
            else {
                file << " " << lhs << i << "=" << rhsNode->_bname << "_" << i;
            }
        }
    }
    file << BLIFIndent;
}

void Aut::writeFAListARITH(bool& tUsed,bool& fUsed,const size_t& bitNum, const string& lhs, VmtNode* rhsNode, ofstream& file)
{
    if ( rhsNode->_type == NUM ) {
        for (int i = bitNum - 1; i >= 0; --i) {
            if ( i >= rhsNode->_bit ) {
                fUsed = 1;
                file << " " << lhs << i << "=const0";
            }
            else {
                file << " " << lhs << i;
                if ( rhsNode->_bname[ rhsNode->_bit-i-1 ] == '1') {tUsed = 1; file << "=const1";}
                else                                              {fUsed = 1; file << "=const0";}
            }
        }
    }
    else if ( rhsNode->_type == LEN ) {
        for (int i = bitNum - 1; i >= 0; --i) {
            if ( i >= rhsNode->_bit ) {
                fUsed = 1;
                file << " " << lhs << i << "=const0";
            }
            else {
                file << " " << lhs << i << "=" << rhsNode->_bname << "_" << i;
            }
        }
    }
    else {
        assert( (rhsNode->_type == PREDIV || Aut::isARITH(rhsNode->_type)) );
        #ifndef FORCE_ASSIGNED
        file << " " << lhs << bitNum - 1 << "=" << rhsNode->_bname << "_" << bitNum - 2;
        for (int i = bitNum - 2; i >= 0; --i) {
            file << " " << lhs << i << "=" << rhsNode->_bname << "_" << i;
        }
        #else
        for (int i = bitNum - 1; i >= 0; --i)
            file << " " << lhs << i << "=" << rhsNode->_bname << "_" << i;
        #endif
    }
    file << BLIFIndent;
}

void Aut::writeUSFA(ofstream& file)
{
    file << ".model SFA\n"
         << ".inputs a b cin sign\n"
         << ".outputs s cout\n"
         << ".names b sign bs\n"
         << "01 1\n"
         << "10 1\n"
         << ".names bs cin k\n"
         << "01 1\n"
         << "10 1\n"
         << ".names a k s\n"
         << "01 1\n"
         << "10 1\n"
         << ".names a bs cin cout\n"
         << "11- 1\n"
         << "1-1 1\n"
         << "-11 1\n"
         << ".end\n";
}

void Aut::writeUHA(ofstream& file)
{
    file << ".model HA\n"
         << ".inputs a b\n"
         << ".outputs s cout\n"
         << ".names a b s\n"
         << "01 1\n"
         << "10 1\n"
         << ".names a b cout\n"
         << "11 1\n"
         << ".end\n";
}

void Aut::writeSFA(const size_t& bitNum, ofstream& file)
{
    file << ".model " << bitNum << "bSFA"
         << "\n.inputs";
    for (int i = bitNum - 1; i >= 0; --i) file << " a" << i;
    for (int i = bitNum - 1; i >= 0; --i) file << " b" << i;
    file << " sign"
         << "\n.outputs";
    for (int i = bitNum - 1; i >= 0; --i) file << " s" << i;
    file << " cout";

    file << "\n.subckt SFA a=a0 b=b0 cin=sign sign=sign s=s0 cout=c1";
    for (size_t i = 1; i < bitNum - 1; ++i)
        file << "\n.subckt SFA a=a" << i << " b=b" << i << " cin=c" << i 
             << " sign=sign s=s" << i << " cout=c" << i + 1;
    file << "\n.subckt SFA a=a" << bitNum - 1 << " b=b" << bitNum - 1 
         << " cin=c" << bitNum - 1 << " sign=sign s=s" << bitNum - 1 << " cout=cout"
         << "\n.end\n";
}

void Aut::writeEQ(const size_t& bitNum, ofstream& file)
{
    file << ".model " << bitNum << "bEQ"
         << "\n.inputs";
    for (int i = bitNum - 1; i >= 0; --i) file << " a" << i;
    for (int i = bitNum - 1; i >= 0; --i) file << " b" << i;
    file << "\n.outputs out";
    for (size_t i = 0; i < bitNum; ++i) {
        file << "\n.names a" << i << " b" << i << " e" << i
             << "\n00 1"
             << "\n11 1";
    }
    file << "\n.names";
    for (int i = bitNum - 1; i >= 0; --i) file << " e" << i;
    file << " out\n"
         << string(bitNum,'1') << " 1"
         << "\n.end\n";
}

void Aut::writeINC1(const size_t& bitNum, ofstream& file)
{
    file << ".model " << bitNum << "bINC1"
         << "\n.inputs";
    for (int i = bitNum - 1; i >= 0; --i) file << " a" << i;
    for (int i = bitNum - 1; i >= 0; --i) file << " b" << i;
    file << "\n.outputs out"
         << "\n.subckt " << bitNum << "bEQ";
    for (int i = bitNum - 1; i >= 0; --i) file << " a" << i << "=s" << i;
    for (int i = bitNum - 1; i >= 0; --i) file << " b" << i << "=b" << i;
    file << " out=out"
         << "\n.subckt HA a=a0 b=const1 s=s0 cout=c1";
    for (size_t i = 1; i < bitNum; ++i)
        file << "\n.subckt HA a=a" << i << " b=c" << i << " s=s" << i << " cout=c" << i + 1;
    file << "\n.names const1"
         << "\n1"
         << "\n.end\n";
}

}
