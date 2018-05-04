#include <iostream>
#include <map>
#include "../FileManipulators/FileReader.h"
#include "../DTOs/PrimaryData.h"
#include "../DTOs/labelInfo.h"
#include "../DTOs/ErrorMsg.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"

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