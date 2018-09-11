#ifndef _SMTNODE_H_
#define _SMTNODE_H_
#include "typedef.h"

namespace dt {

class SmtNode {
    friend class DTMgr;
    public :
        SmtNode (const string& name, const SmtType& type): _name(name), _type(type), _idx(0), _flag(0), _neg(0) {}
        
        // Print Function
        void           print(const size_t&,size_t) const ;
        // Access Function
        const string&  getName()const{return _name;}
        const SmtType& getType()const{return _type;}
        const size_t&  getFlag()const{return _flag;}
        const bool&    isNeg() {return _neg;}
        // Modify Function 
        void addChild(SmtNode*);
        void setName(const string& name) {_name = name;}
        void setFlag(const size_t& flag) {_flag=flag;}
        void setNeg(const bool& neg) { _neg = neg; }
        // Conditional Fcuntion
        bool isReturnTypeStr();
        bool isVarStr();
        bool isConstStr();
        bool isStrComparison();

        void convertOr2And();
        void mergeNotEquivalence();
        void writeCVC4PredVar();
        void writeCVC4PredRoot(string&);
        void analyzeASCII() const;
        virtual DTNode* buildDT(Str2DTNodeMap&);
        virtual void buildPredMap(Str2SmtNodeListMap&, SmtNode*) const;
        virtual void markPred(const Str2SmtNodeListMap&);
        
        // Branching
        virtual size_t getBeginBranchIdx();
        virtual size_t getEndBranchIdx();
        virtual const size_t& getBranchIdx();
        virtual void   setBranchIdx(const size_t& idx);
    protected:
        string          _name;
        SmtType         _type;
        size_t          _idx;
        size_t          _flag;
        size_t          _branch;
        bool            _neg;
        SmtNodeList     _children;
};

class SmtConstBoolNode : public SmtNode {
    public :
        SmtConstBoolNode(const string& name) : SmtNode(name,CONST_BOOL) {}
};

class SmtConstIntNode  : public SmtNode {
    public :
        SmtConstIntNode(const string& name) : SmtNode(name,CONST_INT) {}
};

class SmtConstStrNode  : public SmtNode {
    public :
        SmtConstStrNode(const string& name) : SmtNode(name,CONST_STR) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtVarBoolNode   : public SmtNode {
    public :
        SmtVarBoolNode(const string& name) : SmtNode(name,VAR_BOOL) {}
        void buildPredMap(SmtNode*);
        void markPred(const Str2SmtNodeListMap&);
};

class SmtVarIntNode : public SmtNode {
    public :
        SmtVarIntNode(const string& name) : SmtNode(name,VAR_INT) {}
        void buildPredMap(SmtNode*);
        void markPred(const Str2SmtNodeListMap&);
};

class SmtVarStrNode : public SmtNode {
    public :
        SmtVarStrNode(const string& name) : SmtNode(name,VAR_STR) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtEqNode : public SmtNode {
    public :
        SmtEqNode() : SmtNode("=",EQ) {}
        DTNode* buildDT(Str2DTNodeMap&);
        void buildPredMap(SmtNode*);
};

class SmtBoolNode : public SmtNode {
    public :
        SmtBoolNode(const string& name, const SmtType& type) : SmtNode(name,type) {}
        virtual DTNode* buildDT(Str2DTNodeMap&);
};

class SmtIntNode : public SmtNode {
    public :
        SmtIntNode(const string& name, const SmtType& type) : SmtNode(name,type) {}
        virtual DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrNode : public SmtNode {
    public :
        SmtStrNode(const string& name, const SmtType& type, const DType& dtype) : SmtNode(name,type),_dtype(dtype) {}
        virtual DTNode* buildDT(Str2DTNodeMap&);
    private:
        DType _dtype;
};

/////////////////////////// Bool Type ////////////////////////////////

class SmtNotNode : public SmtBoolNode {
    public :
        SmtNotNode() : SmtBoolNode("not",NOT) {}
};

class SmtAndNode : public SmtBoolNode {
    public :
        SmtAndNode() : SmtBoolNode("and",AND) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtOrNode : public SmtBoolNode {
    public :
        SmtOrNode() : SmtBoolNode("or",OR) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtIteNode : public SmtBoolNode {
    public :
        SmtIteNode() : SmtBoolNode("ite",ITE) { }
        DTNode* buildDT(Str2DTNodeMap&);
        size_t getBeginBranchIdx() {return 1;}
        size_t getEndBranchIdx()   {return 3;}
        const size_t& getBranchIdx() {return _branch;}
        void   setBranchIdx(const size_t& idx) { _branch = idx;}
};
/*
/////////////////////////// Int Type ////////////////////////////////

class SmtLTNode : public SmtIntNode {
    public :
        SmtLTNode() : SmtIntNode("<",LT) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtLTOEQNode : public SmtIntNode {
    public :
        SmtLTOEQNode() : SmtIntNode("<=",LTOEQ) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtMTNode : public SmtIntNode {
    public :
        SmtMTNode() : SmtIntNode(">",MT) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtMTOEQNode : public SmtIntNode {
    public :
        SmtMTOEQNode() : SmtIntNode(">=",MTOEQ) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtPlusNode : public SmtIntNode {
    public :
        SmtPlusNode() : SmtIntNode("+",PLUS) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtMinusNode : public SmtIntNode {
    public :
        SmtMinusNode() : SmtIntNode("-",MINUS) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtNegNode : public SmtIntNode {
    public :
        SmtNegNode() : SmtIntNode("-",NEG) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtDivNode : public SmtIntNode {
    public :
        SmtDivNode() : SmtIntNode("div",DIV) {}
        DTNode* buildDT(Str2DTNodeMap&);
};
*/
/////////////////////////// String Type ////////////////////////////////
class SmtStrToReNode : public SmtStrNode {
    public :
        SmtStrToReNode() : SmtStrNode("str.to.re",STRTORE,OTHER) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrInReNode : public SmtStrNode {
    public :
        SmtStrInReNode() : SmtStrNode("str.in.re",STRINRE,OTHER){}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrLenNode : public SmtStrNode {
    public :
        SmtStrLenNode() : SmtStrNode("str.len",STRLEN,OTHER) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrConcateNode : public SmtStrNode {
    public :
        SmtStrConcateNode() : SmtStrNode("str.++",STRCONCATE,CONCATE) {}
};

class SmtStrContainsNode : public SmtStrNode {
    public :
        SmtStrContainsNode() : SmtStrNode("str.contains",STRCONTAINS,CONTAINS) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrPrefixofNode : public SmtStrNode {
    public :
        SmtStrPrefixofNode() : SmtStrNode("str.prefixof",STRPREFIXOF,PREFIXOF) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrSuffixofNode : public SmtStrNode {
    public :
        SmtStrSuffixofNode() : SmtStrNode("str.suffixof",STRSUFFIXOF,SUFFIXOF) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrSubstrNode : public SmtStrNode {
    public :
        SmtStrSubstrNode() : SmtStrNode("str.substr",STRSUBSTR,SUBSTR) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrReplaceNode : public SmtStrNode {
    public :
        SmtStrReplaceNode() : SmtStrNode("str.replace",STRREPLACE,REPLACE) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtStrIndexofNode : public SmtStrNode {
    public :
        SmtStrIndexofNode() : SmtStrNode("str.indexof",STRINDEXOF,TRKIDX) {}
        DTNode* buildDT(Str2DTNodeMap&);
};

class SmtReInterNode : public SmtStrNode {
    public :
        SmtReInterNode() : SmtStrNode("re.inter",REINTER,INTER) {}
};

class SmtReUnionNode : public SmtStrNode {
    public :
        SmtReUnionNode() : SmtStrNode("re.union",REUNION,UNION) {}
};

class SmtReConcateNode : public SmtStrNode {
    public :
        SmtReConcateNode() : SmtStrNode("re.++",RECONCATE,CONCATE) {}
};

};
#endif
