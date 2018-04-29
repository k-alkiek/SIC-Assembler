
#ifndef PRIMARYDATA_H
#define PRIMARYDATA_H

#include <iostream>
#include <map>
#include "Command.h"
#include "labelInfo.h"


using namespace std;
class PrimaryData {
public:
    string startingAddress;
    map<string,labelInfo> symbolTable;
    string programLength;
    vector<Command> commands;
    map<int, string> errorMsgsMap;
};

#endif