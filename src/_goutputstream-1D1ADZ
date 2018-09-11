#include "utility.h"

bool isNumber(const string& s)
{
    size_t size = s.size();
    if (size == 0) return 0;
    else if (size == 1) {
        if (s[0] < 48 || s[0] > 57)
            return 0;
        else
            return 1;
    }
    else {
        if (s[0] == 48) return 0;
        bool b = 1;
        for (size_t i=0; i<size; ++i)
            if (s[0] < 48 || s[i] > 57){
                b = 0;
                break;
            }
        return b;
    }
}

string itos(const size_t& n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

size_t stoi(const string& s)
{
    size_t sum = 0,p = 1;
    for (string::const_reverse_iterator it = s.rbegin(); it != s.rend(); ++it) {
        sum += p * (*it - 48);
        p *= 10;
    }
    return sum;
}

void splitLine(const char* s)
{
    cout << endl;
    cout << "--------------------"
         << s
         << "--------------------";
    cout << endl;
}

void str2tokens(const string& s, vector<string>& tokenList)
{
    for (size_t i = 0, j = 0, size = s.size(); i < size; ++i) {
        if (s[i] == ' ') continue;
        j = i;
        while (s[j] != ' ' && j < size) ++j;
        tokenList.push_back(s.substr(i,j-i));
        i = j;
    }
}

void str2tokens(const string& s, const string& delimiters, vector<string>& tokenList)
{
    set<char> dSet;
    for (size_t i = 0, size = delimiters.size(); i < size; ++i)
        dSet.insert(delimiters[i]);
    for (size_t i = 0, j = 0, size = s.size(); i < size; ++i) {
        if (dSet.find(s[i]) != dSet.end()) continue;
        j = i;
        while (dSet.find(s[j]) == dSet.end() && j < size) ++j;
        tokenList.push_back(s.substr(i,j-i));
        i = j;
    }
}

string Uint2BitString(const size_t& n, const size_t& bitNum)
{
    size_t prod = 1;
    for (size_t i = 0; i < bitNum; ++i) prod *= 2;
    assert( ( n < prod ) );

    string bitstr (bitNum,'0');
    size_t tmp = n, j = bitNum;
    while (tmp % 2 != 0 || tmp / 2 != 0) {
        --j;
        if (tmp % 2 != 0) bitstr[j] = '1';
        tmp /= 2;
    }
    return bitstr;
}

size_t binaryEncodedBitNum(const size_t& num)
{
    if (num == 0) return 1;
    else {
        size_t tmp = num, bitNum = 0;
        while (tmp % 2 != 0 || tmp / 2 != 0) {
            ++bitNum;
            tmp /= 2;
        }
        return bitNum;
    }
}
