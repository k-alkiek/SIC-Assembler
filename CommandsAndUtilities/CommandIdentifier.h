#include <iostream>
#include <map>
#include "../DTOs/OperationInfo.h"

using namespace std;
class CommandIdentifier {
private:
    map<string,OperationInfo> opTable;
public:
    CommandIdentifier();//initialize op table
    bool isInTable(string command);
    OperationInfo getInfo(string command);
};

