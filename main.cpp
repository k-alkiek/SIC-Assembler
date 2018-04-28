#include <iostream>
#include "LoopManager.h"
#include "FileWriter.h"
#include "CommandParser.h"
int main() {

    FileReader reader;
    CommandParser commandParser;
    LoopManager manager;
    PrimaryData data;
    vector<Command> commands;// = commandParser.parseFile(reader.readFile("/home/khaled/test.txt"));
    commands = commandParser.parseFile(reader.readFile("/home/saraheldafrawy/CLionProjects/SIC-Assembler/testInput.txt"));
    data = manager.loop(commands);

    FileWriter writer;
    writer.writeToFile("/home/saraheldafrawy/CLionProjects/SIC-Assembler/test2.txt",data);
    return 0;



}