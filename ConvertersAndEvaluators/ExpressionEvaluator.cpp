
#include <vector>
#include <stack>
#include "ExpressionEvaluator.h"

ExpressionEvaluator::ExpressionEvaluator(map<string, labelInfo> symtable, HexaConverter converter) {
    this->symtable = symtable;
    this->converter = converter;
}

OperandHolder ExpressionEvaluator::evaluateExpression(string expression, string locctr) {

    if(expression.length() == 0)
        __throw_runtime_error("Empty expression");

    labelInfo locctr_label;
    locctr_label.type = "relative";
    locctr_label.address = locctr;
    this->symtable.insert(pair<string, labelInfo>("LOCCTR",locctr_label));

    vector<string> labels = {};
    vector<char> operators = {};

    parse_expression(&labels, &operators, expression);

    return evaluate(&labels, &operators);
}

bool ExpressionEvaluator::isOperator(char c) {
    if(c=='+' || c=='-' || c=='/' || c=='*')
        return true;
    return false;
}

void ExpressionEvaluator::parse_expression(vector<string>* labels, vector<char>* operators, string expression) {

    string label_container;

    for(char c : expression)
    {
        if(isOperator(c))
        {
            if(label_container.empty()) // operator at the beginning of expression
            {
                if(c == '*')
                    label_container == "LOCCTR";
                else
                    __throw_runtime_error("Operator Problem in Expression");
            }
            if(label_container != "LOCCTR")
                operators->push_back(c);
            labels->push_back(label_container);
            label_container = "";
        }
        else if(isdigit(c) || isalpha(c))
            label_container += c;
        else
            __throw_runtime_error("Invalid symbol in expression");
    }
    labels->push_back(label_container);

    if(labels->size() != (*operators).size()+1)
        __throw_runtime_error("Incompatible number of operands to operations");

}


OperandHolder ExpressionEvaluator::evaluate(vector<string>* labels, vector<char>* operators) {

    vector<OperandHolder> expression = form_operands(labels);
    stack<OperandHolder> operands;
    stack<char> operations;

    int op_count = 0, exp_count = 0, j = 0;
    for(int i = 0 ; i < operators->size() + expression.size(); i++)
    {
        if(j == 0)
        {
            operands.push(expression[exp_count]);
            exp_count++;
            j = 1;
        }
        else
        {
            char op = (*operators)[op_count];
            if(operations.empty())
                operations.push(op);
            else
            {
                while(!operations.empty() && is_higher(operations.top(),op))
                {
                    OperandHolder operand2 = operands.top();
                    operands.pop();
                    OperandHolder operand1 = operands.top();
                    operands.pop();

                    operands.push(calculate(operand1, operand2,operations.top()));

                    operations.pop();
                }
                operations.push(op);
            }
            op_count++;
            j = 0;
        }
    }

    while(!operations.empty())
    {
        OperandHolder operand2 = operands.top();
        operands.pop();
        OperandHolder operand1 = operands.top();
        operands.pop();

        operands.push(calculate(operand1, operand2,operations.top()));

        operations.pop();
    }

    return operands.top();
}

vector<OperandHolder> ExpressionEvaluator::form_operands(vector<string>* labels) {

    vector<OperandHolder> operands = {};
    for(int i = 0 ; i < labels->size(); i++)
    {
        if(this->symtable.find((*labels)[i]) == this->symtable.end())
            operands.push_back(OperandHolder((*labels)[i],0));
        else
        {
            labelInfo label = this->symtable[(*labels)[i]];
            int type;
            if(label.type == "relative")
                type = 1;
            else
                type = 0;
            operands.push_back(OperandHolder(label.address,type));
        }

    }

    return operands;
}

bool ExpressionEvaluator::is_higher(char top, char op) {
    if(top == '+' || top == '-')
        if(op  == '+' || op == '-')
            return true;
        else
            return false;
    return true;
}

OperandHolder ExpressionEvaluator::calculate(OperandHolder operand1, OperandHolder operand2, char operation) {

    if(operation == '+')
    {
        int type = operand1.type + operand2.type;
        if(type > 1)
            __throw_runtime_error("Invalid Expression");

        int address = this->converter.hexToDecimal(operand1.value);
        address+= this->converter.hexToDecimal(operand2.value);

        return OperandHolder(this->converter.decimalToHex(address),type);
    }
    else if(operation == '-')
    {
        int type = operand1.type - operand2.type;
        if(type < 0)
            __throw_runtime_error("Invalid Expression");

        int address = this->converter.hexToDecimal(operand1.value);
        address-= this->converter.hexToDecimal(operand2.value);

        return OperandHolder(this->converter.decimalToHex(address),type);
    }
    else if(operation == '*')
    {
        if(operand1.type || operand2.type)
            __throw_runtime_error("Invalid Expression");

        int address = this->converter.hexToDecimal(operand1.value);
        address*= this->converter.hexToDecimal(operand2.value);

        return OperandHolder(this->converter.decimalToHex(address),0);
    }
    else
    {
        if(operand1.type || operand2.type)
            __throw_runtime_error("Invalid Expression");

        int address = this->converter.hexToDecimal(operand1.value);
        address/= this->converter.hexToDecimal(operand2.value);

        return OperandHolder(this->converter.decimalToHex(address),0);
    }



}


