//#include <iostream>
//#include "LoopManager.h"
//#include "FileWriter.h"
//#include "CommandParser.h"
//int main() {
//
//    FileReader reader;
//    CommandParser commandParser;
//    LoopManager manager;
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
//    vector<Command> commands = commandParser.parseFile(lines);
//    vector<ErrorMsg> wrongCommands = commandParser.getWrongCommands();
//    data = manager.loop(commands,wrongCommands);
//
//    FileWriter writer;
//    writer.writeToFile("Output.txt",data);
//    return 0;
//
//
//
//}