#include <iostream>
#include "Command.h"

using namespace std;

class CommandParser {

public:
    vector<Command> parseFile(vector<string> lines);
};

