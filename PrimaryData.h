
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
    int programLength;
    vector<Command> commands;
};

#endif