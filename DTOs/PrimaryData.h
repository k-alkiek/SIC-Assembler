
#ifndef PRIMARYDATA_H
#define PRIMARYDATA_H

#include <iostream>
#include <map>
#include "../CommandsAndUtilities/Command.h"
#include "labelInfo.h"
#include "ExternalSymbolInfo.h"
#include "Literal.h"


using namespace std;
class PrimaryData {
public:
    string startingAddress;
    map<string, labelInfo> symbolTable;
    map<string, Literal> literalTable;
    string programLength;
    vector<Command> commands;
    map<int, string> errorMsgsMap;
    string programName;
    map<string, ExternalSymbolInfo> externalSymbols;
    map<string, ExternalSymbolInfo> externalReference;
};

#endif