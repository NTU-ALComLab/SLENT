#include "smtnode.h"
#include "dtnode.h"
#include "dtMgr.h"

extern dt::DTMgr* dtmgr;
static const size_t& gflag = dtmgr->getGFlag(); 

namespace dt {

//-------------base class-----------

void SmtNode::print(const size_t& indent,size_t level) const
{
    cout << string( indent * level ,' ') << _name << " " << dtmgr->getSmtTypeString(_type) << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}
/*
void SmtNode::analyzeASCII() const
{
    bool nonascii = 0;
    if (_type == CONST_STRING)
        for (string::const_iterator it=_name.begin(); it!=_name.end(); ++it)
            if (*it < 0 || *it >= 128) nonascii = 1;
    if (nonascii) cout << "NON-ASCII=" << _name << endl;
    for (size_t i=0,size=_children.size(); i<size; ++i)
        _children[i]->analyzeASCII();
}

bool SmtNode::isVarStr()
{
    return (_type == VAR_STR) ? 1 : 0;
}

bool SmtNode::isConstStr()
{
    if (_name[0] == '\"') return true;
    else return false;
}

bool SmtNode::isReturnTypeStr()
{
    if (_name == "str.++") return true;
    else {
        Str2TypeMap::const_iterator it = typeMap.find(_name);
        if (it != typeMap.end()) {
            if (it->second == VAR_STRING)
                return true;
            else
                return false;
        }
        else 
            return false;
    }
}

bool SmtNode::isStrComparison()
{
    assert((_type == BOOL_EQ || _type == BOOL_NEQ));
    const Type& ltype = _children[0]->getType();
    const Type& rtype = _children[1]->getType();
    if (ltype == VAR_STRING || ltype == CONST_STRING || rtype == VAR_STRING || rtype == CONST_STRING)
        return 1;
    else
        return 0;
}

void SmtNode::writeCVC4PredVar()
{
    assert((_type == VAR_BOOL || _type == VAR_INT));
    Str2TypeMap& dgTypeMap  = dg->getTypeMap();
    vector<string>& cvc4PredList = dg->getCVC4PredList();
    vector<string>& s3PredList   = dg->getS3PredList();
    set<string>& bvPredSet  = dg->getBVPredSet();
    set<string>& ivPredSet  = dg->getIVPredSet();
    set<string>::iterator kt;
    if (_type == VAR_BOOL) {
        kt = bvPredSet.find(_name);
        if (kt != bvPredSet.end()) return;
        else bvPredSet.insert(_name);
    }
    else {
        kt = ivPredSet.find(_name);
        if (kt != ivPredSet.end()) return;
        else ivPredSet.insert(_name);
    }

    Str2TypeMap::iterator sit = dgTypeMap.find(_name);
    if (sit == dgTypeMap.end()) {
        dgTypeMap.insert(Str2Type(_name,_type));
    }
    else {
        //FIXME
    }
    
    Str2SmtNodeListMap::iterator it = ptnodeListMap.find(_name);
    assert((it != ptnodeListMap.end()));
    //if (it != ptnodeListMap.end()) {
    SmtNodeList& ptnodeList = it->second;
    for (SmtNodeList::iterator jt=ptnodeList.begin(); jt!=ptnodeList.end(); ++jt) {
        if ((*jt)->_flag != gflag && ( (*jt)->_bflag == dg->getBFlag() || (*jt)->_level == 1 ) ) {
            (*jt)->_flag = gflag;
            string s = "(assert";
            (*jt)->writeCVC4PredRoot(s);
            // check if this predicate including invalid operator
            if (s[0] == 'd' && s[1] == 'i' && s[2] == 'v') continue;
            s += ")";
            cvc4PredList.push_back(s);
            s3PredList.push_back(s);
        }
    }
    //}
}

void SmtNode::writeCVC4PredRoot(string& s)
{
    if (!_children.empty()) s += " (";
    else                    s += " ";
    if (_name == "!=") s += "not (=";
    else               s += _name;
    for (SmtNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        // eliminate predicate including div
        if (_name == "div") {
            s = "div";
        }
        const Type& type = (*it)->_type;
        if (type == VAR_INT || type == VAR_BOOL) {
            s += " " + (*it)->_name;
            (*it)->writeCVC4PredVar();
        }
        else if (type == CONST_BOOL || type == CONST_INT || type == CONST_STRING) {
            s += " " + (*it)->_name;
        }
        else if (type == VAR_STRING) {
            Str2DTNodeMap::iterator jt = dgMap.find((*it)->_name);
            assert((jt != dgMap.end()));
            s += " " + jt->second->findLeader()->getName();
        }
        else {
            (*it)->writeCVC4PredRoot(s);
        }
    }
    if (_name == "!=") s += ")";
    if (!_children.empty()) s += ")";
}

void SmtNode::addChild(SmtNode* n)
{
    _children.push_back(n);
}

const SmtNodeList& SmtNode::getChildren() const
{
    return _children;
}
*/
void SmtNode::convertOr2And()
{
    for (SmtNodeList::iterator it = _children.begin(); it != _children.end(); ++it) {
        if ( (*it)->_name == "or" ) {
            SmtNode* notNode = new SmtNotNode();
            SmtNode* andNode = new SmtAndNode();
            
            for (size_t i = 0, size = (*it)->_children.size(); i < size; ++i) {
                SmtNode* notn = new SmtNotNode();
                notn->addChild((*it)->_children[i]);
                andNode->addChild(notn);
            }
            notNode->addChild(andNode);
            _children.erase(it);
            _children.insert(it,notNode);
        }
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->convertOr2And();
}

void SmtNode::mergeNotEquivalence()
{
    for (SmtNodeList::iterator it = _children.begin(); it != _children.end(); ++it) {
        if ( (*it)->_name == "not" ) {
            assert( ((*it)->_children.size() == 1) );
            SmtNode* n  = 0;
            SmtNode* gs = (*it)->_children[0];
            const string& gsName = gs->_name;
            if (gsName == "ite") {
                assert( (gs->_children.size() == 3) );
                n = new SmtIteNode();
                n->addChild(gs->_children[0]);
                for (size_t i = 1; i < 3; ++i) {
                    SmtNode* notNode = new SmtNotNode();
                    notNode->addChild(gs->_children[i]);
                    n->addChild(notNode);
                }
            }
            else if (gsName == "=" || gsName == "str.in.re" || gsName == "str.contains" || gsName == "str.prefixof" || gsName == "str.suffixof") {
                gs->setNeg(1);
                n = gs;
            }
            else {
                assert( (gsName == "not") );
                assert( (gs->_children.size() == 1) );
                n = gs->_children[0];
            }
            _children.erase(it);
            _children.insert(it,n);
        }
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->mergeNotEquivalence();
}

///////////////////////////////// Build Dependency Tree //////////////////////////////////////////////////
DTNode* SmtNdoe::buildDT(Str2DTNodeMap& dtMap)
{
    _flag = gflag;
    return 0;
}

DTNode* SmtConstStrNode::buildDT(Str2DTNodeMap& dtMap)
{
    _flag = gflag;
    return new DTNode(dtmgr->getNewNodeName(),_name);
}

DTNode* SmtVarStrNode::buildDT(Str2DTNodeMap& dtMap)
{
    _flag = gflag;
    Str2DTNodeMap::iterator it = dtMap.find(_name);
    if (it != dgMap.end()) {
        return it->second->findLeader();
    }
    else {
        DTNode* n = new DTNode(_name);
        dtMap.insert(Str2DTNode(_name,n));
        return n;
    }
}

DTNode* SmtEqNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert((_children.size() == 2));
    _flag = gflag;
    DTNode* left  = _children[0]->buildDT(dtMap);
    DTNode* right = _children[1]->buildDT(dtMap);

    if (!left && !right) return 0;
    else if (left && !right) {
        const string&  LName = left->getName();
        const SmtType& RType = _children[1]->getType();
        if (LName == "str.len") {
            assert( ( !isComple() && (RType == CONST_INT || RType == VAR_INT) ) );
            left->addLengthVar(_children[1]);
        }
        else if  (LName == "=" || LName == "str.in.re") {
            left->addAssertion(Assertion(!isComple(),_children[1]));
        }
        else {
            cout << "[WARNING:SmtEqNode::buildDT] invalid left child name=" << LName << endl;
            assert((0));
        }
        return 0;
    }
    else if (!left && right) {
        const string&  RName = right->getName();
        const SmtType& LType = _children[0]->getType();
        if (RName == "str.len") {
            assert( ( !isComple() && (LType == CONST_INT || LType == VAR_INT) ) );
            right->addLengthVar(_children[0]);
        }
        else if  (RName == "=" || RName == "str.in.re") {
            right->addAssertion(Assertion(!isComple(),_children[0]));
        }
        else {
            cout << "[WARNING:SmtEqNode::buildDT] invalid right child name=" << RName << endl;
            assert((0));
        }
        return 0;
    }
    else {
        DTNode* cp_left  = new DTNode(dtmgr->getNewNodeName(),left);
        DTNode* cp_right = new DTNode(dtmgr->getNewNodeName(),right);
        left->clearChildren();
        right->clearChildren();
        if (left->isVar()) {
            right->setLeader(left);
            left->setType(INTER);
            left->addChild(cp_left);
            left->addChild(cp_right);
            return left;
        }
        else if(right->isVar()) {
            left->setLeader(right);
            right->setType(INTER);
            right->addChild(cp_left);
            right->addChild(cp_right);
            return right; 
        }
        else {
            cout << "[WARNING:SmtEqNode::buildDT] \"=\" two operands are not VAR_STR"
                 << " n1=" << left->getName() << " type=" << dtmgr->getSmtTypeString(left->getType())
                 << " n2=" << right->getName() << " type=" << dtmgr->getSmtTypeString(right->getType())
                 << endl;
            assert(0);
        }
    }
}

///////////////////////////////// Bool Type //////////////////////////////////////////////////
DTNode* SmtAndNode::buildDT(Str2DTNodeMap& dtMap)
{
    _flag = gflag;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildDT(dtMap);
    return 0;
}

DTNode* SmtOrNode::buildDT(Str2DTNodeMap& dtMap)
{
    _flag = gflag;
    _children[_branch]->buildDT(dtMap);
    return 0;
}

DTNode* SmtIteNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert((_children.size() == 3));
    _flag = gflag;
    _children[_branch]->buildDT(dtMap);
    return 0;
}

///////////////////////////////// String Type //////////////////////////////////////////////////
DTNode* SmtStrNode::buildDT(Str2DTNodeMap dtMap)
{
    _flag = gflag;
    DTNode* n = new DTNode(dtmgr->getNewNodeName(),_dtype);
    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        DTNode* cur = _children[i]->buildDT(dtMap);
        assert(cur);
        cur->setNotSink();
        n->addChild(cur);
    }
    return n;
}

DTNode* SmtStrToReNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert((_children.size() == 1));
    _flag = gflag;
    return _children[0]->buildDT(dtMap);
}

DTNode* SmtStrInReNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert((_children.size() == 2));
    _flag = gflag;
    DTNode* left  = _children[0]->buildDT(dtMap);
    DTNode* right = _children[1]->buildDT(dtMap);
    
    assert((left && right));

    if (isComple()) {
        DTNode* cp_left = new DTNode(dtmgr->getNewNodeName(),left);
        DTNode* comple  = new DTNode(dtmgr->getNewNodeName(),COMPLE);
        right->setNotSink();
        comple->addChild(right);
        left->setType(INTER);
        left->clearChildren();
        left->addChild(cp_left);
        left->addChild(comple);
    }
    else {
        DTNode* cp_left = new DTNode(dtmgr->getNewNodeName(),left);
        right->setNotSink();
        left->setType(INTER);
        left->clearChildren();
        left->addChild(cp_left);
        left->addChild(right);        
    }

    return left;
}

DTNode* SmtStrLenNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert((_children.size() == 1));
    _flag = gflag;
    DTNode* n = new DTNode(dtmgr->getNewNodeName(),TRKLEN);
    n->addChild(_children[0]->buildDT(dtMap));
    return n;
}

DTNode* SmtStrContainsNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert( (_children.size() == 2) );
    _flag = gflag;
    DTNode* left  = _children[0]->buildDT(dtMap);
    DTNode* right = _children[1]->buildDT(dtMap);

    if (isComple()) {
        assert( (right->isConstLEAF()) );
        DTNode* cp_left = new DTNode(left);
        DTNode* comple  = new DTNode(COMPLE);
        right->setNotSink();
        right->setRegex( ".*" + right->getRegex() + ".*" );
        comple->addChild(right);
        left->setType(CONTAINS);
        left->clearChildren();
        left->addChild(cp_left);
        left->addChild(comple);
    }
    else {
        DTNode* cp_left = new DTNode(left);
        right->setNotSink();
        left->setType(CONTAINS);
        left->clearChildren();
        left->addChild(cp_left);
        left->addChild(right);
    }
    
    return left;
}

DTNode* SmtStrPrefixofNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert( (_children.size() == 2) );
    _flag = gflag;
    DTNode* left  = _children[0]->buildDT(dtMap);
    DTNode* right = _children[1]->buildDT(dtMap);

    if (isComple()) {
        assert( (left->isConstLEAF()) );
        DTNode* cp_right = new DTNode(right);
        DTNode* comple   = new DTNode(COMPLE);
        left->setNotSink();
        left->setRegex( left->getRegex() + ".*" );
        comple->addChild(left);
        right->setType(PREFIXOF);
        right->clearChildren();
        right->addChild(comple);
        right->addChild(cp_right);
    }
    else {
        DTNode* cp_right = new DTNode(right);
        left->setNotSink();
        right->setType(PREFIXOF);
        right->clearChildren();
        right->addChild(left);
        right->addChild(cp_right);
    }
    
    return right;
}

DTNode* SmtStrSuffixofNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert( (_children.size() == 2) );
    _flag = gflag;
    DTNode* left  = _children[0]->buildDT(dtMap);
    DTNode* right = _children[1]->buildDT(dtMap);

    if (isComple()) {
        assert( (left->isConstLEAF()) );
        DTNode* cp_right = new DTNode(right);
        DTNode* comple   = new DTNode(COMPLE);
        left->setNotSink();
        left->setRegex( ".*" + left->getRegex() );
        comple->addChild(left);
        right->setType(SUFFIXOF);
        right->clearChildren();
        right->addChild(comple);
        right->addChild(cp_right);
    }
    else {
        DTNode* cp_right = new DTNode(right);
        left->setNotSink();
        right->setType(SUFFIXOF);
        right->clearChildren();
        right->addChild(left);
        right->addChild(cp_right);
    }
    
    return right;
}

DTNode* SmtStrSubstrNode::buildDT(Str2DTNodeMap& dtMap)
{
    assert( (_children.size() == 3) );
    _flag = gflag;
    DTNode* n1 = _children[0]->buildDT(dtMap);
    DTNode* i1 = new DTNode(_children[1]->getName(),LVAR);
    DTNode* i2 = new DTNode(_children[2]->getName(),LVAR);

    DTNode* cp_n1 = new DTNode(n1);
    n1->setType(SUBSTR);
    n1->clearChildren();
    n1->addChild(cp_n1);
    n1->addChild(i1);
    n1->addChild(i2);

    // add assertion
}

DTNode* SmtStrReplaceNode::buildDT(Str2DTNodeMap& dtMap)
{
    return 0;
}

DTNode* SmtStrIndexofNode::buildDT(Str2DTNodeMap& dtMap)
{
    return 0;
}

///////////////////////////////// Build SmtNodeList Map //////////////////////////////////////////////////
void SmtNode::buildPredMap(Str2SmtNodeListMap& m, SmtSmtNode* root) const
{
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildPredMap(m,root);
}

void SmtVarBoolNode::buildPredMap(Str2SmtNodeListMap& m, SmtNode* root) const
{
    Str2SmtNodeListMap::iterator it = m.find(_name);
    assert( (it != m.end()) );
    it->second.push_back(root);
}

void SmtVarIntNode::buildPredMap(Str2SmtNodeListMap& m, SmtNode* root) const
{
    Str2SmtNodeListMap::iterator it = m.find(_name);
    assert( (it != m.end()) );
    it->second.push_back(root);
}

void SmtEqNode::buildPredMap(Str2SmtNodeListMap& m, SmtNode* root) const
{
    assert( (_children.size() == 2) );
    const string&  LName = _children[0]->getName();
    const string&  RName = _children[1]->getName();
    const SmtType& LType = _children[0]->getType();
    const SmtType& RType = _children[1]->getType();
    if (LName == "str.len") {
        assert((RType == VAR_INT || RType == CONST_INT));
        return;
    }
    else if (RName == "str.len") {
        assert((LType == VAR_INT || LType == CONST_INT));
        return;
    }
    else if ( (LName == "=" || LName == "!=") && _children[0]->isStrComparison() ) {
        assert((RType == VAR_BOOL));
        return;
    }
    else if ( (RName == "=" || RName == "!=") && _children[1]->isStrComparison() ) {
        assert((LType == VAR_BOOL));
        return;
    }
    else {
        for (SmtNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
            (*it)->buildPredMap(m,root);
    }
}

///////////////////////////////// Build Branching List //////////////////////////////////////////////////
void SmtNode::buildBranchList(SmtNodeList& bList)
{
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildBranchList(bList);
}

void SmtOrNode::buildBranchList(SmtNodeList& bList)
{
    bList.push_back(this);
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildBranchList(bList);
}

void SmtIteNode::buildBranchList(SmtNodeList& bList)
{
    bList.push_back(this);
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildBranchList(bList);
}

///////////////////////////////// Mark Predicates //////////////////////////////////////////////////
void SmtNode::markPred(const Str2SmtNodeListMap& predMap)
{
    if (_flag == gflag) return;
    _flag = gflag;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->markPred(predMap);
}

void SmtVarBoolNode::markPred(const Str2SmtNodeListMap& predMap)
{
    if (_flag == gflag) return;
    _flag = gflag;
    Str2SmtNodeListMap::it = predMap.find(_name);
    assert( (it != predMap.end()) );
    for (size_t i = 0, size = it.second.size(); i < size; ++i)
        it.second[i]->markPred(predMap);
}

void SmtVarIntNode::markPred(const Str2SmtNodeListMap& predMap)
{
    if (_flag == gflag) return;
    _flag = gflag;    
    Str2SmtNodeListMap::it = predMap.find(_name);
    assert( (it != predMap.end()) );
    for (size_t i = 0, size = it.second.size(); i < size; ++i)
        it.second[i]->markPred(predMap);
}

///////////////////////////////// Write SMT File //////////////////////////////////////////////////
void SmtNode::writeSmtPredRoot(ofstream& outFile)
{
    if (_flag == _gflag) {
        if (_phase) {
            outFile << "(assert";
            writeSmtFile(outFile);
            outFile << ")\n";
        }
        else {
            outFile << "(assert (not";
            writeSmtFile(outFile);
            outFile << "))\n";
        }
        return;
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeSmtPredRoot(outFile);
}

void SmtNode::writeSmtFile(ofstream& outFile)
{
    if (!_children.empty()) outFile << " (";
    else                    outFile << " ";
    outFile << _name;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeSmtFile(outFile);
    if (!_children.empty()) outFile << ")";
}

void SmtEqNode::writeSmtFile(ofstream& outFile)
{
    if (!_children.empty()) outFile << " (";
    else                    outFile << " ";
    if (_comple)            outFile << "not (=";
    else                    outFile << "=";
    outFile << _name;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeSmtFile(outFile);
    if (!_children.empty()) outFile << ")";
    if (_comple)            outFile << ")";
}

///////////////////////////////// Write S3 File //////////////////////////////////////////////////
void SmtNode::writeS3PredRoot(ofstream& outFile)
{
    if (_flag == _gflag) {
        if (_phase) {
            outFile << "(assert";
            writeS3File(outFile);
            outFile << ")\n";
        }
        else {
            outFile << "(assert (not";
            writeS3File(outFile);
            outFile << "))\n";
        }
        return;
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeS3PredRoot(outFile);
}

void SmtNode::writeS3File(ofstream& outFile)
{
    if (!_children.empty()) outFile << " (";
    else                    outFile << " ";
    outFile << _name;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeS3File(outFile);
    if (!_children.empty()) outFile << ")";
}

void SmtEqNode::writeS3File(ofstream& outFile)
{
    if (!_children.empty()) outFile << " (";
    else                    outFile << " ";
    if (_comple)            outFile << "not (=";
    else                    outFile << "=";
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeS3File(outFile);
    if (!_children.empty()) outFile << ")";
    if (_comple)            outFile << ")";
}

///////////////////////////////// Write LAut File //////////////////////////////////////////////////
void SmtNode::writeLAutPredRoot(ofstream& outFile)
{
    if (_flag == _gflag) {
        if (_phase) {
            outFile << "(assert";
            writeLAutFile(outFile);
            outFile << ")\n";
        }
        else {
            outFile << "(assert (not";
            writeLAutFile(outFile);
            outFile << "))\n";
        }
        return;
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeLAutPredRoot(outFile);
}

void SmtNode::writeLAutFile(ofstream& outFile)
{
    if (!_children.empty()) outFile << " (";
    else                    outFile << " ";
    outFile << _name;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeLAutFile(outFile);
    if (!_children.empty()) outFile << ")";
}

void SmtEqNode::writeLAutFile(ofstream& outFile)
{
    if (!_children.empty()) outFile << " (";
    else                    outFile << " ";
    if (_comple)            outFile << "not (=";
    else                    outFile << "=";
    outFile << _name;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeLAutFile(outFile);
    if (!_children.empty()) outFile << ")";
    if (_comple)            outFile << ")";
}

}
