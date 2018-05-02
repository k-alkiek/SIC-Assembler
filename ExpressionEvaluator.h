#ifndef ASSEMBLER_EXPRESSIONEVALUATOR_H
#define ASSEMBLER_EXPRESSIONEVALUATOR_H

#include <iostream>
#include <map>
#include "labelInfo.h"

using namespace std;
class ExpressionEvaluator {
public:
    string evaluateExpression(
            string expression,map<string,labelInfo> symtable,map<string,string> litTable); // throws exception if illegal
};


#endif 