#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <assert.h>
#include <stdlib.h>

using namespace std;

namespace dt {

class DTMgr;
class SmtNode;
class SmtIteNode;
class DTNode;
class PredicateNode;

enum SmtType {
    CONST_BOOL=0, CONST_INT, CONST_STR,
    VAR_BOOL, VAR_INT, VAR_STR,
    EQ,
    // classified by argument type
    NOT, AND, OR, ITE,
    LT,LTOEQ,MT,MTOEQ,PLUS,MINUS,NEG,DIV,
    STRTORE,STRINRE,STRLEN,STRCONCATE,STRCONTAINS,
    STRPREFIXOF,STRSUFFIXOF,STRSUBSTR,STRREPLACE,STRINDEXOF,
    REINTER,REUNION,RECONCATE
};

enum DType {
    LEAF, LVAR, TRKLEN, TRKSTR, COMPLE, 
    CONCATE, UNION, INTER, CONTAINS, PREFIXOF, SUFFIXOF,
    SUBSTR, REPLACE, TRKIDX, OTHER
};

typedef vector<SmtNode*>        SmtNodeList;
typedef queue<SmtNode*>         SmtNodeQueue;
typedef pair<string,SmtNode*>   Str2SmtNode;
typedef map<string,SmtNode*>    Str2SmtNodeMap;
typedef vector<DTNode*>         DTNodeList;
typedef pair<string,DTNode*>    Str2DTNode;
typedef map<string,DTNode*>     Str2DTNodeMap;
typedef pair<bool,SmtNode*>     Assertion; // Phase and SmtNode*
typedef vector<Assertion>       AssertionList;

typedef pair<string,SmtNodeList> Str2SmtNodeList;
typedef map<string,SmtNodeList>  Str2SmtNodeListMap;

typedef pair<string,size_t>     Str2Uint;
typedef map<string,size_t>      Str2UintMap;

};

namespace aut {

class TGEdge;
class TGraph;
class VmtNode;
class Aut;
class AutMgr;

// PARAM : has _paramList different from _source
enum VmtType {
    // LEAF  (leaf node)
    // PI    (primary input)
    INPUT , EXIST  , STATE  , LEN   , STATE_N , LEN_N , PREDBV , PREDIV , 
    // CONST (constant)
    NUM   , CONST0 , CONST1 , PARAM , SPECIAL , 
    // IMD   (intermediate node)
    NOT=30, NEG    , AND    , OR    , PLUS    , MINUS , LT     , LTOEQ  , EQ, MTOEQ, MT, EXCM, MODULE
};

enum ModuleType {
    M_SFA, M_EQ, M_INC1
};

enum AType {
    EPSILON, LEFT_ANGLE, RIGHT_ANGLE
};

enum AutOpType {
    ADDLEN, PREFIX, SUFFIX, INTERSECT, CONCATE, REPLACE, REPLACE_A4
};

typedef vector<string>        STRList;
typedef vector<char>          CHRList;
typedef vector<TGEdge*>       TGEdgeList;
typedef vector<VmtNode*>      VmtNodeList;
typedef set<VmtNode*>         VmtNodeSet;
typedef map<size_t,string>    CubeMap;
typedef vector<VmtNodeList>   VarList;
typedef pair<string,VmtNode*> Str2VmtNode;
typedef map<string,VmtNode*>  Str2VmtNodeMap;

};
#endif
