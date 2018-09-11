#include "autMgr.h"
#include <map>

///////////////////////////////// Global Variable /////////////////////////////

extern aut::AutMgr* autmgr;
static size_t& gflag = autmgr->getGFlag();

namespace aut {

///////////////////////////// Non-Static Member ///////////////////////////////

void VmtNode::print(const size_t& level) const
{
    //cout << string(level*3,' ') << _name << " " << this << " " << Aut::getTypeStr(_type) << " " << _flag;
    cout << string(level*3,' ') << _name << " " << Aut::getTypeStr(_type) << " " << _flag;
    /*
    for (size_t i = 0; i < PI_NUM; ++i) {
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            cout << " " << _paramList[i][j]->_name;
    }*/
    cout << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->print(level+1);
    }
}

void VmtNode::printPARAM() const
{
    cout << "name=" << _name << endl;
    for (size_t i = 0; i < PI_NUM; ++i) {
        cout << Aut::getTypeStr(i) << " :";
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            cout << " " << _paramList[i][j]->_name;
        cout << endl;
    }
}

void VmtNode::write(const size_t& level,ofstream& outFile) const
{
    cout << string(level*3,' ') << _name << " " << Aut::getTypeStr(_type) << " " << _flag << endl;
    #ifndef VMTNODE_NDEBUG
        cout << string(level*3,' ') << _name << " " << this << " " << Aut::getTypeStr(_type) << " " << _flag;
        if   (_flag == gflag)            cout << " visited";
        else                             cout << " unvisited";
        if   (Aut::isLEAF(_type))        cout << " LEAF\n";
        else {assert(Aut::isIMD(_type)); cout << " IMD\n";}
    #endif
    /*
    if (_flag == gflag) {
        if (_type == PARAM) {
            assert( (_children.empty()) );
            assert( (_source->_flag == gflag) );
            assert( (hasParam()) );
            writeParamBody(_source->_name,outFile);
        }
        else {
            if (!_children.empty()) {
                if ( hasParam() ) {
                    writeParamBody(_name,outFile);
                }
                else {
                    cout << "[VmtNode::write] special node name=" << _name << " c0=" << _children[0]->_name << endl;
                    assert( (_children.size() == 1) );
                    assert( (_children[0] == Aut::const0 || _children[0] == Aut::const1) );
                    outFile << _name;
                }
            }
            else {
                outFile << _name;
            }
        }
        return;
    }
    */
    if ( _flag == gflag ) {
        if ( Aut::isLEAF(_type) ) {
            cout << "name=" << _name << " " << this << " type=" << Aut::getTypeStr(_type) << endl;
            // type=PARAM could be refered multiple times
            assert( (_type != NUM &&  _type != SPECIAL) );
            if ( _type == PARAM ) {
                assert( (_children.empty()) );
                assert( (_source->_flag == gflag) );
                assert( (hasParam()) );
                writeParamBody(_source->_name,outFile);
            }
            else
                outFile << _name;
        }
        else {
            assert( (_type == MODULE) );
            assert( (!_children.empty()) );
            if ( hasParam() ) 
                writeParamBody(_name,outFile);
            else {
                cout << "[VmtNode::write] special node name=" << _name << " c0=" << _children[0]->_name << endl;
                assert( (_children.size() == 1) );
                assert( (_children[0] == Aut::const0 || _children[0] == Aut::const1) );
                outFile << _name;
            }
        }
        return;
    }
    _flag = gflag;
    if ( Aut::isLEAF(_type) ) {
        if (_type == PARAM) {
            assert( (_children.empty()) );
            assert( (_source->_flag == gflag) );
            assert( (hasParam()) );
            writeParamBody(_source->_name,outFile);
        }
        else
            outFile << _name;
    }
    else {
        assert( (Aut::isIMD(_type)) );
        assert( (!_children.empty()) );
        //cout << "name=" << _name << " " << this << " type=" << Aut::getTypeStr(_type) << endl;
        outFile << "(" << _name;
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            outFile << " ";
            _children[i]->write(level+1,outFile);
        }
        outFile << ")";
    }
    /*
    if (_type == PARAM) {
        assert( (_children.empty()) );
        assert( (_source->_flag == gflag) );
        assert( (hasParam()) );
        writeParamBody(_source->_name,outFile);
    }
    else {
        if (!_children.empty()) {
            outFile << "(" << _name;
            for (size_t i = 0, size = _children.size(); i < size; ++i) {
                outFile << " ";
                _children[i]->write(level+1,outFile);
            }
            outFile << ")";
        }
        else {
            outFile << _name;
        }
    }
    */
}

VmtType VmtNode::getType(const string& name) const
{
    if      ( name == "not"         ) return NOT;
    else if ( name == "-"           ) return MINUS; // default all MINUS ; merge NEG for _predList
    else if ( name == "and"         ) return AND;
    else if ( name == "or"          ) return OR;
    else if ( name == "+"           ) return PLUS;
    else if ( name == "<"           ) return LT;
    else if ( name == "<="          ) return LTOEQ;
    else if ( name == "="           ) return EQ;
    else if ( name == ">="          ) return MTOEQ;
    else if ( name == ">"           ) return MT;
    else if ( name == "!"           ) return EXCM;
    else if ( name == "false"       ) return CONST0;
    else if ( name == "true"        ) return CONST1;
    else if ( isNumber(name)        ) return NUM;
    else if ( Aut::isSpecialString(name) ) return SPECIAL;
    else if ( Aut::isPISymbol(name[0]) ) Aut::getPITypeByName(name);
    else                              return MODULE;
}

bool VmtNode::hasParam() const
{
    for (size_t i = 0; i < PI_NUM; ++i) {
        if (!_paramList[i].empty())
            return 1;
    }
    return 0;
}

bool VmtNode::haveSameParam(VmtNode* n) const
{
    for (size_t i = 0; i < PI_NUM; ++i) {
        set<string> s0,s1;
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            s0.insert(_paramList[i][j]->_name);
        for (size_t j = 0, size = n->_paramList[i].size(); j < size; ++j)
            s1.insert(n->_paramList[i][j]->_name);
        for (set<string>::iterator it = s1.begin(); it != s1.end(); ++it)
            if (s0.find(*it) == s0.end())
                return 0;
    }
    return 1;
}

void VmtNode::addChild(VmtNode* n)
{
    _children.push_back(n);
}

void VmtNode::clearParam(const size_t& level)
{
    #ifndef VMTNODE_NDEBUG
        cout << string(level*3,' ') << _name << " " << this << " " << Aut::getTypeStr(_type) << " " << _flag;
        if      (_flag == gflag)     cout << " visited";
        else if (Aut::isLEAF(_type)) cout << " LEAF";
        cout << endl;
    #endif
    if ( _flag == gflag || Aut::isLEAF(_type) ) return;
    
    _flag = gflag;
    
    for (size_t i = 0; i < PI_NUM; ++i)
        _paramList[i].clear();
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->clearParam(level+1);
}

void VmtNode::buildParam(const size_t& level)
{
    #ifndef VMTNODE_NDEBUG
        cout << string(level*3,' ') << _name << " " << this << " " << Aut::getTypeStr(_type) << " " << _flag;
        if      (_flag == gflag)     cout << " visited";
        else if (Aut::isLEAF(_type)) cout << " LEAF";
        cout << endl;
    #endif
    if ( _flag == gflag || Aut::isLEAF(_type) ) return;
    _flag = gflag;

    // do not push CONST0 CONST1 into _paramList
    vector<set<size_t> > count(PI_NUM,set<size_t>());
    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->buildParam(level+1);
        const VmtType& type = _children[i]->_type;
        if ( Aut::isPI(type) ) {
            count[type].insert( _children[i]->_idx );
        }
        else if ( Aut::isIMD(type) || type == PARAM ){
            for (size_t j = 0; j < PI_NUM; ++j) {
                for (size_t k = 0; k < _children[i]->_paramList[j].size(); ++k) {
                    if (_children[i]->_paramList[j][k]->_type != CONST0 &&
                        _children[i]->_paramList[j][k]->_type != CONST1   )
                        count[j].insert( _children[i]->_paramList[j][k]->_idx );
                }
            }
        }
    }
    // push parameters into _paramList increasingly
    for (size_t i = 0; i < PI_NUM; ++i) {
        for (set<size_t>::iterator it = count[i].begin(); it != count[i].end(); ++it) {
            _paramList[i].push_back(Aut::piList[i][*it]);
        }
    }
}

void VmtNode::collectPARAM(VmtNodeList& PARAMList)
{
    if (_flag == gflag) return;
    _flag = gflag;
    if (_type == PARAM) PARAMList.push_back(this);
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->collectPARAM(PARAMList);
}

void VmtNode::spotNEG()
{
    cout << "spotneg name=" << _name << " type=" << Aut::getTypeStr(_type) << endl;
    if ( _flag == gflag ) return;
    _flag = gflag;
    if ( _type == MINUS && _children.size() == 1) { cout << "change to NEG\n";_type = NEG;}
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->spotNEG();
}

void VmtNode::writeParamHead(ofstream& file) const
{
    cout << "name=" << _name << " type" << Aut::getTypeStr(_type) << endl;
    assert( (_type == MODULE) );
    bool isfirst = 1;
    #ifndef VMTNODE_NDEBUG
        cout << "[VmtNode::writeParamHead]";
    #endif
    for (size_t i = 0; i < PI_NUM; ++i) {
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j) {
            #ifndef VMTNODE_NDEBUG
                cout << " " << _paramList[i][j]->_name 
                     << " " << Aut::getTypeStr(_paramList[i][j]->_type);
            #endif
            if (!isfirst) file << " ";
            isfirst = 0;
            file << "(" << _paramList[i][j]->_name;
            if (i == LEN || i == LEN_N || i == PREDIV) {
                file << " Int)";
            }
            else {
                file << " Bool)";
            }
        }
    }
    #ifndef VMTNODE_NDEBUG
        cout << endl;
    #endif
}

void VmtNode::writeParamBody(const string& fname, ofstream& file) const
{
    file << "(" << fname;
    for (size_t i = 0; i < PI_NUM; ++i) {
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            file << " " << _paramList[i][j]->_name;
    }
    file << ")";
}

void VmtNode::shiftStateVar(const size_t& delta)
{
    if ( _flag == gflag || (_type != PARAM && !Aut::isIMD(_type)) ) return;
    
    _flag = gflag;
    if (_type == PARAM) {
        assert((_children.empty()));
        
        // change STATE 
        VmtNodeList tmp;
        for (size_t i = 0, size = _paramList[STATE].size(); i < size; ++i)
            tmp.push_back( Aut::piList[STATE][ _paramList[STATE][i]->_idx + delta ] );
        _paramList[STATE] = tmp;
        
        // change STATE_N
        tmp.clear();
        for (size_t i = 0, size = _paramList[STATE_N].size(); i < size; ++i)
            tmp.push_back( Aut::piList[STATE_N][ _paramList[STATE_N][i]->_idx + delta ] );
        _paramList[STATE_N] = tmp;
        return;
    }

    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->shiftStateVar(delta);
        //cout << "node = " << _name << " children[" << i << "] = " << _children[i]->_name << endl;
        const VmtType& type = _children[i]->_type;
        if (type != STATE && type != STATE_N) continue;
        _children[i] = Aut::piList[type][ _children[i]->_idx + delta ];
    }
}

size_t VmtNode::getBit() const
{
    return _bit + _pad;
}

void VmtNode::setBit(const size_t& bit,bool& isUpdate)
{
    const size_t ori = getBit();
    if ( _type == LEN || _type == LEN_N ) {
        assert( (_bit == LVAR_BIT_NUM) );
        assert( (bit >= _bit) );
        _pad = bit - _bit;
    }
    else if ( _type == NUM ) {
        assert( (bit >= _bit) );
        _pad = bit - _bit;
    }
    else {
        _bit = bit;
    }
    if ( ori != getBit() ) {
        cout << "[updateBit] " << _name << " " << ori << " -> " << getBit() << endl;
        isUpdate = 1;
    }
}

void VmtNode::buildBLIF(int& tCnt)
{
    assert( (_type != EXCM && _type != SPECIAL) );
    if ( _flag == gflag ) return;
    _flag = gflag;
    cout << "[buildBLIF] " << _name << endl;
    if ( Aut::isOP(_type) ) {
        _bname = "t" + itos(++tCnt);
        #ifdef FORCE_ASSIGNED
        if ( Aut::isARITH(_type) ) _bit = FORCE_SIGNED_BIT_NUM + LVAR_BIT_NUM;
        else {
            assert( (_type == NOT   || _type == AND || _type == OR || _type == LT    ||
                     _type == LTOEQ || _type == EQ  || _type == MT || _type == MTOEQ   ) );
            _bit = 1;
        }
        #endif
    }
    else if ( _type == PARAM ) {
        assert( (_source->_type == MODULE) );
        //_source->_flag  = gflag;
        //_source->_bname = _source->_name;
        _bname = "t" + itos(++tCnt);
        #ifdef FORCE_ASSIGNED
        _bit = 1;
        #endif
        _source->buildBLIF(tCnt);
    }
    else if ( _type == NUM ) {
        const size_t n = stoi(_name);
        _bit   = binaryEncodedBitNum( n );
        _bname = Uint2BitString( n , _bit );
        #ifdef FORCE_ASSIGNED
        assert( (FORCE_SIGNED_BIT_NUM + LVAR_BIT_NUM >= _bit) );
        _pad   = FORCE_SIGNED_BIT_NUM + LVAR_BIT_NUM - _bit;
        #endif
    }
    else if ( Aut::isCONST(_type) ) {
        _bit  = 1;
    }
    else if ( _type == LEN || _type == LEN_N ) {
        _bit   = LVAR_BIT_NUM;
        #ifdef FORCE_ASSIGNED
        _pad   = FORCE_SIGNED_BIT_NUM;
        #else
        _pad   = 1;
        #endif
        _bname = _name;
    }
    else if ( _type == PREDIV ) {
        #ifdef FORCE_ASSIGNED
        _bit   = FORCE_SIGNED_BIT_NUM + LVAR_BIT_NUM;
        #endif
        _bname = _name;
    }
    else if ( _type == MODULE ) {
        if ( this != Aut::epsilon && this != Aut::leftAngle && this != Aut::rightAngle ) _bname = _name;
        #ifdef FORCE_ASSIGNED
        _bit = 1;
        #endif
    }
    else {
        // _bit of PREDIV is set in setBit()
        assert( (_type == INPUT || _type == EXIST || _type == STATE || _type == STATE_N || _type == PREDBV) );
        _bname = _name;
        #ifdef FORCE_ASSIGNED
        _bit = 1;
        #endif
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildBLIF(tCnt);
}

// for _imdList
void VmtNode::setBitNum()
{
    cout << "    name=" << _name << " bname=" << _bname << " type=" << Aut::getTypeStr(_type) << endl;
    assert( (_type != EXCM && _type != SPECIAL) );
    if ( Aut::isLEAF(_type) ) {
        if ( !Aut::isPARAMINT(_type) && _type != NUM ) _bit = 1;
    }
    else if ( _type == MODULE || _type == NOT || _type == AND || _type == OR || _type == EQ ) {
        _bit = 1;
    }
    else {
        assert( (_type == PLUS) );
        assert( (_children[0]->_type == LEN) );
        assert( (_children[1]->_type == NUM) );
        assert( (_children[1]->_bname == "1" ) );
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->setBitNum();
    }
}

// bottom-up
void VmtNode::setBitNumUP(bool& isUpdate)
{
    assert( (_type != EXCM && _type != SPECIAL) );
    if ( Aut::isLEAF(_type) || _type == MODULE ) {
        if ( !Aut::isPREDINT(_type) ) _bit = 1;
        return;
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->setBitNumUP(isUpdate);
    if ( Aut::isARITH(_type) ) {
        assert( (!_children.empty()) );
        size_t max = 0;
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            if ( _children[i]->getBit() > max ) max = _children[i]->getBit();
        setBit( max + 1 , isUpdate );
    }
    else {
        assert( (_type == NOT   || _type == AND || _type == OR || _type == LT    ||
                 _type == LTOEQ || _type == EQ  || _type == MT || _type == MTOEQ   ) );
        setBit( 1 , isUpdate );
    }
}

// top-down
void VmtNode::setBitNumDN(bool& isUpdate)
{
    assert( (_type != EXCM && _type != SPECIAL) );
    if ( Aut::isLEAF(_type) || _type == MODULE ) {
        return;
    }
    else if ( Aut::isARITH(_type) ) {
        size_t max = 0;
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            const VmtType& type = _children[i]->_type;
            assert( (Aut::isARITH(type) || Aut::isPREDINT(type)) );
            if (_children[i]->getBit() > max) max = _children[i]->getBit();
        }
        if (max >= getBit())
            setBit( max + 1 , isUpdate );
        else
            max = getBit() - 1;
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            _children[i]->setBit( max , isUpdate );
    }
    else if ( _type == LT || _type == LTOEQ || _type == EQ || _type == MTOEQ || _type == MT) {
        size_t max = 0;
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            if ( _type == EQ ) {
                const VmtType& type = _children[i]->_type;
                assert( (Aut::isARITH(type) || Aut::isPREDINT(type) ||
                         Aut::isRETBOOL(type) && type != PARAM)      );
            }
            else {
                const VmtType& type = _children[i]->_type;
                assert( (Aut::isARITH(type) || Aut::isPREDINT(type)) );
            }
            if (_children[i]->getBit() > max) max = _children[i]->getBit();
        }
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            _children[i]->setBit( max , isUpdate );
    }
    else {
        assert( (_type == NOT || _type == AND || _type == OR));
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            const VmtType& type = _children[i]->_type;
            assert( (Aut::isRETBOOL(type) && type != PARAM) );
            _children[i]->setBit( 1 , isUpdate );
        }
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->setBitNumDN(isUpdate);
}

void VmtNode::writeMODEL(ofstream& file, vector<set<size_t> >& sizeMap, const bool& isPred)
{
    assert( (_type == MODULE) );
    cout << "name=" << _name << " bname=" << _bname << " type=" << Aut::getTypeStr(_type) << endl;
    file << ".model " << _bname 
         << "\n.inputs";
    for (size_t i = 0; i < PI_NUM; ++i) {
        const VmtType type = static_cast<VmtType>(i);
        const VmtNodeList& v = _paramList[i];
        for (size_t j = 0, size = v.size(); j < size; ++j) {
            if ( Aut::isPARAMINT(type) ) {
                for (size_t k = 0; k < v[j]->_bit; ++k)
                    file << " " << v[j]->_bname << "_" << k;
                file << Aut::BLIFIndent;
            }
            else {
                file << " " << v[j]->_bname;
            }
        }
        if ( !Aut::isPARAMINT(type) && !v.empty() )
            file << Aut::BLIFIndent;
    }
    file << "\n.outputs " << _bname << "_0"
         << "\n.names "   << _children[0]->_bname;
    bool tUsed = 0, fUsed = 0;
    if ( !Aut::isPARAMBOOL(_children[0]->_type) && !Aut::isCONST(_children[0]->_type) ) file << "_0";
    if ( _children[0] == Aut::const0 ) fUsed = 1;
    if ( _children[0] == Aut::const1 ) tUsed = 1;
    file << " " << _bname << "_0"
         << "\n1 1\n";
    _children[0]->writeSUBCKT(file,sizeMap,tUsed,fUsed,isPred);
    if (tUsed) file << ".names const1\n1\n";
    if (fUsed) file << ".names const0\n";
    file << ".end\n\n";
}

void VmtNode::writeSUBCKT(ofstream& file, vector<set<size_t> >& sizeMap ,bool& tUsed, bool& fUsed, const bool& isPred)
{
    cout << "    name=" << _name << " bname=" << _bname << " type=" << Aut::getTypeStr(_type) << "isPred=" << isPred << endl;
    assert( (_type != EXCM && _type != SPECIAL) );
    if ( Aut::isLEAF(_type) && _type != PARAM ) return;
    if ( _type == MODULE ) {
        assert( (getBit() == 1) );
        file << ".subckt " << _bname << Aut::BLIFIndent;
        for (size_t i = 0; i < PI_NUM; ++i) {
            const VmtType type = static_cast<VmtType>(i);
            const VmtNodeList& v = _paramList[i];
            for (size_t j = 0, size = v.size(); j < size; ++j) {
                if ( Aut::isPARAMINT(type) ) {
                    for (size_t k = 0; k < v[j]->_bit; ++k)
                        file << " " << v[j]->_bname << "_" << k
                             << "=" << v[j]->_bname << "_" << k;
                    file << Aut::BLIFIndent;
                }
                else {
                    file << " " << v[j]->_bname
                         << "=" << v[j]->_bname;
                }
            }
            if ( !Aut::isPARAMINT(type) && !v.empty() )
                file << Aut::BLIFIndent;
        }
        // output of MODULE is indicate by _bname + "_0"
        file << " " << _bname << "_0=" << _bname << "_0\n";
        return;
    }
    else if ( _type == PARAM ) {
        assert( (getBit() == 1) );
        assert( (!isPred) );
        assert( (_children.empty()) );
        assert( (_source->_type == MODULE) );
        assert( (_paramList[PREDBV].empty()) );
        assert( (_paramList[PREDIV].empty()) );
        assert( (_source->_paramList[PREDBV].empty()) );
        assert( (_source->_paramList[PREDIV].empty()) );
        file << "# PARAM\n";
        file << ".subckt " << _source->_bname << Aut::BLIFIndent;
        if ( _paramList[INPUT].empty() ) {
            if ( _paramList[EXIST].empty() ) {
                // I(s.next)
                for (size_t i = 0; i < PI_NUM; ++i) {
                    if (i != STATE_N) assert( (_paramList[i].empty()) );
                    if (i != STATE) assert( (_source->_paramList[i].empty()) );
                }
                assert( (_paramList[STATE_N].size() == _source->_paramList[STATE].size()) );
                for (size_t i = 0, size = _paramList[STATE_N].size(); i < size; ++i)
                    file << " " << _source->_paramList[STATE][i]->_bname 
                         << "=" << _paramList[STATE_N][i]->_bname;
                file << Aut::BLIFIndent;
            }
            else {
                // T(y,...)
                const size_t pysize = _paramList[EXIST].size();
                const size_t sxsize = _source->_paramList[INPUT].size();
                const size_t sysize = _source->_paramList[EXIST].size();
                assert( (_paramList[INPUT].empty()) );
                assert( (pysize == sxsize + sysize) );
                for (size_t i = 0; i < sxsize; ++i) {
                    file << " " << _source->_paramList[INPUT][i]->_bname
                         << "=" << _paramList[EXIST][i]->_bname;
                }
                file << Aut::BLIFIndent;
                for (size_t i = 0; i < sysize; ++i)
                    file << " " << _source->_paramList[EXIST][i]->_bname
                         << "=" << _paramList[EXIST][i+sxsize]->_bname;
                file << Aut::BLIFIndent;
                for (size_t i = 2; i < PI_NUM; ++i) {
                    const VmtType type = static_cast<VmtType>(i);
                    const VmtNodeList& pv = _paramList[i];
                    const VmtNodeList& sv = _source->_paramList[i];
                    assert( (pv.size() == sv.size()) );
                    for (size_t j = 0, size = pv.size(); j < size; ++j) {
                        if ( Aut::isPARAMINT(type) ) {
                            for (size_t k = 0; k < pv[j]->_bit; ++k) {
                                file << " " << sv[j]->_bname << "_" << k
                                     << "=" << pv[j]->_bname << "_" << k;
                            }
                            file << Aut::BLIFIndent;
                        }
                        else {
                            file << " " << sv[j]->_bname << "=" << pv[j]->_bname;
                        }
                    }
                    if ( !Aut::isPARAMINT(type) && !pv.empty() )
                        file << Aut::BLIFIndent;
                }
            }
        }
        else {
            // T(<special_alphabet>,...)
            assert( (!_paramList[INPUT].empty()) );
            for (size_t i = 0, size = _paramList[INPUT].size(); i < size; ++i) {
                const VmtType& type = _paramList[INPUT][i]->_type;
                assert( (Aut::isCONST(type)) );
                if ( type == CONST1 ) tUsed = 1;
                else                  fUsed = 1;
            }
            for (size_t i = 0; i < PI_NUM; ++i) {
                const VmtType type = static_cast<VmtType>(i);
                const VmtNodeList& pv = _paramList[i];
                const VmtNodeList& sv = _source->_paramList[i];
                assert( (pv.size() == sv.size()) );
                for (size_t j = 0, size = pv.size(); j < size; ++j) {
                    if ( Aut::isPARAMINT(type) ) {
                        for (size_t k = 0; k < pv[j]->_bit; ++k) {
                            file << " " << sv[j]->_bname << "_" << k
                                 << "=" << pv[j]->_bname << "_" << k;
                        }
                        file << Aut::BLIFIndent;
                    }
                    else {
                        file << " " << sv[j]->_bname << "=" << pv[j]->_bname;
                    }
                }
                if ( !Aut::isPARAMINT(type) && !pv.empty() ) 
                    file << Aut::BLIFIndent;
            }
        }
        // output of MODULE is indicate by _bname + "_0"
        file << " " << _source->_bname << "_0=" << _bname << "_0\n";
    }
    else if ( _type == NOT ) {
        assert( (getBit() == 1) );
        assert( (_children.size() == 1) );
        const VmtType& type = _children[0]->_type;
        file << ".names " << _children[0]->_bname;
        if ( !Aut::isPARAMBOOL(type) && !Aut::isCONST(type) ) file << "_0";
        if ( _children[0] == Aut::const0 ) fUsed = 1;
        if ( _children[0] == Aut::const1 ) tUsed = 1;
        file << " " << _bname << "_0"
             << "\n0 1\n";
    }
    else if ( _type == NEG ) {
        assert( (isPred) );
        assert( (_children.size() == 1) );
        const VmtType& type = _children[0]->_type;
        assert( (Aut::isPREDINT(type)) );
        const size_t ibit = getBit();
        tUsed = 1;
        file << ".subckt " << ibit << "bSFA" << Aut::BLIFIndent;
        Aut::writeFAList(ibit,"a","const0",file);
        Aut::writeFAListARITH(tUsed,fUsed,ibit,"b",_children[0],file);
        file << " sign=const1";
        Aut::writeFAList(ibit,"s",this,file);
        file << endl;
        sizeMap[M_SFA].insert(ibit);
    }
    else if ( _type == AND ) {
        //cout << "name=" << _name << " bname=" << _bname << " type=" << Aut::getTypeStr(_type) << endl;
        assert( (getBit() == 1) );
        file << ".names";
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            const VmtType& type = _children[i]->_type;
            file << " " << _children[i]->_bname;
            if ( !Aut::isPARAMBOOL(type) && !Aut::isCONST(type) ) file << "_0";
            if ( _children[i] == Aut::const0 ) fUsed = 1;
            if ( _children[i] == Aut::const1 ) tUsed = 1;
        }
        file << " " << _bname << "_0\n"
             << string(_children.size(),'1') << " 1\n";
    }
    else if ( _type == OR ) {
        assert( (getBit() == 1) );
        file << ".names";
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            const VmtType& type = _children[i]->_type;
            file << " " << _children[i]->_bname;
            if ( !Aut::isPARAMBOOL(type) && !Aut::isCONST(type) ) file << "_0";
            if ( _children[i] == Aut::const0 ) fUsed = 1;
            if ( _children[i] == Aut::const1 ) tUsed = 1;
        }
        file << " " << _bname << "_0\n";
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            string bitstr(size,'-'); 
            bitstr[i] = '1';
            file << bitstr << " 1\n";
        }
    }
    else if ( _type == PLUS || _type == MINUS ) {
        assert( (isPred) );
        assert( (_children.size() == 2) );
        const VmtType& type0 = _children[0]->_type;
        const VmtType& type1 = _children[1]->_type;
        assert( (Aut::isPREDINT(type0) || Aut::isARITH(type0)));
        assert( (Aut::isPREDINT(type1) || Aut::isARITH(type1)));
        const size_t ibit = getBit();
        file << ".subckt " << ibit << "bSFA" << Aut::BLIFIndent;
        Aut::writeFAListARITH(tUsed,fUsed,ibit,"a",_children[0],file);
        Aut::writeFAListARITH(tUsed,fUsed,ibit,"b",_children[1],file);
        if ( _type == PLUS ) { fUsed = 1; file << " sign=const0"; }
        else                 { tUsed = 1; file << " sign=const1"; }
        Aut::writeFAList(ibit,"s",this,file);
        file << endl;
        sizeMap[M_SFA].insert(ibit);
    }
    else if ( _type == LT || _type == MTOEQ ) {
        assert( (getBit() == 1) );
        assert( (_children.size() == 2) );
        const VmtType& type0 = _children[0]->_type;
        const VmtType& type1 = _children[1]->_type;
        assert( (Aut::isPREDINT(type0) || Aut::isARITH(type0)));
        assert( (Aut::isPREDINT(type1) || Aut::isARITH(type1)));
        assert( (_children[0]->getBit() == _children[1]->getBit()) );
        #ifdef FORCE_ASSIGNED
        const size_t   ibit  = _children[0]->getBit();
        assert( (ibit == FORCE_SIGNED_BIT_NUM + LVAR_BIT_NUM) );
        #else
        const size_t   ibit  = _children[0]->getBit() + 1;
        #endif
        file << ".subckt " << ibit << "bSFA" << Aut::BLIFIndent;
        Aut::writeFAListARITH(tUsed,fUsed,ibit,"a",_children[0],file);
        Aut::writeFAListARITH(tUsed,fUsed,ibit,"b",_children[1],file);
        tUsed = 1;
        file << " sign=const1";
        if ( _type == LT ) {
            file << " s" << ibit - 1 << "=" << _bname << "_0" << endl;
        }
        else {
            file << " s" << ibit - 1 << "=" << _bname << "_0_0\n"
                 << ".names " << _bname << "_0_0 " << _bname << "_0\n"
                 << "0 1\n";
        }
        sizeMap[M_SFA].insert(ibit);
    }
    else if ( _type == MT || _type == LTOEQ ) {
        assert( (getBit() == 1) );
        assert( (_children.size() == 2) );
        const VmtType& type0 = _children[0]->_type;
        const VmtType& type1 = _children[1]->_type;
        assert( (Aut::isPREDINT(type0) || Aut::isARITH(type0)));
        assert( (Aut::isPREDINT(type1) || Aut::isARITH(type1)));
        assert( (_children[0]->getBit() == _children[1]->getBit()) );
        #ifdef FORCE_ASSIGNED
        const size_t   ibit  = _children[0]->getBit();
        assert( (ibit == FORCE_SIGNED_BIT_NUM + LVAR_BIT_NUM) );
        #else
        const size_t   ibit  = _children[0]->getBit() + 1;
        #endif
        file << ".subckt " << ibit << "bSFA" << Aut::BLIFIndent;
        Aut::writeFAListARITH(tUsed,fUsed,ibit,"a",_children[1],file);
        Aut::writeFAListARITH(tUsed,fUsed,ibit,"b",_children[0],file);
        tUsed = 1;
        file << " sign=const1";
        if ( _type == MT ) {
            file << " s" << ibit - 1 << "=" << _bname << "_0" << endl;
        }
        else {
            file << " s" << ibit - 1 << "=" << _bname << "_0_0\n"
                 << ".names " << _bname << "_0_0 " << _bname << "_0\n"
                 << "0 1\n";
        }
        sizeMap[M_SFA].insert(ibit);
    }
    else {
        assert( (getBit() == 1) );
        assert( (_type == EQ) );
        assert( (_children.size() == 2) );
        const VmtType& type0 = _children[0]->_type;
        const VmtType& type1 = _children[1]->_type;

        if ( isPred ) {
            if ( Aut::isPREDRETBOOL(type0) ) {
                assert( (Aut::isPREDRETBOOL(type1)) );
                file << ".names " << _children[0]->_bname;
                if ( type0 != PREDBV && type0 != CONST0 && type0 != CONST1 ) file << "_0";
                file << " "       << _children[1]->_bname;
                if ( type1 != PREDBV && type1 != CONST0 && type1 != CONST1 ) file << "_0";
                file << " " << _bname << "_0";
                file << "\n00 1"
                     << "\n11 1\n";
            }
            else {
                assert( (Aut::isPREDRETINT(type0)) );
                assert( (Aut::isPREDRETINT(type1)) );
                const size_t bit0 = _children[0]->getBit();
                const size_t bit1 = _children[1]->getBit();
                assert( (bit0 == bit1) );
                const size_t ibit = bit0;
                file << ".subckt " << ibit << "bEQ" << Aut::BLIFIndent;
                Aut::writeFAList(tUsed,fUsed,ibit,"a",_children[0],file);
                Aut::writeFAList(tUsed,fUsed,ibit,"b",_children[1],file);
                file << " out=" << _bname << "_0\n";
                sizeMap[M_EQ].insert(ibit);
            }
            
        }
        else {
            if ( type0 == LEN_N ) {
                if ( type1 ==  LEN ) {
                    assert( (_children[0]->_bit == LVAR_BIT_NUM) );
                    assert( (_children[1]->_bit == LVAR_BIT_NUM) );
                    const size_t ibit = LVAR_BIT_NUM;
                    file << ".subckt " << ibit << "bEQ" << Aut::BLIFIndent;
                    Aut::writeFAList(ibit,"a",_children[0],file);
                    Aut::writeFAList(ibit,"b",_children[1],file);
                    file << " out=" << _bname << "_0\n";
                    sizeMap[M_EQ].insert(ibit);
                }
                else {
                    assert( (type1 == PLUS) );
                    assert( (_children[0]->_bit == LVAR_BIT_NUM) );
                    assert( (_children[1]->_children[0]->_bit == LVAR_BIT_NUM));
                    const size_t ibit = LVAR_BIT_NUM;
                    file << ".subckt " << ibit << "bINC1" << Aut::BLIFIndent;
                    Aut::writeFAList(ibit,"a",_children[1]->_children[0],file);
                    Aut::writeFAList(ibit,"b",_children[0],file);
                    file << " out=" << _bname << "_0\n";
                    sizeMap[M_EQ].insert(ibit);
                    sizeMap[M_INC1].insert(ibit);
                }
            }
            else {
                assert( (type0 == STATE_N && type1 == STATE) );
                file << ".names " << _children[0]->_bname << " " << _children[1]->_bname 
                     << " "       << _bname << "_0"
                     << "\n00 1"
                     << "\n11 1\n";
            }
            return;
        }
    }

    // preorder traversal to early return for some cases
    for ( size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeSUBCKT(file,sizeMap,tUsed,fUsed,isPred);
    
}

}
