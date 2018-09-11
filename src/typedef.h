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
    TRKLEN, TRKSTR, PREFIX, SUFFIX, INTERSECT, UNION, CONCATE, REPLACE, REPLACE_A4
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
