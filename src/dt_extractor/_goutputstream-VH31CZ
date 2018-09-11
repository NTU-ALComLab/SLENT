#ifndef _DTMGR_H_
#define _DTMGR_H_
#include "typedef.h"
#include "utility.h"
#include "smtnode.h"
#include "dtnode.h"

namespace dt {

class DTMgr{
    public :
        DTMgr(): _indent(3), _gflag(0) {
            _piList.assign(3,SmtNodeList());
            initEscapeSet();
        }
        
        // Static Member Function
        static string getSmtTypeString(const SmtType&);
        static string getDTypeString(const DType&);
        static string escape(string);
        // Non-static Member Function
        void          initEscapeSet();
        void          readSmtFile(const char*,const char*);
        void          extractDT();

        void          analyzePTASCII();

        // Access Function
        const size_t& getGFlag() {return _gflag;}
        
        
    private :
        // Read SMT File
        void          declaration(const string&, const string&);
        SmtNode*      assertion(const vector<string>&, size_t, size_t);
        SmtNode*      buildSmtNode(const string&);
        // Dependency Tree Extraction
        void          buildPredMap();
        void          branching(size_t, const size_t&, size_t&);
        void          buildAndWriteDT(const size_t&);
        void          writeSmtFile(const string&, DTNode*, const DTNodeList&);
        void          writeS3File(const string&, DTNode*, const DTNodeList&);
        void          writeLAutFile(const string&, DTNode*, const DTNodeList&);
        
        static set<char>    escapeSet;
        SmtNode*            _root;
        Str2SmtNodeMap      _smtLeafMap;
        vector<SmtNodeList> _piList;
        Str2SmtNodeListMap  _predMap;
        SmtNodeList         _branchList;
        size_t              _indent;
        size_t              _gflag;
        string              _path;
};

};
#endif
