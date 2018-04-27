#include <iostream>
#include "LoopManager.h"
#include "FileWriter.h"
#include "CommandParser.h"
int main() {

    FileReader reader;
    //CommandParser commandParser;
    LoopManager manager;
    PrimaryData data;
    vector<Command> commands;// = commandParser.parseFile(reader.readFile("/home/khaled/test.txt"));
    Command tmp1;

    tmp1.label = "Htp";
    tmp1.mnemonic="START";
    tmp1.operands.push_back("1000");
    commands.push_back(tmp1);
    Command tmp2;

    tmp2.label = "try";
    tmp2.mnemonic="+LDX";
    tmp2.operands.push_back("=X'00'");

    commands.push_back(tmp2);

    Command tmpx;

    tmpx.label = "x";
    tmpx.mnemonic="RMO";
    tmpx.operands.push_back("X'00'");

    commands.push_back(tmpx);

    Command tmp3;

    tmp3.label = "re";
    tmp3.mnemonic="RESW";
    tmp3.operands.push_back("2");

    commands.push_back(tmp3);

    Command tmp4;

    tmp4.label = "by";
    tmp4.mnemonic="BYTE";
    tmp4.operands.push_back("C'abdo'");

    commands.push_back(tmp4);

    Command tmp6;

    tmp6.label = "byx";
    tmp6.mnemonic="LDA";
    tmp6.operands.push_back("=C'nope'");

    commands.push_back(tmp6);

    Command tmp7;

    tmp7.label = "";
    tmp7.mnemonic="LTORG";
    tmp7.operands.push_back("");

    commands.push_back(tmp7);
    Command tmp8;

    tmp8.label = "";
    tmp8.mnemonic="ORG";
    tmp8.operands.push_back("1000");

    commands.push_back(tmp8);
    Command tmp9;

    tmp9.label = "e7m";
    tmp9.mnemonic="RESW";
    tmp9.operands.push_back("20");

    commands.push_back(tmp9);

    Command tmp10;

    tmp10.label = "e7m2";
    tmp10.mnemonic="ORG";
    tmp10.operands.push_back("0000");

    commands.push_back(tmp10);
    Command tmp11;

    tmp11.label = "nope";
    tmp11.mnemonic="EQU";
    tmp11.operands.push_back("222");

    commands.push_back(tmp11);


    Command tmp13;
    tmp13.label = "noob";
    tmp13.mnemonic="EQU";
    tmp13.operands.push_back("nope");
    commands.push_back(tmp13);

    Command tmp5;
    tmp5.label = "";
    tmp5.mnemonic="END";
    commands.push_back(tmp5);
    data = manager.loop(commands);
//    for(vector<Command>::iterator it = data.commands.begin(); it != data.commands.end();++it){
//        cout<<(*it).address<<"\t"<<(*it).label<<"\t"<<(*it).mnemonic<<"\t";
//        if((*it).operands.size()!=0)
//            cout<<(*it).operands.at(0)<<"\n";
//
//    }
//
//    for(map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, labelInfo>::iterator it = data.symbolTable.begin();
//        it != data.symbolTable.end(); ++it)
//    {
//        std::cout << it->first << " " << it->second.address << " " << it->second.type << "\n";
//    }
//
//
    FileWriter writer;
    writer.writeToFile("test2.txt",data);
    return 0;



}