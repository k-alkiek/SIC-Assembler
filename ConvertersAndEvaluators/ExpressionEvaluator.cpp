
#include <vector>
#include <stack>
#include "ExpressionEvaluator.h"
#include "../Logger/Logger.h"
#include <algorithm>

Logger loggerExpressionEval;

ExpressionEvaluator::ExpressionEvaluator(map<string, labelInfo> symtable, HexaConverter converter) {
    this->symtable = symtable;
    this->converter = converter;
}

OperandHolder ExpressionEvaluator::evaluateExpression(string expression, string locctr) {

    if (expression.length() == 0) {
        loggerExpressionEval.errorMsg("ExpressionEvaluator: Empty expression");
        __throw_runtime_error("Empty expression");
    }
    labelInfo locctr_label;
    locctr_label.type = "Relative";
    locctr_label.address = locctr;
    this->symtable.insert(pair<string, labelInfo>("LOCCTR", locctr_label));

    vector<string> labels = {};
    vector<char> operators = {};

    parse_expression(&labels, &operators, expression);

    OperandHolder holder = evaluate(&labels, &operators);

    if(holder.type == 1 || holder.type == 0)
        return holder;
    __throw_runtime_error("Invalid Expression");
}

bool ExpressionEvaluator::isOperator(char c) {
    if (c == '+' || c == '-' || c == '/' || c == '*')
        return true;
    return false;
}

void ExpressionEvaluator::parse_expression(vector<string> *labels, vector<char> *operators, string expression) {

    string label_container;

    for (char c : expression) {
        if (isOperator(c)) {
            if (label_container.empty()) // operator at the beginning of expression
            {
                if (c == '*')
                    label_container = "LOCCTR";
                else {
                    loggerExpressionEval.errorMsg("ExpressionEvaluator: Operator Problem in Expression");
                    __throw_runtime_error("Operator Problem in Expression");
                }
            }
            if (label_container != "LOCCTR")
                operators->push_back(c);
            labels->push_back(label_container);
            label_container = "";
        } else if (isdigit(c) || isalpha(c))
            label_container += c;
        else {
            loggerExpressionEval.errorMsg("ExpressionEvaluator: Invalid symbol in expression");
            __throw_runtime_error("Invalid symbol in expression");
        }
    }
    if (!(labels->size() == 1 && labels->at(0) == "LOCCTR"))
        labels->push_back(label_container);

    if (labels->size() != (*operators).size() + 1) {
        loggerExpressionEval.errorMsg("ExpressionEvaluator: Incompatible number of operands to operations");
        __throw_runtime_error("Incompatible number of operands to operations");
    }

}


OperandHolder ExpressionEvaluator::evaluate(vector<string> *labels, vector<char> *operators) {

    vector<OperandHolder> expression = form_operands(labels);
    stack<OperandHolder> operands;
    stack<char> operations;

    int op_count = 0, exp_count = 0, j = 0;
    for (int i = 0; i < operators->size() + expression.size(); i++) {
        if (j == 0) {
            operands.push(expression[exp_count]);
            exp_count++;
            j = 1;
        } else {
            char op = (*operators)[op_count];
            if (operations.empty())
                operations.push(op);
            else {
                while (!operations.empty() && is_higher(operations.top(), op)) {
                    OperandHolder operand2 = operands.top();
                    operands.pop();
                    OperandHolder operand1 = operands.top();
                    operands.pop();

                    operands.push(calculate(operand1, operand2, operations.top()));

                    operations.pop();
                }
                operations.push(op);
            }
            op_count++;
            j = 0;
        }
    }

    while (!operations.empty()) {
        OperandHolder operand2 = operands.top();
        operands.pop();
        OperandHolder operand1 = operands.top();
        operands.pop();

        operands.push(calculate(operand1, operand2, operations.top()));

        operations.pop();
    }

    return operands.top();
}

vector<OperandHolder> ExpressionEvaluator::form_operands(vector<string> *labels) {

    vector<OperandHolder> operands = {};
    for (int i = 0; i < labels->size(); i++) {
        if (this->symtable.find((*labels)[i]) == this->symtable.end())
            if (isdigit((*labels)[i][0]))
                operands.push_back(OperandHolder((*labels)[i], 0, 1));
            else if (std::find(extref_tab.begin(), extref_tab.end(), (*labels)[i]) != this->extref_tab.end())
                operands.push_back(OperandHolder("0", 0, 1));
            else {
                loggerExpressionEval.errorMsg("ExpressionEvaluator: Undefined label");
                __throw_runtime_error("Undefined label");
            }
        else {
            labelInfo label = this->symtable[(*labels)[i]];
            int type;
            if (label.type == "Relative")
                type = 1;
            else
                type = 0;
            operands.push_back(OperandHolder(label.address, type, 1));
        }

    }

    return operands;
}

bool ExpressionEvaluator::is_higher(char top, char op) {
    if (top == '+' || top == '-')
        if (op == '+' || op == '-')
            return true;
        else
            return false;
    return true;
}

OperandHolder ExpressionEvaluator::calculate(OperandHolder operand1, OperandHolder operand2, char operation) {

    if (operation == '+') {
        int type = operand1.type + operand2.type;
        /*if (type > 1) {
            loggerExpressionEval.errorMsg("ExpressionEvaluator: Invalid Expression");
            __throw_runtime_error("Invalid Expression");
        }*/

        int address = this->converter.hexToDecimal(operand1.value) * operand1.sign;
        address += this->converter.hexToDecimal(operand2.value) * operand2.sign;

        int sign = 1;
        if(address < 0)
            sign = -1;
        return OperandHolder(this->converter.decimalToHex(sign*address), type,sign);
    } else if (operation == '-') {
        int type = operand1.type - operand2.type;
        /*if (type < 0) {
            loggerExpressionEval.errorMsg("ExpressionEvaluator: Invalid Expression");
            __throw_runtime_error("Invalid Expression");
        }
*/
        int address = this->converter.hexToDecimal(operand1.value) * operand1.sign;
        address -= this->converter.hexToDecimal(operand2.value) * operand2.sign;

        int sign = 1;
        if(address < 0)
            sign = -1;
        return OperandHolder(this->converter.decimalToHex(sign*address), type,sign);
    } else if (operation == '*') {
        if (operand1.type != 0 || operand2.type != 0) {
            loggerExpressionEval.errorMsg("ExpressionEvaluator: Invalid Expression");
            __throw_runtime_error("Invalid Expression");
        }

        int address = this->converter.hexToDecimal(operand1.value) * operand1.sign;
        address *= this->converter.hexToDecimal(operand2.value) * operand2.sign;

        int sign = 1;
        if(address < 0)
            sign = -1;
        return OperandHolder(this->converter.decimalToHex(sign*address), 0,sign);
    } else {
        if (operand1.type != 0 || operand2.type != 0) {
            loggerExpressionEval.errorMsg("ExpressionEvaluator: Invalid Expression");
            __throw_runtime_error("Invalid Expression");
        }

        int address = this->converter.hexToDecimal(operand1.value) * operand1.sign;
        address /= this->converter.hexToDecimal(operand2.value) * operand2.sign;

        int sign = 1;
        if(address < 0)
            sign = -1;
        return OperandHolder(this->converter.decimalToHex(sign*address), 0,sign);
    }


}


