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
    int locationCounter;
public:
    PrimaryData loop(string file);
};