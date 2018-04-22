#include <iostream>
#include <map>
#include "FileReader.h"
#include "CommandParser.h"
#include "PrimaryData.h"

using namespace std;
class LoopManager {
private:
    FileReader reader;
    CommandParser parser;
    map<string,string> symbolTable;
    map<string,string> literalTable;
    int locationCounter;

    void dumpLiterals(vector<string> literalsBuffer);
    string getCurrentLocation();
    int getOperandValue(string operand);
public:
    PrimaryData loop(string file);
};