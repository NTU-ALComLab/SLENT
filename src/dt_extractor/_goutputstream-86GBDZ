#include "dtMgr.h"

extern dt::DTMgr* dtmgr;
static const size_t& gflag = dtmgr->getGFlag(); 

namespace dt {

/////////////////////// Static Member Function //////////////////////////////////
string DTMgr::getSmtTypeString(const SmtType& t)
{
    switch (t) {
        case CONST_BOOL : return "CONST_BOOL";
        case CONST_INT  : return "CONST_INT";
        case CONST_STR  : return "CONST_STR";
        case VAR_BOOL   : return "VAR_BOOL";
        case VAR_INT    : return "VAR_INT";
        case VAR_STR    : return "VAR_STR";
        case EQ         : return "EQ";
        // Bool Argument Type
        case NOT        : return "NOT";
        case AND        : return "AND";
        case OR         : return "OR";
        case ITE        : return "ITE";
        // Int Argument Type
        case LT         : return "LT";
        case LTOEQ      : return "LTOEQ";
        case MT         : return "MT";
        case MTOEQ      : return "MTOEQ";
        case PLUS       : return "PLUS";
        case MINUS      : return "MINUS";
        case NEG        : return "NEG";
        case DIV        : return "DIV";
        // String Argument Type
        case STRTORE    : return "STRTORE";
        case STRINRE    : return "STRINRE";
        case STRLEN     : return "STRLEN";
        case STRCONCATE : return "STRCONCATE";
        case STRREPLACE : return "STRREPLACE";
        // Regex Argument
        case REINTER    : return "REINTER";
        case REUNION    : return "REUNION";
        case RECONCATE  : return "RECONCATE";
        default         : return "INVALID_TYPE";
    }
}

string DTMgr::getDTypeString(const DType& t)
{
    switch (t) {
        case LEAF       : return "LEAF";
        case TRKLEN     : return "TRKLEN";
        case TRKSTR     : return "TRKSTR";
        case COMPLE     : return "COMPLE";
        case CONCATE    : return "CONCATE";
        case UNION      : return "UNION";
        case INTER      : return "INTER";
        case CONTAINS   : return "CONTAINS";
        case PREFIXOF   : return "PREFIXOF";
        case SUFFIXOF   : return "SUFFIXOF";
        case SUBSTR     : return "SUBSTR";
        case REPLACE    : return "REPLACE";
        case TRKIDX     : return "TRKIDX";
        case OTHER      : return "OTHER";
        default         : return "INVALID_TYPE";
    }
}

void DTMgr::initEscapeSet()
{
    // Not Escaping double quote "
    char escapeChar[] = { '!', '#', '$', '%', '&', '\'', '(', ')', '*', '+' , ',', '-', '.', 
                          '/', ':', ';', '<', '=', '>' , '?', '@', '[', '\\', ']', '^', '_', 
                          '`', '{', '|', '}', '~'};
    
    for (size_t i = 0; i < 31; ++i) escapeSet.insert(escapeChar[i]);
}

string DTMgr::escape(string regex)
{
    //cout << "[DTMgr::escape] origin regex=" << regex << endl;
    if (regex == ".*" || regex == "") return "\"" + regex + "\"";
    else if (regex == "~(.*)") return "~\\(\".*\"\\)";
    else if (regex == "~()") return "~\\(\"\"\\)";
    bool isComple = 0;
    if (regex[0] == '~' && regex[1] == '(' && *(regex.rbegin()) == ')' ) {
        isComple = 1;
        regex = regex.substr(2,regex.size()-3);
    }
    //cout << "[DTMgr::escape] trimed regex=" << regex << endl;
    string escaped;
    for (size_t i = 0, size = regex.size(); i < size; ++i) {
        assert( (regex[i] != '"') );
        if (escapeSet.find(regex[i]) != escapeSet.end())
            escaped += '\\';
        escaped += regex[i];
    }
    //cout << "[DTMgr::escape] escaped regex=" << escaped << endl;
    if (isComple) return "~\\(\"" + escaped + "\"\\)";
    else          return "\"" + escaped + "\"";
}

/////////////////////// Read SMT File //////////////////////////////////
void DTMgr::readSmtFile(const char* fileName, const char* path)
{
    cout << ">> DTMgr::read::file =" << fileName << endl;
    
    ifstream file(fileName);
    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    
    _path = string(path);

    size_t lineCnt = 0,lCnt = 0,rCnt = 0;
    string line;
    vector<string> tokList;
    vector<vector<string> > contentList;
    while(getline(file,line)) {
        #ifndef _DTMGR_NDEBUG_
            cout << "lineCnt=" << ++lineCnt << " \"" << line << "\"" << endl;
        #endif
        for (size_t i = 0, j = 0, size = line.size(); j < size; ++j) {
            if (line[j] == '(') {
                ++lCnt;
                tokList.push_back("(");
                i = j + 1;
            }
            else if (line[j] == ' ' || line[j] == ')') {
                if (i != j) 
                    tokList.push_back(line.substr(i,j-i));
                if (line[j] == ')') {
                    ++rCnt;
                    tokList.push_back(")");
                }
                i = j + 1;
            }
        }
        if (lCnt == rCnt && !tokList.empty()) {
            #ifndef _DTMGR_NDEBUG_
                for (size_t i = 0,size = tokList.size(); i < size; ++i)
                    cout << tokList[i];
                cout << endl;
            #endif
            contentList.push_back(tokList);
            // reset counter and token list
            lCnt = 0;
            rCnt = 0;
            tokList.clear();
        }
    }
    file.close();

    // build leaves first, then build assertions
    for (size_t i = 0, size = contentList.size(); i < size; ++i) 
        if      (contentList[i][1] == "declare-fun"  ) declaration(contentList[i][2],contentList[i][5]);
        else if (contentList[i][1] == "declare-const") declaration(contentList[i][2],contentList[i][3]);
    for (size_t i = 0, size = contentList.size(); i < size; ++i) 
        if      (contentList[i][1] == "assert"       ) _root->addChild(assertion(contentList[i],2,contentList[i].size()-2));
    
    _root->convertOr2And();
    _root->mergeNotEquivalence();
    buildPredMap();
    
    _root->print(3,0);
    
    /*
    _pt->setLevel();
    _pt->buildPredMap();
    printTypeMap();
    _pt->printStr2SmtNodeListMap();
    _pt->print();
    */
}

void DTMgr::declaration(const string& name, const string& type)
{
    if (type == "Bool") {
        SmtNode* n = new SmtVarBoolNode(name);
        _smtLeafMap.insert(Str2SmtNode(name,n));
        _piList[0].push_back(n);
    }
    else if (type == "Int") {
        SmtNode* n = new SmtVarIntNode(name);
        _smtLeafMap.insert(Str2SmtNode(name,n));
        _piList[1].push_back(n);
    }
    else {
        assert(type == "String");
        SmtNode* n = new SmtVarStrNode(name);
        _smtLeafMap.insert(Str2SmtNode(name,n));
        _piList[2].push_back(n);
    }
}

SmtNode* DTMgr::assertion(const vector<string>& tokList,size_t bpos, size_t epos)
{
    SmtNode* newNode = 0;
    
    if (bpos != epos) newNode = buildSmtNode(tokList[bpos+1]);
    else              newNode = buildSmtNode(tokList[bpos]);
    
    size_t l_lCnt = 0, l_rCnt = 0, l_bpos = bpos + 2;
    for (size_t i = bpos+2; i < epos; ++i) {
        
        const string& cstr = tokList[i];
        
        if      (cstr == "(") ++l_lCnt;
        else if (cstr == ")") ++l_rCnt;
        
        if (l_lCnt == l_rCnt) {
            newNode->addChild(assertion(tokList,l_bpos,i));
            l_bpos = i + 1;
            l_lCnt = 0;
            l_rCnt = 0;
        }
    }
    return newNode;
}

SmtNode* DTMgr::buildSmtNode(const string& name)
{
    if      (name == "="           ) return new SmtEqNode();
    // Bool Argument Type
    else if (name == "not"         ) return new SmtNotNode();
    else if (name == "and"         ) return new SmtAndNode();
    else if (name == "or"          ) return new SmtOrNode();
    else if (name == "ite"         ) return new SmtIteNode();
    // Int Argument Type
    else if (name == "<"           ) return new SmtIntNode(name,LT);
    else if (name == "<="          ) return new SmtIntNode(name,LTOEQ);
    else if (name == ">"           ) return new SmtIntNode(name,MT);
    else if (name == ">="          ) return new SmtIntNode(name,MTOEQ);
    else if (name == "+"           ) return new SmtIntNode(name,PLUS);
    else if (name == "-"           ) return new SmtIntNode(name,MINUS); // default MINUS
    else if (name == "div"         ) return new SmtIntNode(name,DIV);
    // String Argument Type
    else if (name == "str.to.re"   ) return new SmtStrToReNode();
    else if (name == "str.in.re"   ) return new SmtStrInReNode();
    else if (name == "str.len"     ) return new SmtStrLenNode();
    else if (name == "str.++"      ) return new SmtStrConcateNode();
    else if (name == "str.contains") return new SmtStrContainsNode();
    else if (name == "str.prefixof") return new SmtStrPrefixofNode();
    else if (name == "str.suffixof") return new SmtStrSuffixofNode();
    else if (name == "str.substr"  ) return new SmtStrSubstrNode();
    else if (name == "str.replace" ) return new SmtStrReplaceNode();
    else if (name == "str.indexof" ) return new SmtStrIndexofNode();
    else if (name == "re.inter"    ) return new SmtReInterNode();
    else if (name == "re.union"    ) return new SmtReUnionNode();
    else if (name == "re.++"       ) return new SmtReConcateNode();
    else {
        Str2SmtNodeMap::iterator it = _smtLeafMap.find(name);
        if (it != _smtLeafMap.end())            return it->second;
        else if (name[0] == '\"') {
            assert( (name.at(name.size()-1)=='\"') );
            return new SmtConstStrNode( name.substr(1,name.size()-2) );
        }                  
        else if (name == "true" || name == "false") return new SmtConstBoolNode(name);    
        else if (isNumber(name))                    return new SmtConstIntNode(name);
        else {
            cout << "[WARNING:DTMgr::buildSmtNode] invalid string=" << name << " at path=" << _path << endl;
            assert(0);
        }
    }
}

void DTMgr::buildPredMap()
{
    // For Bool and Int Variables
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0, size = _piList[i].size(); j < size; ++j)
            _predMap.insert(Str2SmtNodeList(_piList[i][j]->getName(),SmtNodeList()));
    
    SmtNodeQueue q;
    q.push(_root);
    while (!q.empty()) {
        SmtNode* cur = q.front();
        q.pop();
        if ( cur->getName() == "and" || cur->getName() == "assert" ) {
        for (SmtNodeList::iterator it = cur->_children.begin(); it != cur->_children.end(); ++it) {
            if ( (*it)->getName() == "ite" ) {
                for (size_t i = (*it)->getBeginBranchIdx(); i < (*it)->getEndBranchIdx(); ++i)
                    q.push((*it)->_children[i]);
            }
            else
                (*it)->buildPredMap(_predMap,*it);
        }
    }
}

/////////////////////// Dependency Tree Extraction //////////////////////////////////

void DTMgr::extractDT()
{
    // Build Branch List
    SmtNodeQueue q;
    q.push(_root);
    while (!q.empty()) {
        SmtNode* cur = q.front();
        q.pop();
        if ( cur->getName() == "ite" )
            _branchList.push_back(cur);
        for (size_t i = 0, size = cur->_children.size(); i < size; ++i)
            q.push(cur->_children[i]);
    }
    
    size_t branchCnt = 0;
    branching(0, _branchList.size(), branchCnt);
}
/*
def rec(v,i,n,bound) :
    if i == n : 
        print v
        del v[-1]
        return
    for j in range(bound[i]) :
        v.append(j)
        rec(v,i+1,n,bound)
    if i > 0 :
        del v[-1]

rec([],0,3,[1,2,3])
*/

void DTMgr::branching(size_t level, const size_t& totLevel, size_t& branchCnt)
{
    if (level == totLevel) {
        buildAndWriteDT(branchCnt++);
        return;
    }
    assert( (_branchList[level]->getName() == "ite") );
    for (size_t i = _branchList[level]->getBeginBranchIdx(); i < _branchList[level]->getEndBranchIdx(); ++i) {
        _branchList[level]->setBranchIdx(i);
        branching(level+1,totLevel,branchCnt);
    }
}

void DTMgr::buildAndWriteDT(const size_t& branchCnt)
{
    // Build Dependency Trees (probably graphs)
    Str2DTNodeMap dtMap;
    ++_gflag;
    _root->buildDT(dtMap);

    // Collect Sink Node of each Dependency Tree
    ++_gflag;
    DTNodeList sinkList;
    for (Str2DTNodeMap::iterator it = dtMap.begin(); it != dtMap.end(); ++it) {
        DTNode* leader = it->second->findLeader();
        if (leader->isSink() && leader->getFlag() != _gflag) {
            leader->setFlag(_gflag);
            sinkList.push_back(leader);
        }
    }

    for (size_t i = 0, size = sinkList.size(); i < size; ++i) {
        
        // Merge Dependency Tree and Check Tree Property and
        // Collect String Variables Used in Dependency Tree
        ++_gflag;
        DTNodeList svList;
        sinkList[i]->merge();
        sinkList[i]->collect(svList);

        // Mark Associated Bool/Int Predicates
        ++_gflag;
        sinkList[i]->markPred(_predMap);
        for (SmtNodeList::iterator it = _branchList.begin(); it != _branchList.end(); ++it)
            if ((*it)->getName() == "ite") {
                if ((*it)->getBranchIdx() == 1)
                    (*it)->_children[0]->setNeg(0);
                else                
                    (*it)->_children[0]->setNeg(1);  
                (*it)->_children[0]->markPred(_predMap);                  
            }

        // Create Branch Directory
        string bpath = _path + "/barnch_" + itos(branchCnt+1) + "/" + itos(i+1) + "/";
        system(("mkdir -p " + bpath).c_str());

        // Write Output in Different Formats
        writeSmtFile ( bpath + "sink.smt2" , sinkList[i], svList );
        writeS3File  ( bpath + "sink.s3"   , sinkList[i], svList );
        writeLAutFile( bpath + "laut"      , sinkList[i], svList );
    }

}

void DTMgr::writeSmtFile(const string& fileName, DTNode* sink, const DTNodeList& svList)
{
    #ifndef _DTMGR_NDEBUG_
        splitLine("DTMgr::writeSmtFile");
    #endif

    ofstream outFile(fileName.c_str());

    outFile << "(set-logic QF_S)" << endl;

    // Write Variable Declaration
    for (size_t i = 0, size = _piList[0].size(); i < size; ++i)
        if (_piList[0][i]->getFlag() == _gflag)
            outFile << "(declare-fun " << _piList[0][i]->getName() << " () Bool)\n";
    for (size_t i = 0, size = _piList[1].size(); i < size; ++i)
        if (_piList[1][i]->getFlag() == _gflag)
            outFile << "(declare-fun " << _piList[1][i]->getName() << " () Int)\n";        
    for (size_t i = 0, size = svList.size(); i < size; ++i)
        outFile << "(declare-fun " << svList[i]->getName() << " () String)\n";

    // Write Associated Bool/Int Predicates
    _root->writeSmtPredRoot(outFile);

    // Write String Predicates
    sink->writeSmtFile(outFile);

    outFile << "(check-sat)";
    outFile.close();
}

void DTMgr::writeS3File(const string& fileName, DTNode* sink, const DTNodeList& svList)
{
    #ifndef _DTMGR_NDEBUG_
        splitLine("DTMgr::writeS3File");
    #endif

    ofstream outFile(fileName.c_str());

    // Write Variable Declaration
    for (size_t i = 0, size = _piList[0].size(); i < size; ++i)
        if (_piList[0][i]->getFlag() == _gflag)
            outFile << "(declare-variable " << _piList[0][i]->getName() << " Bool)\n";
    for (size_t i = 0, size = _piList[1].size(); i < size; ++i)
        if (_piList[1][i]->getFlag() == _gflag)
            outFile << "(declare-variable " << _piList[1][i]->getName() << " Int)\n";        
    for (size_t i = 0, size = svList.size(); i < size; ++i)
        outFile << "(declare-variable " << svList[i]->getName() << " String)\n";

    // Write Associated Bool/Int Predicates
    _root->writeS3PredRoot(outFile);
    
    // Write String Predicates
    sink->writeS3File(outFile);

    outFile << "(check-sat)";
    outFile.close();
}

void DTMgr::writeLAutFile(const string& fileName, DTNode* sink, const DTNodeList& svList)
{
    #ifndef _DTMGR_NDEBUG_
        splitLine("DTMgr::writeLAutFile");
    #endif

    ofstream outFile(fileName.c_str());

    // Write Leaf Automaton Regex
    for (size_t i = 0, size = svList.size(); i < size; ++i) {
        DTNode* n = svList[i];
        if (n->getType() == LEAF)
            outFile << n->getName() << " " << kmgr->escape(n->getRegex()) << endl;
        else if (n->getType() == COMPLE) 
            outFile << n->getName() << " " << kmgr->escape("~("+n->getRegex()+")") << endl;
    }
    outFile << ";\n";

    // Write Variable Declaration
    for (size_t i = 0, size = _piList[0].size(); i < size; ++i)
        if (_piList[0][i]->getFlag() == _gflag)
            outFile << _piList[0][i]->getName() << " Bool\n";
    for (size_t i = 0, size = _piList[1].size(); i < size; ++i)
        if (_piList[1][i]->getFlag() == _gflag)
            outFile << _piList[1][i]->getName() << " Int\n";
    // Write Associated Bool/Int Predicates
    _root->writeLAutPredRoot(outFile);
    outFile << ";\n";

    // Write Dependency Tree
    sink->writeLAutFile(outFile);

    outFile.close();
}

}
