#ifndef _DTNODE_H_
#define _DTNODE_H_
#include "typedef.h"

namespace dt {

class DTNode {
    friend class DTMgr;
    public :
        // For Original String Variable
        DTNode (const string& name) {
            _name        = name;
            _type        = LEAF;
            _regex       = ".*";
            _isSink      = 1;
            _furtherRefine       = 1;
            _leader      = this;
            _flag        = 0;
        }
        // For Original Constant String
        DTNode (const string& name,const string& regex) {
            _name        = name;
            _type        = LEAF;
            _regex       = regex;
            _isSink      = 0;
            _furtherRefine       = 0;
            _leader      = this;
            _flag        = 0;
        }
        // For Intermediate Operations
        DTNode (const string& name,const DType& type) {
            _name        = name;
            _type        = type;
            _isSink      = 0;
            _furtherRefine       = 0;
            _leader      = this;
            _flag        = 0;
        }
        // Copy Constructor
        DTNode (const string& name,DTNode* n) {
            _name        = name;
            _type        = n->_type;
            _regex       = n->_regex;
            _isSink      = 0;
            _furtherRefine       = 0;
            _leader      = this;
            //_assertionList = n->_assertionList;
            _children    = n->_children;
            //FIXME
            _flag        = n->_flag;
        }
        // Print Function
        void           print (const size_t&,size_t)const;
        void           printLengthVarList() const;
        void           printAssertionList() const;
        // Access Function
        const bool&    isSink(){return _isSink;}
        const bool&    isFurtherRefine(){return _furtherRefine;}
        bool           isConstLEAF() {return (_type == LEAF && !_furtherRefine);}
        const DType&   getType(){return _type;}
        const string&  getName(){return _name;}
        const string&  getRegex()const {return _regex;}
        const size_t&  getFlag(){return _flag;}
        DTNode*        findLeader();
        // Modify Function
        void           setNotSink() {_isSink = 0;}
        void           setType(const DType& type) {_type = type;}
        void           setName(const string& name){_name = name;}
        void           setRegex(const string& regex){_regex = regex;}
        void           setLeader(DTNode* n);
        void           setFlag(const size_t& flag) {_flag=flag;}
        void           addChild(DTNode* n){_children.push_back(n);}
        void           clearChildren(){_children.clear();}
        void           addAssertion(const Assertion& n) {_assertionList.push_back(n);}
        // Dependency Tree Extraction
        void           markPred(const Str2SmtNodeListMap&);
        void           merge();
        void           collect(DTNodeList&) const;
        void           writeSmtFile (ofstream&) const;
        void           writeS3File (ofstream&) const;
        void           writeLAutFile (ofstream&) const;
    private:
        bool           _isSink;  // default 1
        bool           _furtherRefine; 
        size_t         _idx;
        DType          _type;       
        string         _name;    
        string         _regex;   
        DTNode*        _leader;
        DTNodeList     _children;
        AssertionList  _assertionList;
        size_t         _flag;
};

};
#endif
