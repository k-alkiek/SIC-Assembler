#include <iostream>
#include <map>
#include <vector>
#include "HexaConverter.h"
#include "../DTOs/labelInfo.h"
#include "../DTOs/OperandHolder.h"
#include "../DTOs/ExternalSymbolInfo.h"

using namespace std;
class ExpressionEvaluator {
public:
    map<string,labelInfo> symtable;
    vector<string> extref_tab;
    HexaConverter converter;

    ExpressionEvaluator(map<string,labelInfo> symtable, HexaConverter converter);
    OperandHolder evaluateExpression(string expression, string locctr); // throws exception if illegal

private:
    bool isOperator(char c);
    void parse_expression(vector<string>* labels, vector<char>* operators, string expression);
    OperandHolder evaluate(vector<string>* labels, vector<char>* operators);

    vector<OperandHolder> form_operands(vector<string> *labels);
    bool is_higher(char top, char op);

    OperandHolder calculate(OperandHolder operand1, OperandHolder operand2, char operation);
};
