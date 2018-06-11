
#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <iostream>
#include "Command.h"
#include "../DTOs/ErrorMsg.h"

using namespace std;

class CommandParser {

public:

    vector<vector<ErrorMsg>> wrongCommands;
    vector<vector<Command>> parseFile(vector<string> lines);
    vector<vector<ErrorMsg>> getWrongCommands();

private:
    char validateLineRegex(string line);
    string validateLineSyntax(Command line);
    Command extractData(string line);

    string validateWord(Command command);

    string validateRes(Command command);

    string validateByte(Command command);

    string validateStart(Command command);

    string validateLiteral(Command command);
};


#endif