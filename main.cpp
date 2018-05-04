#include <iostream>
#include "FileManipulators/FileWriter.h"
#include "CommandsAndUtilities/CommandParser.h"
#include "PassesLogic/PassOneManager.h"

int main() {

    FileReader reader;
    CommandParser commandParser;
    PassOneManager manager;
    PrimaryData data;

    vector<string> lines = reader.readFile("Input.txt");
    if(lines.size() == 0){

        ofstream file;
        file.open ("Output.txt");
        file<<"the input file is empty";
        return 0;
    }


    vector<Command> commands = commandParser.parseFile(lines);
    vector<ErrorMsg> wrongCommands = commandParser.getWrongCommands();
    data = manager.loop(commands,wrongCommands);
    FileWriter writer;

    vector<ModificationRecord> records;
    ModificationRecord record;
    record.index = 3;
    record.address = commands.at(3).address;

    records.push_back(record);

    vector<string> ob;
    ob.push_back("000000");
    ob.push_back("111111");
    ob.push_back("222222");
    ob.push_back("333333");
    ob.push_back("444444");
    ob.push_back("555555");
    ob.push_back("666666");
    ob.push_back("777777");
    ob.push_back("888888");
    ob.push_back("999999");
    ob.push_back("AAAAAA");
    ob.push_back("BBBBBB");



    writer.generateObjectCodeFile("Output.txt",ob,data,records);

    return 0;



}