#include "autMgr.h"
#include "utility.h"
aut::AutMgr* autmgr = new aut::AutMgr();

int main(int argc, char* argv[]) {
    //for (size_t i =0;i<50;++i)
    //    cout << dtmgr->uint2str(i) << endl;
    /*
    vector<string> v;
    str2tokens(string("abc   dd asdas  "),v);
    for (size_t i=0, size = v.size(); i < size; ++i)
        cout << v[i] << endl;
    
    str2tokens(string("abc   ddd  asdas~"),v);
    for (size_t i=0, size = v.size(); i < size; ++i)
        cout << v[i] << endl;
    */
    //Kaluza
    //dtmgr->read(argv[1],argv[2]);
    //dtmgr->buildAndWriteDG();
    /*
    string opt(argv[1]);
    if (opt == "0")
        autmgr->blif2vmt(argv[2],argv[3]);
    else if(opt == "1")
        autmgr->readCmdFile(argv[2]);
    */
    /*
    Aut* a1 = new Aut("easy/a.vmt");
    Aut* a2 = new Aut("1001.corecstrs.readable/1/T2_2.vmt");
    Aut* a3 = new Aut();
    a3->concate(a1,a2);
    a3->write("test.vmt");
    */

    /*
    autmgr->readDefFile(argv[1]);
    cout << endl;
    autmgr->readPredFile(argv[2]);
    cout << endl;
    autmgr->print();
    */

    //dtmgr->closeLogFile();
    //dtmgr->analyzePT();
    /*
    vector<string> v;
    v.push_back("(= 0 (str.len T0_2))");
    v.push_back("(= (str.len TY) UI10~)");
    v.push_back("(str.substr v1 0 v3)");
    v.push_back("(= v3 (str.indexof v1 \"<>\" 0))");
    v.push_back("(= (str.indexof v1 \"<\" 0) v3)");
    vector<string> tokens;
    for (size_t i = 0, size = v.size(); i < size; ++i) {
        tokens.clear();
        str2tokens(v[i]," ()",tokens);
        for(size_t j=0,size1=tokens.size();j<size1;++j)
            cout << tokens[j] << "|";
        cout << endl;
    }*/
    string input(argv[1]);
    //AutOp
    /*
    cout << "main.cpp" << endl;
    //Aut* a1 = new Aut();
    if (input == "0") {
        const char* c1 = "debug/blif2vmt/literal_45.blif";
        const char* c2 = "debug/blif2vmt/literal_45.vmt.1";
        autmgr->blif2vmt(c1,c2);
    }
    else if (input == "100") {
        Aut* a1 = new Aut();
        a1->test();
    }
    else if (input == "1") {
        cout << "read a.vmt   write a.vmt.1\n"
             << "read a.vmt.1 write a.vmt.2\n";
        Aut* a1 = new Aut("debug/easy/a.vmt");
        a1->write("debug/easy/a.vmt.1");
        Aut* a2 = new Aut("debug/easy/a.vmt.1");
        a2->write("debug/easy/a.vmt.2");
    }
    else if (input == "2") {
        cout << "read b.vmt   write b.vmt.1\n"
             << "read b.vmt.1 write b.vmt.2\n";
        Aut* a1 = new Aut("debug/easy/b.vmt");
        a1->write("debug/easy/b.vmt.1");
        Aut* a2 = new Aut("debug/easy/b.vmt.1");
        a2->write("debug/easy/b.vmt.2");
    }
    else if (input == "3") {
        cout << "read a_lc.vmt   write a_lc.vmt.1\n"
             << "read a_lc.vmt.1 write a_lc.vmt.2\n";
        Aut* a1 = new Aut("debug/easy/a_lc.vmt");
        a1->write("debug/easy/a_lc.vmt.1");
        Aut* a2 = new Aut("debug/easy/a_lc.vmt.1");
        a2->write("debug/easy/a_lc.vmt.2");
    }
    else if (input == "4") {
        cout << "read a.vmt.1\naddlen\nwrite a_lc.vmt.3\n";
        Aut* a1 = new Aut("debug/easy/a.vmt.1");
        a1->addlen("10");
        a1->write("debug/easy/a_lc.vmt.3");
    }
    else if (input == "5") {
        cout << "read a.vmt b.vmt\nintersect ab_inter.vmt.1\n";
        Aut* a1 = new Aut("debug/easy/a.vmt");
        Aut* a2 = new Aut("debug/easy/b.vmt");
        Aut* a3 = new Aut();
        a3->intersect(a1,a2);
        a3->write("debug/easy/ab_inter.vmt.1");
    }
    else if (input == "6") {
        cout << "read a.vmt.1 b.vmt.1\nintersect ab_inter.vmt.2\n";
        Aut* a1 = new Aut("debug/easy/a.vmt.1");
        Aut* a2 = new Aut("debug/easy/b.vmt.1");
        Aut* a3 = new Aut();
        a3->intersect(a1,a2);
        a3->write("debug/easy/ab_inter.vmt.2");
    }
    else if (input == "7") {
        cout << "read a.vmt b.vmt\nconcate ab_concate.vmt.1\n";
        Aut* a1 = new Aut("debug/easy/a.vmt");
        Aut* a2 = new Aut("debug/easy/b.vmt");
        Aut* a3 = new Aut();
        a3->concate(a1,a2);
        a3->write("debug/easy/ab_concate.vmt.1");
    }
    else if (input == "8") {
        cout << "read ab_concate.vmt.1 a.vmt\nconcate aba_concate.vmt.1\n";
        Aut* a1 = new Aut("debug/easy/ab_concate.vmt.1");
        Aut* a2 = new Aut("debug/easy/a.vmt");
        Aut* a3 = new Aut();
        a3->concate(a1,a2);
        a3->write("debug/easy/aba_concate.vmt.1");
    }
    else if (input == "9") {
        cout << "read ab_concate.vmt.1\nwrite ab_concate.vmt.2\n";
        Aut* a1 = new Aut("debug/easy/ab_concate.vmt.1");
        a1->write("debug/easy/ab_concate.vmt.2");
    }
    else if (input == "10")
        autmgr->blif2vmt(argv[2],argv[3]);
    else if (input == "11")
        autmgr->readCmdFile(argv[2]);
    else if (input == "12") {
        dtmgr->read(argv[2],argv[3]);
        dtmgr->analyzePTASCII();
    }
    else if (input == "13") {
        autmgr->dot2blif("test0.dot","test0.blif");
    }
    else if(input == "14")
        autmgr->dot2blif("test1.dot","test1.blif");
    */
    // commands
    /*
    if (input == "--intersect") {
        Aut* a1 = new Aut(argv[2]);
        Aut* a2 = new Aut(argv[3]);
        Aut* a3 = new Aut();
        a3->intersect(a1,a2);
        a3->write(argv[3]);
    }
    */
    if (input == "--dot2blif") {
        autmgr->dot2blif(argv[2],argv[3]);
    }
    else if (input == "--blif2vmt") {
        autmgr->blif2vmt(argv[2],argv[3]);
    }
    else if (input == "--readCmd") {
        autmgr->readCmdFile(argv[2]);
    }
    /*
    else if (input == "0") {
        cout << "addlen " << argv[2] << endl << "write " << argv[3] << endl;
        Aut* a1 = new Aut(argv[2]);
        a1->addlen("10");
        a1->write(argv[3]);
    }
    else if (input == "1") {
        cout << "read " << argv[2] << endl << "write "  << argv[3] << endl;
        Aut* a1 = new Aut(argv[2]);
        a1->write(argv[3]);
    }
    else if (input == "2") {
        cout << "intersect " << argv[2] << " " << argv[3] << endl << "write " << argv[4] << endl;
        Aut* a1 = new Aut(argv[2]);
        Aut* a2 = new Aut(argv[3]);
        Aut* a3 = new Aut();
        a3->intersect(a1,a2);
        a3->write(argv[4]);
    }
    else if (input == "concate") {
        cout << "concate " << argv[2] << " " << argv[3] << endl << "write " << argv[4] << endl;
        Aut* a1 = new Aut(argv[2]);
        Aut* a2 = new Aut(argv[3]);
        Aut* a3 = new Aut();
        a3->concate(a1,a2);
        a3->write(argv[4]);
    }
    else if (input == "mark") {
        Aut* a1 = new Aut(argv[2]);
        size_t alpha = a1->mark();
        cout << "alpha = " << alpha << endl;
        a1->write(argv[3]);
    }
    else if (input == "replace_A4") {
        Aut* a1 = new Aut(argv[2]);
        Aut* a2 = new Aut(argv[3]);
        Aut* a3 = new Aut(a1,a2,aut::REPLACE_A4);
        a3->write(argv[4]);
    }
    else if (input == "replace") {
        Aut* a1 = new Aut(argv[2]);
        Aut* a2 = new Aut(argv[3]);
        Aut* a3 = new Aut(a1,a2,0,aut::REPLACE);
        a3->write(argv[4]);
    }
    */
    else
        cout << "invalid option=" << input << endl;
    //cout << "at main"<< endl;
    //autmgr->_epsilon->print(0);
    //Aut* a2 = new Aut(argv[2]);
    //Aut* a3 = new Aut();
    //a3->intersect(a1,a2);
    //a1->addlen("1");
    //for (int j=0;j<4;++j)
    //for (int i=0;i<8;++i)
    //    cout << Aut::_xsList[j][i]->getName() << endl;
    /*
    for (int i = 0; i< 4;++i) {
        string s(new char(i));
        cout << s << " | " << bitset<8>(s[0]);
        if (s[0] == i) cout << " y" << endl;
        else                 cout << " n" << endl;
    }
    string s(new char(2));
    cout << s << endl;
    if (s[0] == 3) cout << "gg" << endl;
    else cout << "ng" << endl;
    */
    return 0;
}
