#include <iostream>
#include "LoopManager.h"

int main() {

    FileReader reader;
    CommandParser commandParser;
    LoopManager manager;
    PrimaryData data;
    vector<Command> commands= commandParser.parseFile(reader.readFile("/home/mostafa/test.txt"));
    data = manager.loop(commands);
}