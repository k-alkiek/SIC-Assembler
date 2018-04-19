#include <iostream>
#include <map>
#include "OperationInfo.h"

using namespace std;
class CommandIdentifier {
private:
    static map<string,OperationInfo> opTable;
public:
    CommandIdentifier();//initialize op table
    static bool isInTable(string command);
    static OperationInfo getInfo(string command);
};

