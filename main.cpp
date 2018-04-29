#include <iostream>
#include "LoopManager.h"
#include "FileWriter.h"
#include "CommandParser.h"
int main() {

    FileReader reader;
    CommandParser commandParser;
    LoopManager manager;
    PrimaryData data;
    vector<Command> commands = commandParser.parseFile(reader.readFile("Input.txt"));
    vector<ErrorMsg> wrongCommands = commandParser.getWrongCommands();
    data = manager.loop(commands,wrongCommands);

    FileWriter writer;
    writer.writeToFile("Output.txt",data);
    return 0;



}