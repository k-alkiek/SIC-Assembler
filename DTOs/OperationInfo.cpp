
#include "OperationInfo.h"

OperationInfo::OperationInfo(string code1, bool hasOperand1, int numberOfOperands1, int format1) {
    code = code1;
    hasOperand = hasOperand1;
    numberOfOperands = numberOfOperands1;
    format = format1;
}
