#include "autMgr.h"

namespace aut {

void TGEdge::print() const 
{
    cout << "[TGEdge::print] " << _sIdx << " -> "<< _eIdx << " ";
    if (_labels.size() == 2)
        cout << _labels[0] << " - " << _labels[1] << endl;
    else {
        assert( (_labels.size() == 1) );
        cout << _labels[0] << endl;
    }
}

void TGEdge::write(const CubeMap& downCubeMap, const CubeMap& rangeCubeMap, const vector<string>& stateBitStringList, ofstream& file)
{
    file << "\n# " << _sIdx << " -> "<< _eIdx << " ";
    if (_labels.size() == 2)
        file << _labels[0] << " - " << _labels[1] << endl;
    else {
        assert( (_labels.size() == 1) );
        file << _labels[0] << endl;
    }
    _tBitString = stateBitStringList[_sIdx] + stateBitStringList[_eIdx] + " 1\n";
    if (_labels.size() == 2) {
        if (_labels[0] == 0) {
            if (_labels[1] == MAX_ENCODE) {
                file << downCubeMap.find(MAX_ENCODE)->second << _tBitString;
            }
            else {
                CubeMap::const_iterator it = downCubeMap.lower_bound(_labels[1]);
                if (it->first == _labels[1])
                    file << it->second << _tBitString;
                else {
                    assert( (_labels[1] > 0 && it->first > _labels[1]) );
                    file << (--it)->second << _tBitString;
                    writeRangeMinterm(it->first+1,_labels[1],file);
                }
            }
        }
        else {
            if (_labels[1] == MAX_ENCODE) {
                CubeMap::const_iterator it = rangeCubeMap.lower_bound(_labels[0]);
                if (it->first/2 + 1 == _labels[0]) {
                    for (; it != rangeCubeMap.end(); ++it)
                        file << it->second << _tBitString;
                }
                else {
                    writeRangeMinterm(_labels[0],it->first,file);
                    for (++it; it != rangeCubeMap.end(); ++it)
                        file << it->second << _tBitString;
                }
            }
            else {
                CubeMap::const_iterator it0 = rangeCubeMap.lower_bound(_labels[0]);
                CubeMap::const_iterator it1 = rangeCubeMap.lower_bound(_labels[1]);
                if (it0 == it1) {
                    if (it0->first/2 + 1 == _labels[0] && it1->first == _labels[1])
                        file << it0->second << _tBitString;
                    else
                        writeRangeMinterm(_labels[0],_labels[1],file);
                }
                else {
                    if (it0->first/2 + 1 != _labels[0]) {
                        writeRangeMinterm(_labels[0],it0->first,file);
                        ++it0;
                    }
                    if (it1->first == _labels[1]) ++it1;
                    else {
                        writeRangeMinterm(it1->first/2+1,_labels[1],file);
                    }
                    for (; it0 != it1; ++it0)
                        file << it0->second << _tBitString;
                }
            }
        }
    }
    else {
        assert( _labels.size() == 1 );
        file << Uint2BitString(_labels[0],INPUT_ENCODE_BIT_NUM+1) << _tBitString;
    }
}

void TGEdge::writeRangeMinterm(const size_t& m1, const size_t& m2, ofstream& file)
{
    for (size_t i = m1; i <= m2; ++i)
        file << Uint2BitString(i,INPUT_ENCODE_BIT_NUM+1) << _tBitString;
}

void TGraph::init()
{  
    /*
    cout << "inputBitNum     = " << INPUT_ENCODE_BIT_NUM << endl
         << "initStateBitNum = " << INIT_STATE_BIT_NUM << endl
         << "initLvarNum     = " << INIT_LVAR_NUM << endl
         << "epsilon         = " << EPSILON_ENCODE << endl
         << "left            = " << LEFT_ANGLE_BRACKET_ENCODE << endl
         << "right           = " << RIGHT_ANGLE_BRACKET_ENCODE << endl;
    */
    _numbers.insert('0');
    _numbers.insert('1');
    _numbers.insert('2');
    _numbers.insert('3');
    _numbers.insert('4');
    _numbers.insert('5');
    _numbers.insert('6');
    _numbers.insert('7');
    _numbers.insert('8');
    _numbers.insert('9');
    
    _h2dMap.insert(pair<char,size_t>('0',0));
    _h2dMap.insert(pair<char,size_t>('1',1));
    _h2dMap.insert(pair<char,size_t>('2',2));
    _h2dMap.insert(pair<char,size_t>('3',3));
    _h2dMap.insert(pair<char,size_t>('4',4));
    _h2dMap.insert(pair<char,size_t>('5',5));
    _h2dMap.insert(pair<char,size_t>('6',6));
    _h2dMap.insert(pair<char,size_t>('7',7));
    _h2dMap.insert(pair<char,size_t>('8',8));
    _h2dMap.insert(pair<char,size_t>('9',9));
    _h2dMap.insert(pair<char,size_t>('a',10));
    _h2dMap.insert(pair<char,size_t>('b',11));
    _h2dMap.insert(pair<char,size_t>('c',12));
    _h2dMap.insert(pair<char,size_t>('d',13));
    _h2dMap.insert(pair<char,size_t>('e',14));
    _h2dMap.insert(pair<char,size_t>('f',15));

    size_t prod = 2;
    string bitstr ( INPUT_ENCODE_BIT_NUM + 1, '0' );
    for (int i = INPUT_ENCODE_BIT_NUM; i >= 1; --i) {
        bitstr[i] = '-';
        _downCubeMap.insert(pair<size_t,string>(prod-1,bitstr));
        prod *= 2;
    }

    bitstr = string( INPUT_ENCODE_BIT_NUM + 1, '0' );
    bitstr[ INPUT_ENCODE_BIT_NUM ] = '-';
    _rangeCubeMap.insert(pair<size_t,string>(1,bitstr));
    prod = 4;
    for (int i = INPUT_ENCODE_BIT_NUM-1; i >= 1; --i) {
        bitstr[i+1] = '-';
        bitstr[i]   = '1';
        _rangeCubeMap.insert(pair<size_t,string>(prod-1,bitstr));
        prod *= 2;
    }
}

void TGraph::parse(const char* fileName)
{
    string line;
    ifstream file(fileName);

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    size_t maxStateIdx = 0;
    set<size_t> oSet;
    vector<string> tokenList;
    while (getline(file,line)) {
        tokenList.clear();
        str2tokens(line,tokenList);
        #ifndef TGRAPH_NDEBUG
        cout << "[TGraph::parse] line = \"" << line << "\" tokens = [";
        for (size_t i = 0, size = tokenList.size(); i < size; ++i) {
            if (i != 0) cout << ",";
            cout << " \"" << tokenList[i] << "\" ";
        }
        cout << "]" << endl;
        #endif
        if (tokenList[0] == "initial" && tokenList[1] == "->")
            _initStateIdx = stoi(tokenList[2]);
        else if (_numbers.find(tokenList[0][0]) != _numbers.end()) {
            if (tokenList[1] == "->") {
                size_t sIdx = stoi(tokenList[0]);
                size_t eIdx = stoi(tokenList[2]);
                TGEdge* edge = new TGEdge(sIdx,eIdx);
                parseLabels(tokenList[3], edge->_labels);
                _tList.push_back(edge);
            }
            else {
                assert( (   tokenList[1][0] == '[' && 
                        ( *(tokenList[1].rbegin()) == ']'||
                          *(tokenList[1].rbegin()) == ';') ));
                size_t idx = stoi(tokenList[0]);
                if (idx > maxStateIdx) maxStateIdx = idx;
                if (isAccepting(tokenList[1])) oSet.insert(idx);
            }
        }
        
    }
    file.close();
    
    _stateBitNum = binaryEncodedBitNum(maxStateIdx);

    for (size_t i = 0; i <= maxStateIdx; ++i)
        _stateBitStringList.push_back(Uint2BitString(i,_stateBitNum));

    for (set<size_t>::iterator it = oSet.begin(); it != oSet.end(); ++it)
        _oList.push_back(*it);
    #ifndef TGRAPH_NDEBUG
        print();
    #endif
}

void TGraph::write(const char* fileName)
{
    write(string(fileName));
}

void TGraph::write(const string& fileName)
{
    ofstream file(fileName.c_str());

    file << ".model " << fileName.substr(0,fileName.find_last_of('.'))
         << "\n.inputs";
    for (size_t i = 0; i < INPUT_ENCODE_BIT_NUM + 1; ++i) file << " x" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " n" << i;
    file << "\n.outputs i o t";

    // initial state
    file << "\n.names";
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    file << " i\n" << _stateBitStringList[_initStateIdx] << " 1\n";

    // accpeting states
    file << "\n.names";
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    file << " o\n";
    for (size_t i = 0, size = _oList.size(); i < size; ++i)
        file << _stateBitStringList[_oList[i]] << " 1\n";
    // transition function
    file << "\n.names";
    for (size_t i = 0; i < INPUT_ENCODE_BIT_NUM + 1; ++i) file << " x" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " n" << i;
    file << " t\n";
    for (size_t i = 0, size = _tList.size(); i < size; ++i) 
        _tList[i]->write(_downCubeMap,_rangeCubeMap,_stateBitStringList,file);
    
    file << "\n.end";
    file.close();
}

void TGraph::print() const
{
    cout << "[TGraph::print] initial " << _initStateIdx << endl;
    cout << "[TGraph::print] accpeting";
    for (size_t i = 0, size = _oList.size(); i < size; ++i)
        cout << " " << _oList[i];
    cout << "[TGraph::print] transition\n";
    for (size_t i = 0, size = _tList.size(); i < size; ++i)
        _tList[i]->print();
}

bool TGraph::isAccepting(const string& line)
{
    size_t i = 0;
    while(line[i] != '=') ++i;
    size_t j = ++i;
    while(line[i] != ',') ++i;
    string type = line.substr(j,i-j);
    assert( (type == "circle" || type == "doublecircle") );
    if (type == "circle") return 0;
    else                  return 1;
}

void TGraph::parseLabels(const string& line, vector<size_t>& labels)
{
    size_t bpos = line.find_first_of('\"') + 1;
    size_t epos = line.find_last_of('\"');
    string labelStr = line.substr(bpos,epos-bpos);

    vector<string> labelTokens;
    size_t dashBpos = labelStr.find_first_of('-');
    size_t dashEpos = labelStr.find_last_of('-');

    if (dashBpos == dashEpos) {
        if (labelStr.size() == 1) {
            labelTokens.push_back(labelStr);
        }
        else {
            // range
            str2tokens(labelStr,"-",labelTokens);
        }
    }
    else {
        // special cases
        assert( (labelStr[0] == '-' || *(labelStr.rbegin()) == '-') );
        if (labelStr[0] == '-') {
            assert( (labelStr[1] == '-') );
            labelTokens.push_back("-");
            labelTokens.push_back(labelStr.substr(2));
        }
        else {
            size_t len = labelStr.size();
            assert( (labelStr[len-2] == '-') );
            labelTokens.push_back(labelStr.substr(0,len-2));
            labelTokens.push_back("-");
        }
    }

    size_t tokenNum = labelTokens.size();
    assert( ( tokenNum == 1 || tokenNum == 2 ) );

    if (tokenNum == 1) {
        labels.push_back(label2Decimal(labelTokens[0]));
    }
    else {
        size_t d0 = label2Decimal(labelTokens[0]);
        size_t d1 = label2Decimal(labelTokens[1]);
        assert( (d0 < d1) );
        labels.push_back(d0);
        labels.push_back(d1);
    }
}

//TODO
size_t TGraph::label2Decimal(const string& s)
{
    if (s[0] == '\\') {
        size_t prod = 1, sum = 0;
        #ifdef UTF16_ENCODE
            size_t ebpos = 2;
        #else
            size_t ebpos = 4;
        #endif
        for (size_t i = 5; i >= ebpos; --i) {
            sum += prod * _h2dMap[s[i]];
            prod *= 16;
        }
        if (sum > MAX_ENCODE) sum = MAX_ENCODE;
        return sum;
    }
    else {
        assert( (s.size() == 1) );
        assert( (size_t(s[0]) <= MAX_ENCODE));
        return size_t(s[0]);
    }
}

}
