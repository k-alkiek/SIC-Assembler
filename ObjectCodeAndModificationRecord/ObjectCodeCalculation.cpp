//
// Created by saraheldafrawy on 04/06/18.
//

#include "ObjectCodeCalculation.h"
#include "../CommandsAndUtilities/CommandIdentifier.h"
#include "../ConvertersAndEvaluators/ExpressionEvaluator.h"
#include "../DTOs/ExternalSymbolInfo.h"
#include <cmath>


string nextInstructAddress;
string currentInstructionAddress;
map<string, labelInfo> symblTable;
HexaConverter hexConverter;
vector<string> extRef;
bool isPc;


//TODO litrals
string ObjectCodeCalculation::getObjectCode(Command cursor, string nextInstAdd, string currentInstAdd, map<string, labelInfo> symTable,bool isPcFlag,vector<string> externalReference) {
    ExpressionEvaluator expressionEvaluator(symblTable, hexConverter);
    OperandHolder operandHolder("", 0);
    nextInstructAddress = nextInstAdd;
    symblTable = symTable;
    extRef = externalReference;
    currentInstructionAddress = currentInstAdd;
    isPc = isPcFlag;
    CommandIdentifier opTable;
    if (cursor.mnemonic == "WORD") {
        string obcode = "";
        for (int i = 0; i < cursor.operands.size(); i++) {
            if(isExpression(cursor.operands[i])){
                operandHolder = expressionEvaluator.evaluateExpression(cursor.operands[i],currentInstAdd);
                string value = "000000" + operandHolder.value;
                obcode += value.substr(value.length()-6,value.length()-1);
            } else{
                if(containsExternalReference(cursor.operands[i],externalReference)){
                    obcode += "000000";
                } else{
                    string value = "000000" + hexConverter.decimalToHex(stoi(cursor.operands[i]));
                    obcode += value.substr(value.length()-6,value.length()-1);
                }
            }
        }
        return obcode;
    } else if (cursor.mnemonic == "BYTE") {
        string operand;
        if (cursor.operands.size() != 1) {
            __throw_runtime_error("Invalid operands");
        } else {
            operand = cursor.operands[0];
        }
        if (operand.front() == 'X' && operand[1] == '\'') {
            return operand.substr(2, operand.length() - 3);
        } else if (operand.front() == 'C' && operand[1] == '\'') {
            return convertCToObjCode(operand.substr(2, operand.length() - 3));
        } else {
            __throw_runtime_error("Invalid type");
        }
    } else if (opTable.isInTable(cursor.mnemonic)) {
        OperationInfo operationInfo = opTable.getInfo(cursor.mnemonic);
        int commandObjCode = hexConverter.hexToDecimal(operationInfo.code);
        int format = operationInfo.format;

        if (format == 1) {
            return hexConverter.decimalToHex(commandObjCode);
        } else if (format == 2) {
            return completeObjCodeFormat2(commandObjCode, cursor.operands);
        } else if (format == 3 && cursor.operands[0].front() != '+') {
            return completeObjCodeFormat3(commandObjCode, cursor.operands,isPc);
        } else {
            return completeObjCodeFormat4(commandObjCode, cursor.operands);
        }
    }
}

string ObjectCodeCalculation::completeObjCodeFormat2(int uncompletedObjCode, vector<string> operands) {
    int registerCode = getRegisterNumber(operands[0]);
    if (operands.size() != 1) {
        registerCode = registerCode << 4;
        registerCode = registerCode | getRegisterNumber(operands[1]);
        uncompletedObjCode = uncompletedObjCode << 8;
    } else {
        uncompletedObjCode = uncompletedObjCode << 4;
    }
    uncompletedObjCode = uncompletedObjCode | registerCode;
    return hexConverter.decimalToHex(uncompletedObjCode);
}

string ObjectCodeCalculation::completeObjCodeFormat3(int uncompletedObjCode, vector<string> operands, bool baseAvailable) {
    ExpressionEvaluator expressionEvaluator(symblTable, hexConverter);
    OperandHolder operandHolder("", 0);
    labelInfo label;
    int displacement = 0;
    bool isPC;
    bool constImmediateOrIndirect = true;
    vector<int> results;
    if (operands.size() != 0) {
        bool isAnExpression = isExpression(operands[0]);
        string address;
        if (isAnExpression) {
            operandHolder = expressionEvaluator.evaluateExpression(operands[0], currentInstructionAddress);
            if(operandHolder.type == 0){
                __throw_runtime_error("absolute expression in format 3 instruction");
            }
            address = operandHolder.value;
        } else if (operands[0][0] != '#' && operands[0][0] != '@' && symblTable.find(operands[0]) != symblTable.end()) {
            label = symblTable.at(operands[0]);
            address = label.address;
        } else if ((operands[0][0] == '#' || operands[0][0] == '@')) {
            if(symblTable.find(operands[0].substr(1, operands[0].length() - 1)) != symblTable.end()) {
                label = symblTable.at(operands[0].substr(1, operands[0].length() - 1));
                address = label.address;
            } else if(false/*litTable.find(operands[0].substr(1, operands[0].length() - 1)) != symblTable.end()*/){ //TODO barie must have finished litTable
               // label = litTable.at(operands[0].substr(1, operands[0].length() - 1));
                address = label.address;
            } else if(is_number(operands[0].substr(1, operands[0].length() - 1))){
                displacement = stoi(operands[0].substr(1, operands[0].length() - 1));
                if(baseAvailable){
                    if(displacement > 4095){
                        __throw_runtime_error("Displacement out of range");
                    }
                    isPC = false;
                } else {
                    if (displacement < 2048 || displacement >= -2048) {
                        isPC = true;
                    } else if(displacement > 2047){
                        __throw_runtime_error("Displacement out of range");
                    }
                }

                constImmediateOrIndirect = false;
            } else{
                __throw_runtime_error("operand is not in symTable or LitTable");
            }

        } else {
            __throw_runtime_error("Error");
        }
        if (constImmediateOrIndirect) {
            results = getSimpleDisplacement(address, nextInstructAddress,baseAvailable);
            if (results[0] == 1) {
                isPC = true;
            } else {
                isPC = false;
            }
            displacement = results[1];
        }
        vector<int> nixbpe = getFlagsCombination(operands, 3, isPC); // give me ni separated from xbpe
        unsigned int completedObjCode = ((uncompletedObjCode | nixbpe[0]) << 4) | nixbpe[1];
        completedObjCode = (completedObjCode << 12) | ((displacement << 20) >> 20);
        string final = "000000" + hexConverter.decimalToHex(completedObjCode);
        return (final).substr(final.length() - 6, final.length() - 1);
    } else { //TODO check if its correct
        return "4C0000"; //return opcode only ex: 1027 RSUB 4C0000 (got it from optable)
    }
}

string ObjectCodeCalculation::completeObjCodeFormat4(int uncompletedObjCode, vector<string> operands) {
    vector<int> nixbpe = getFlagsCombination(operands, 4, false);// give me ni separated from xbpe
    labelInfo label;
    ExpressionEvaluator expressionEvaluator(symblTable, hexConverter);
    OperandHolder operandHolder("", 0);
    string address;
    if (operands.size() != 0) {
        bool isAnExpression = isExpression(operands[0]);
        string address;
        if (isAnExpression) {
            operandHolder = expressionEvaluator.evaluateExpression(operands[0], currentInstructionAddress);
            address = operandHolder.value;
        } else if ((operands[0][0] == '#' || operands[0][0] == '@')) {
            if(symblTable.find(operands[0].substr(1, operands[0].length() - 1)) != symblTable.end()) {
                label = symblTable.at(operands[0].substr(1, operands[0].length() - 1));
                address = label.address;
            } else if(containsExternalReference(operands[0].substr(1, operands[0].length() - 1),extRef)){
                address = "00000";
            } else if(is_number(operands[0].substr(1, operands[0].length() - 1))){
                address = stoi(operands[0].substr(1, operands[0].length() - 1));
                if (stoi(address) > pow(2, 20)) {
                    __throw_runtime_error("address is bigger than 20 bit");
                }
            }

        } else {
            if(symblTable.find(operands[0]) != symblTable.end()) {
                label = symblTable.at(operands[0]);
                address = label.address;
            } else if(containsExternalReference(operands[0],extRef)){
                address = "00000";
            } else if(is_number(operands[0])){
                address = stoi(operands[0].substr(1, operands[0].length() - 1));
                if (stoi(address) > pow(2, 20)) {
                    __throw_runtime_error("address is bigger than 20 bit");
                }
            } else{
                __throw_runtime_error("Invaled operand");
            }
        }
        unsigned int completedObjCode = ((((uncompletedObjCode >> 2) << 2) | nixbpe[0]) << 4) |
                                        nixbpe[1]; //deleted first two bits from the right (enta sa7 :D) (i knew it :p)
        completedObjCode = (completedObjCode << 20) | ((stoi(address) << 12) >> 12);
        return hexConverter.decimalToHex(completedObjCode);
    } else {
        __throw_runtime_error("RSUB No Rsub in format 4 ");
    }
}

vector<int> ObjectCodeCalculation::getFlagsCombination(vector<string> operands, int format, bool PCRelative) {
    int ni = 0;
    int xbpe = 0;
    if (operands.size() == 2) {
        if (operands[1] == "X") {
            xbpe = 8;       //indexing
        } else {
            __throw_runtime_error("Wrong number of operands");
        }
    }
    if (format == 3) {
        if (operands[0].front() == '#') {
            if (xbpe == 8) {
                __throw_runtime_error("ERROR can't have immediate with X");
            }
            ni = 1;             //01 immediate
        } else if (operands[0].front() == '@') {
            if (xbpe == 8) {
                __throw_runtime_error("ERROR can't have indirect with X");
            }
            ni = 2;  //10 indirect
        } else {
            ni = 3; //11 simple addressing
        }
        if (PCRelative) {
            xbpe = xbpe | 2; //pc relative
        } else {
            xbpe = xbpe | 4; //base relative
        }

    } else { //format 4
        ni = 3;
        xbpe = xbpe | 1;
    }
    vector<int> returnedValue;
    returnedValue.push_back(ni);
    returnedValue.push_back(xbpe);
    return returnedValue;
}

int ObjectCodeCalculation::getRegisterNumber(string registerr) {
    if (registerr.compare("A") == 0) {
        return 0;
    } else if (registerr.compare("X") == 0) {
        return 1;
    } else if (registerr.compare("L") == 0) {
        return 2;
    } else if (registerr.compare("B") == 0) {
        return 3;
    } else if (registerr.compare("S") == 0) {
        return 4;
    } else if (registerr.compare("T") == 0) {
        return 5;
    } else if (registerr.compare("F") == 0) {
        return 6;
    } else if (registerr.compare("PC") == 0) {
        return 8;
    } else if (registerr.compare("SW") == 0) {
        return 9;
    } else {
        return 10;
    }
}

vector<int> ObjectCodeCalculation::getSimpleDisplacement(string TA, string progCounter,bool baseAvailable) {
    vector<int> results;
    int displacement = 0;
    int isPC = 1;
    int targetAdd = hexConverter.hexToDecimal(TA);
    int programCount = hexConverter.hexToDecimal(progCounter);
    displacement = targetAdd - programCount;

    if(baseAvailable){
        if(displacement > 4095 || displacement < 0){
            __throw_runtime_error("Displacement out of range");
        }
        isPC = false;
    } else {
        if (displacement <= 2047 && displacement > -2048) { //error law -4000
            isPC = true;
        } else {
            __throw_runtime_error("Displacement out of range");
        }
    }
    results.push_back(isPC);
    results.push_back(displacement);
    return results;
}

bool ObjectCodeCalculation::isExpression(string operand) {
    if (operand.find('+') != std::string::npos
        || operand.find('-') != std::string::npos
        || (operand.find('*') != std::string::npos && operand.length() != 1)
        || operand.find('/') != std::string::npos) {
        return true;
    }
    return false;
}

string ObjectCodeCalculation::convertCToObjCode(string str) {
    string asciiString = "";
    for (int i = 0; i < str.length(); i++) {
        asciiString += hexConverter.decimalToHex(str[i]);
    }
    return asciiString;
}

bool ObjectCodeCalculation::is_number(string s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

vector<ExternalSymbolInfo> ObjectCodeCalculation::getDataFromMap(map<string, ExternalSymbolInfo> externalReference){
    vector<ExternalSymbolInfo> data;
    for(std::map<string, ExternalSymbolInfo>::iterator it = externalReference.begin(); it != externalReference.end(); ++it) {
        data.push_back(externalReference.at(it->first));
    }
    return data;
}

bool ObjectCodeCalculation::containsExternalReference (string expression, vector<string> extReferences) {
    for (int i = 0; i < extReferences.size(); i++) {
        if (expression.find(extReferences[i]) != string::npos) {
            return true;
        }
    }
    return false;
}