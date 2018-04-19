#include <iostream>
#include <map>
#include "OperationInfo.h"

using namespace std;

class CommandIdentifier {
private:
    map<string,OperationInfo> opTable;
    string currentCommand;
public:
    CommandIdentifier();//initialize op table
    bool isInTable(string command);
    OperationInfo getInfo(string command);
};

