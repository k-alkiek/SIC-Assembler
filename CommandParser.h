
#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <iostream>
#include "Command.h"
#include "ErrorMsg.h"

using namespace std;

class CommandParser {

public:

    vector<ErrorMsg> wrongCommands;
    vector<Command> parseFile(vector<string> lines);
    vector<ErrorMsg> getWrongCommands();

private:
    char validateLineRegex(string line);
    string validateLineSyntax(Command line);
    Command extractData(string line);

    string validateWord(Command command);

    string validateRes(Command command);

    string validateByte(Command command);

    string validateStart(Command command);
};


#endif