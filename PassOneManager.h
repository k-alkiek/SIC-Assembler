#include <iostream>
#include <map>
#include "FileReader.h"
#include "PrimaryData.h"
#include "labelInfo.h"
#include "ErrorMsg.h"

using namespace std;
class PassOneManager {
private:
    map<string,labelInfo> symbolTable;
    int locationCounter;
    int programLength;

    void dumpLiterals(vector<string> literalsBuffer);
    string getCurrentLocation();
    labelInfo getOperandValue(string operand);
    int hexToDecimal(string hexValue);
    string decimalToHex(int decimalValue);
public:
    PrimaryData loop(vector<Command> commands, vector<ErrorMsg> wrongCommands);
};