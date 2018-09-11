#ifndef _UTIL_H_
#define _UTIL_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <assert.h>

using namespace std;

bool isNumber(const string&);
string itos(const size_t&);
size_t stoi(const string&);
void splitLine(const char* );
void str2tokens(const string& , vector<string>&);
void str2tokens(const string& , const string&, vector<string>&);
string Uint2BitString(const size_t&, const size_t&);
size_t binaryEncodedBitNum(const size_t&);
#endif
