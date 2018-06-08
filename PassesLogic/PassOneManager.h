#include <iostream>
#include <map>
#include "../DTOs/PrimaryData.h"
#include "../DTOs/labelInfo.h"
#include "../DTOs/ErrorMsg.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"
#include "../DTOs/Literal.h"

using namespace std;
class PassOneManager {
private:
    map<string, labelInfo> symbolTable;
    map<string, Literal> literalTable;
    int locationCounter;
    int programLength;
    HexaConverter hexaConverter;
    vector<ErrorMsg> dumpLiterals(vector<pair<string, int>> literalsBuffer);
    string getCurrentLocation();
    labelInfo getOperandValue(string operand);

public:
    PrimaryData loop(vector<Command> commands, vector<ErrorMsg> wrongCommands);
};