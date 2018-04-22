#include <iostream>
#include "Command.h"

using namespace std;

class CommandParser {

public:

    vector<Command> parseFile(vector<string> lines);

private:
    char validateLineRegex(string line);
    bool validateLineSyntax(Command line);
    Command extractData(string line);

    bool validateWord(Command command);

    bool validateRes(Command command);

    bool validateByte(Command command);
};


