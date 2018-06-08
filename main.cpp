#include <iostream>
#include "FileManipulators/FileWriter.h"
#include "CommandsAndUtilities/CommandParser.h"
#include "PassesLogic/PassOneManager.h"
#include "PassesLogic/PassTwoManager.h"
#include "FileManipulators/FileReader.h"

int main() {

//    FileReader reader;
//    CommandParser commandParser;
//    PassOneManager manager;
//    PrimaryData data;
//
//    vector<string> lines = reader.readFile("Input.txt");
//    if(lines.size() == 0){
//
//        ofstream file;
//        file.open ("Output.txt");
//        file<<"the input file is empty";
//        return 0;
//    }
//
//
//    vector<Command> commands = commandParser.parseFile(lines);
//    vector<ErrorMsg> wrongCommands = commandParser.getWrongCommands();
//    data = manager.loop(commands,wrongCommands);
//    FileWriter writer;
//
//    vector<ModificationRecord> records;
//    ModificationRecord record;
//    record.index = 3;
//    record.address = commands.at(3).address;
//
//    records.push_back(record);
//
//    vector<string> ob;
//    ob.push_back("000000");
//    ob.push_back("111111");
//    ob.push_back("222222");
//    ob.push_back("333333");
//    ob.push_back("444444");
//    ob.push_back("555555");
//    ob.push_back("666666");
//    ob.push_back("777777");
//    ob.push_back("888888");
//    ob.push_back("999999");
//    ob.push_back("AAAAAA");
//    ob.push_back("BBBBBB");





    FileReader reader;
    CommandParser parser;
    vector<string> lines = reader.readFile("/home/mostafa/CLionProjects/SIC-Assembler/test2.txt");
    vector<Command> commands = parser.parseFile(lines);
    vector<ErrorMsg> wrongCommands = parser.getWrongCommands();
    PassOneManager passOne;
    PrimaryData primaryData = passOne.loop(commands,wrongCommands);
    PassTwoManager passTwo;
    vector<string> obCode = passTwo.generateObjectCode(primaryData);
    vector<ModificationRecord> mod = passTwo.getModifiactionRecords();
    vector<string> textRec = passTwo.getTextRecord();
    for(int i = 0; i < textRec.size(); i++){
        std::cout << (textRec[i]) << "\n";
    }
    return 0;

}