#include <iostream>
using namespace std;

class OperationInfo {
public:
    string code;
    bool hasOperand;
    int numberOfOperands;
    int format;
    OperationInfo(string code, bool hasOperand, int numberOfOperands, int format);
};

