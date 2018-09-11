#include "dtnode.h"
#include "dtMgr.h"

extern dt::DTMgr* dtmgr;
static const size_t& gflag = dtmgr->getGFlag(); 

namespace dt {

/////////////////////// Print Function //////////////////////////////////
void DTNode::print(const size_t& indent,size_t level) const
{
    cout << string(indent*level,' ') 
         << _name << " "
         << dtmgr->getDTypeString(_type);
    if (_type == CONST_STR) {
        #ifndef _NLOG_
        cout << " regex=" << _regex;
        #endif
    }
    #ifndef _NLOG_
    cout << endl;
    #endif
    for (DTNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->findLeader()->print(indent,level+1);
}

void DTNode::printLengthVarList() const
{
    #ifndef _NLOG_
        cout << "DTNode: name=" << _name << endl;
    #endif
    for (PTNodePairList::const_iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it)
        (*it).second->print(3,0);
    for (DTNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->findLeader()->printLengthVarList();
}

void DTNode::printAssertionList() const
{
    #ifndef _NLOG_
        cout << "DTNode: name=" << _name << endl;
    #endif
    for (PTNodePairList::const_iterator it=_assertionList.begin(); it!=_assertionList.end(); ++it)
        (*it).second->print(3,0);
    for (DTNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->findLeader()->printAssertionList();
}

/////////////////////// Disjoint Set //////////////////////////////////
void DTNode::setLeader(DTNode* n)
{
    _leader = n;
    for (SmtNodeList::iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it)
        n->_lengthVarList.push_back(*it);
    for (AssertionList::iterator it=_assertionList.begin(); it!=_assertionList.end(); ++it)
        n->_assertionList.push_back(*it);
}

DTNode* DTNode::findLeader()
{
    // Path Compression
    if (_leader != this) 
        _leader = _leader->findLeader();
    return _leader;
}

/////////////////////// Dependency Tree Extraction //////////////////////////////////
void DTNode::markPred(const Str2SmtNodeListMap& predMap)
{
    for (size_t i = 0, size = _lengthVarList.size(); i < size; ++i)
        _lengthVarList[i]->markPred(predMap);
    for (size_t i = 0, size = _assertionList.size(); i < size; ++i) {
        SmtNode* n = _assertionList[i].second;
        n->setPhase(_assertionList[i].first);
        n->markPred(predMap);
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->findLeader()->markPred(predMap);
}

void DTNode::merge()
{
    // Post-order Traversal
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->findLeader()->merge()
    
    if (_flag != gflag) _flag = gflag;
    else {
        // TODO
        cout << "[WARNING:DTNode::merge] this DG is NOT a Tree : at node " << _name << endl;
        assert(0);
    }
    
    if (_type == INTER) {
        size_t cnt = 0;
        for (DTNodeList::iterator it = _children.begin(); it != _children.end(); ++it) {
            if ((*it)->findLeader()->_type == LEAF && (*it)->findLeader()->_isVar) {
                assert(((*it)->findLeader()->_children.empty()));
                //FIXME
                _children.erase(it);
                --it;
            }
            else {
                ++cnt;
            }
        }
        if (cnt == 1) {
            DTNode* n = _children[0]->findLeader();
            if (n->_type == LEAF && !n->_isVar) {
                assert((n->_children.size()==0));
                _type  = LEAF;
                _isVar = 0; 
                _regex = n->_regex;
                _children.clear();
            }
            else {
                _type = n->_type;
                _children.clear();
                _children = n->_children;
            }
        }
        else if (cnt == 0) {
            _type  = LEAF;
            _isVar = 1;
            _regex = ".*";
            _children.clear();
        }
    }
    else if (_type == COMPLE) {
        assert((_children.size() == 1));
        SmtNode* n = _children[0];
        if (n->_type == CONCATE) {
            _regex = "";
            for (size_t i = 0, size = _children.size(); i < size; ++i)
                _regex += (*it)->findLeader()->_regex;
        }
        else {
            assert( (n->_type == LEAF && !n->_isVar) );
            _regex = n->_regex;
        }
    }  
}

void DTNode::collect(DTNodeList& svList) const
{
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->findLeader()->collect(svList);

    svList.push_back(findLeader());
    if (_type == COMPLE) return;
}

void DTNode::writeSmtFile(ofstream& outFile) const
{
    for (size_t i = 0, size = _lengthVarList.size(); i < size; ++i)
        outFile << "(assert (= (str.len " << _name << ") " << _lengthVarList[i]->getName() << "))\n";

    if (_type == COMPLE) {
        assert((_children.size() == 1));
        outFile << "(assert (not (str.in.re " + _name;
                << " (str.to.re " << _children[0]->findLeader()->_regex
                << "))))";
        return;
    }
    else if (_type == LEAF && !_isVar) {
        outFile << "(assert (= " << _name << " " << _regex << "))";
    }

    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->findLeader()->writeSmtFile(outFile);

    if (_type == CONCATE) {
        outFile << "(assert (= " << _name << " (str.++";
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            outFile << _children[i]->findLeader()->_name;
        outFile << ")))\n";
    }
    else if (_type == INTER) {
        assert((_children.size() == 2));
        outFile << "(assert (= " << _name << " " << _children[0]->findLeader()->_name << "))"
                << "(assert (= " << _children[0]->findLeader()->_name << " " 
                                 << _children[1]->findLeader()->_name << "))\n"; 
    }
}

void DTNode::writeS3File(ofstream& outFile) const
{
    for (size_t i = 0, size = _lengthVarList.size(); i < size; ++i)
        outFile << "(assert (= (Length " << _name << ") " << _lengthVarList[i]->getName() << "))\n";

    if (_type == COMPLE) {
        assert((_children.size() == 1));
        outFile << "(assert (not (In " + _name;
                << " " << _children[0]->findLeader()->_regex
                << ")))";
        return;
    }
    else if (_type == LEAF && !_isVar) {
        outFile << "(assert (= " << _name << " " << _regex << "))";
    }

    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->findLeader()->writeSmtFile(outFile);

    if (_type == CONCATE) {
        outFile << "(assert (= " << _name << " (Concat";
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            outFile << _children[i]->findLeader()->_name;
        outFile << ")))\n";
    }
    else if (_type == INTER) {
        assert((_children.size() == 2));
        outFile << "(assert (= " << _name << " " << _children[0]->findLeader()->_name << "))"
                << "(assert (= " << _children[0]->findLeader()->_name << " " 
                                 << _children[1]->findLeader()->_name << "))\n"; 
    }
}

void DTNode::writeLAutFile(ofstream& outFile) const
{

}

void DTNode::writeCVC4File()
{
    assert((_flag != gflag));
    _flag = gflag;
    Str2TypeMap& typeMap = dg->getTypeMap();
    vector<string>& cvc4StrList  = dg->getCVC4StrList();
    vector<string>& cvc4PredList = dg->getCVC4PredList();
    vector<string>& s3StrList    = dg->getS3StrList();
    vector<string>& s3PredList   = dg->getS3PredList();
    set<string>& bvStrSet = dg->getBVStrSet(); 
    set<string>& ivStrSet = dg->getIVStrSet(); 

    Str2TypeMap::iterator it = typeMap.find(_name);
    assert((it == typeMap.end()));
    assert((_type == AUT_CONCATE || _type == AUT_INTER || _type == dtAUT_COMPLE || _type == CONST_STR || _type == VAR_STRING));
    typeMap.insert(Str2Type(_name,VAR_STRING));
    
    if (_type == AUT_CONCATE) {
        cvc4StrList.push_back("(assert (= "+_name+" (str.++ "+_children[0]->findLeader()->_name+" "+_children[1]->findLeader()->_name+")))");
        s3StrList.push_back("(assert (= "+_name+" (Concat "+_children[0]->findLeader()->_name+" "+_children[1]->findLeader()->_name+")))");
    }
    else if (_type == AUT_INTER) {
        assert((_children.size() == 2));
        cvc4StrList.push_back("(assert (= "+_name+" "+_children[0]->findLeader()->_name+"))");
        cvc4StrList.push_back("(assert (= "+_children[0]->findLeader()->_name+" "+_children[1]->findLeader()->_name+"))");
        s3StrList.push_back("(assert (= "+_name+" "+_children[0]->findLeader()->_name+"))");
        s3StrList.push_back("(assert (= "+_children[0]->findLeader()->_name+" "+_children[1]->findLeader()->_name+"))");
    }
    else if (_type == AUT_COMPLE) {
        assert((_children.size() == 1));
        string cvc4str = "(assert (not (str.in.re " + _name;
        string s3str   = "(assert (not (In " + _name;
        _children[0]->findLeader()->writeCVC4LeafNode(cvc4str,s3str);
        cvc4str += ")))";
        s3str   += ")))";
        cvc4StrList.push_back(cvc4str);
        s3StrList.push_back(s3str);
    }    
    else if (_type == CONST_STR) {
        cvc4StrList.push_back("(assert (= "+_name+" "+_regex+"))");
        s3StrList.push_back("(assert (= "+_name+" "+_regex+"))");
    }
    else {
        assert((_type == VAR_STRING));
        
        //#ifndef _NLOG_
        //    cout << "[WARNING08]: invalid type=" << _type << " at DTNode=" << _name << endl;
        //#endif
        //cout << "[WARNING08]: invalid type=" << _type << " at DTNode=" << _name << endl;
        
    }

    for (PTNodePairList::iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it) {
        const Type& type = (*it).second->getType();
        if (type == CONST_INT) {
            cvc4PredList.push_back("(assert (= "+(*it).second->getName()+" (str.len "+_name+"))) ; len "+itos(_lengthVarCnt));
            s3PredList.push_back("(assert (= "+(*it).second->getName()+" (Length "+_name+"))) ; len "+itos(_lengthVarCnt));
            //cvc4PredList.push_back("(assert (= "+(*it).second->getName()+" (str.len "+_name+"))) ; cstrlen "+itos(_lengthVarCnt));
        }
        else {
            assert((type == VAR_INT));
            set<string>::iterator jt = ivStrSet.find((*it).second->getName());
            if (jt == ivStrSet.end()) {
                ivStrSet.insert((*it).second->getName());
                string s = "(assert";
                (*it).first->writeCVC4PredRoot(s);
                s += ") ; len "+itos(_lengthVarCnt);
                //s += ") ; vstrlen "+itos(_lengthVarCnt);
                cvc4PredList.push_back(s);
                s3PredList.push_back(s);
            }
            else {
                #ifndef _NLOG_
                    cout << "[WARNING:DTNode::writeCVC4File] same Int Variable assigned to 2 different String Variable" << endl;
                #endif 
                cout << "[WARNING:DTNode::writeCVC4File] same Int Variable assigned to 2 different String Variable" << endl;
            }
        }
    }
    for (PTNodePairList::iterator it=_assertionList.begin(); it!=_assertionList.end(); ++it) {
        assert(((*it).second->getType() == VAR_BOOL));
        set<string>::iterator jt = bvStrSet.find((*it).second->getName());
        if (jt == bvStrSet.end()) {
            bvStrSet.insert((*it).second->getName());
            // FIXME
            //string s = "(assert";
            //(*it).first->writeCVC4PredRoot(s);
            //s += ")";
            string s = "(assert " + (*it).second->getName() + ")";
            cvc4PredList.push_back(s);
            s3PredList.push_back(s);
            (*it).second->setFlag(gflag);
            (*it).second->writeCVC4PredVar();
            
            //string s = "(assert";
            //(*it).first->writeCVC4PredRoot(s);
            //s += ")";
            //cvc4PredList.push_back(s);
            
        }
        else {
            #ifndef _NLOG_
                cout << "[WARNING:DTNode::writeCVC4File] same Bool Variable implied by 2 different String Variable" << endl;
            #endif 
            cout << "[WARNING:DTNode::writeCVC4File] same Bool Variable implied by 2 different String Variable" << endl;
        }
    }
    if (_type == AUT_COMPLE) return;
    for (DTNodeList::iterator it=_children.begin();it!=_children.end();++it)
        (*it)->findLeader()->writeCVC4File();
}

void DTNode::writeCmdFile(ofstream& cmdFile,ofstream& autFile)
{
    assert((_flag != gflag)); // Not DAG
    _flag = gflag;
    if (_type == VAR_STRING || _type == CONST_STR || _type == AUT_COMPLE) {
        if (this == dg->getSinkNode()) {
            if (_lengthVarCnt != -1) {
                cmdFile << "addlen " << _name << " " << _lengthVarCnt << endl;
                // intermediate file for debug
                cmdFile << "write " << _name << "_l" << endl;
            }
            else {
                cmdFile << "read " << _name << endl;
                // intermediate file for debug
                cmdFile << "write " << _name << endl;
            }
        }
        else {
            if (_lengthVarCnt != -1) {
                cmdFile << "addlen " << _name << " " << _lengthVarCnt << endl;
                cmdFile << "write " << _name << "_l" << endl;
            }
        }
        autFile << _name << " " << dtmgr->escape(getRegex()) << endl;
        return;
    }
    for (DTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        (*it)->findLeader()->writeCmdFile(cmdFile,autFile);
    }
    if     (_type == AUT_CONCATE) cmdFile << "concate";  
    else if(_type == AUT_REPLACE) cmdFile << "replace";
    else if(_type == AUT_UNION)   cmdFile << "union";
    else if(_type == AUT_INTER)   cmdFile << "intersect";
    for (DTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        cmdFile << " " << (*it)->findLeader()->getName();
        if ((*it)->findLeader()->_lengthVarCnt != -1) cmdFile << "_l";
    }
    cmdFile << endl << "write " << _name << endl;
    if (_lengthVarCnt != -1) {
        cmdFile << "addlen " << _name << " " << _lengthVarCnt << endl;
        cmdFile << "write " << _name << "_l" << endl;
    }
}

}
