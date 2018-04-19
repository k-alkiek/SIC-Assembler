#include <iostream>
using namespace std;

class OperationInfo {
public:
    string code;
    bool hasOperand;
    int numberOfOperands;
    OperationInfo(string code, bool hasOperand, int numberOfOperands);
};

