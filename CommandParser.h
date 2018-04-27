
#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <iostream>
#include "Command.h"

using namespace std;

class CommandParser {

public:

    vector<int> wrongCommands;
    vector<Command> parseFile(vector<string> lines);
    vector<int> getWrongCommands();

private:
    char validateLineRegex(string line);
    bool validateLineSyntax(Command line);
    Command extractData(string line);

    bool validateWord(Command command);

    bool validateRes(Command command);

    bool validateByte(Command command);

    bool validateStart(Command command);
};


#endif