#include <iostream>
#include <map>
#include "FileReader.h"
#include "PrimaryData.h"
#include "labelInfo.h"
#include "ErrorMsg.h"
#include "HexaConverter.h"

using namespace std;
class PassOneManager {
private:
    map<string,labelInfo> symbolTable;
    int locationCounter;
    int programLength;
    HexaConverter hexaConverter;
    void dumpLiterals(vector<string> literalsBuffer);
    string getCurrentLocation();
    labelInfo getOperandValue(string operand);

public:
    PrimaryData loop(vector<Command> commands, vector<ErrorMsg> wrongCommands);
};