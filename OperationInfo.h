#include <iostream>
using namespace std;

class OperationInfo {
public:
    string code;
    bool hasOperand;
    int numberOfOperands;
    int maxLengthOfOperand;
    int type; // 1 directive , 2 executable
};

