#include <iostream>
#include <map>
#include "FileReader.h"
#include "PrimaryData.h"
#include "labelInfo.h"

using namespace std;
class LoopManager {
private:
    map<string,labelInfo> symbolTable;
    int locationCounter;

    void dumpLiterals(vector<string> literalsBuffer);
    string getCurrentLocation();
    labelInfo getOperandValue(string operand);
public:
    PrimaryData loop(vector<Command> commands);
};