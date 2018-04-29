#include <algorithm>
#include "LoopManager.h"
#include "ErrorMsg.h"

PrimaryData LoopManager::loop(vector<Command> commands, vector<ErrorMsg> wrongCommands) {
    string startingAddress;
    string nameOfProgram;
    vector<Command>::iterator it;
    vector<Command> finalCommands;
    vector<ErrorMsg> newWrongCommands;
    Command command;
    it = commands.begin();
    command = *it;
    programLength = 0;
    vector<string> literalsBuffer;
    int count = 0;
    vector<ErrorMsg>::iterator wrongCommandsIterator = wrongCommands.begin();
    map<int, string> errorMsgsMap;

    if(wrongCommandsIterator != wrongCommands.end()&&(*wrongCommandsIterator).index == count) {
        startingAddress = "0000";
        nameOfProgram = "NONE";
        command.address = "0000";
        locationCounter = 0;
        ++wrongCommandsIterator;
    } else if(command.mnemonic != "START") {
        startingAddress = "0000";
        nameOfProgram = "NONE";
        command.address = "0000";
        locationCounter = 0;
        ErrorMsg msg;
        msg.index = count;
        msg.msg = "INVALID START instruction";
        newWrongCommands.push_back(msg);
        finalCommands.push_back(command);
        ++it;
    } else {
        startingAddress = command.operands.at(0);
        while (startingAddress.length() < 4){
            startingAddress = "0" + startingAddress;
        }
        std::transform(startingAddress.begin(), startingAddress.end(), startingAddress.begin(), ::toupper);
        nameOfProgram = command.label;
        command.address = startingAddress;
        locationCounter = hexToDecimal(startingAddress);
        finalCommands.push_back(command);
        ++it;
    }

    while (it != commands.end()) {
        count++;
        command = *it;
        command.address = getCurrentLocation();
        finalCommands.push_back(command);
        if(wrongCommandsIterator != wrongCommands.end() && (*wrongCommandsIterator).index == count) {
            ++wrongCommandsIterator;
            ++it;
            continue;
        }

        if(command.mnemonic.compare("END") == 0){
            dumpLiterals(literalsBuffer);
            break;
        }
        else if (command.mnemonic.compare("ORG") == 0) {
            try {
                locationCounter = hexToDecimal(getOperandValue(command.operands.front()).address);
            } catch (invalid_argument e) {
                ErrorMsg msg;
                msg.index = count;
                msg.msg = "The label " + command.operands.at(0) + " hasn't been yet defined";
                newWrongCommands.push_back(msg);
                ++it;
                continue;
            }


        }
        else if (command.mnemonic.compare("LTORG") == 0){
            dumpLiterals(literalsBuffer);
        }

        if (command.operands.front()[0] == '=') {     //literal operand
            if (symbolTable.find(command.operands.front()) == symbolTable.end()) {
                literalsBuffer.push_back(command.operands.front());
            }
        }
        if(command.label.compare("") != 0){
            if(command.label.compare(nameOfProgram) == 0) {
                //TODO error
                ErrorMsg msg;
                msg.index = count;
                msg.msg = "The label " + command.label + " has the same name as the program";
                newWrongCommands.push_back(msg);
                ++it;
                continue;

            }
            if (command.mnemonic.compare("EQU") == 0) {
                try {
                    symbolTable[command.label] = getOperandValue(command.operands.front());
                } catch (invalid_argument e) {
                    ErrorMsg msg;
                    msg.index = count;
                    msg.msg = "The label " + command.operands.at(0) + " hasn't been yet defined";
                    newWrongCommands.push_back(msg);
                    ++it;
                    continue;
                }

            }
            else {
                if(LoopManager::symbolTable.find(command.label) != symbolTable.end()){
                    //TODO error
                    ErrorMsg msg;
                    msg.index = count;
                    msg.msg = "The label " + command.label + " is already defined";
                    newWrongCommands.push_back(msg);
                    ++it;
                    continue;
                }
                string temp = getCurrentLocation();
                labelInfo info;
                info.address = temp;
                info.type = "Relative";
                std::pair<std::string,labelInfo> trying = std::make_pair(command.label,info);
                symbolTable.insert(trying);
            }
        }
        locationCounter += command.getNeededSpace();
        programLength += command.getNeededSpace();
        it++;
    }


    for (vector<ErrorMsg>::iterator it = wrongCommands.begin(); it != wrongCommands.end(); it++) {
        ErrorMsg errorMsg = *it;
        errorMsgsMap.insert(make_pair(errorMsg.index, errorMsg.msg));
    }

    for (vector<ErrorMsg>::iterator it = newWrongCommands.begin(); it != newWrongCommands.end(); it++) {
        ErrorMsg errorMsg = *it;
        errorMsgsMap.insert(make_pair(errorMsg.index, errorMsg.msg));
    }

    PrimaryData data;
    data.errorMsgsMap = errorMsgsMap;
    data.symbolTable = symbolTable;
    data.programLength = decimalToHex(programLength);
    data.startingAddress = startingAddress;
    data.commands = finalCommands;
    return data;
}

void LoopManager::dumpLiterals(vector<string> literalsBuffer) {
    for(vector<string>::iterator it = literalsBuffer.begin(); it != literalsBuffer.end(); it++)    {
        string literal = *it;
        labelInfo info;
        info.address = getCurrentLocation();
        info.type = "Relative";

        symbolTable.insert(make_pair(literal,info));
        string value = literal.substr(3, literal.length() - 4);
        char type = literal[1];
        int space = 0;
        if (type == 'C') {
            space = value.length();
        }
        else if (type == 'X') {
            space = (value.length() + 1)/2;
        }
        else if (type == 'W') {
            space = 3;
        }

        locationCounter += space;
        programLength += space;
    }
    literalsBuffer.clear();
}

string LoopManager::getCurrentLocation() {
    string temp = decimalToHex(locationCounter);
    while (temp.length() < 4){
        temp = "0" + temp;
    }
    return temp;
}

labelInfo LoopManager::getOperandValue(string operand) {
    labelInfo info;
    try {
        int value =stoi(operand);
        while (operand.length() < 4) {
            operand = "0" + operand;
        }
        info.address = operand;
        info.type = "Absolute";
    } catch (invalid_argument e) {
        if (operand == "*") {
            info.address = getCurrentLocation();
            info.type = "Absolute";
        }
        else if (LoopManager::symbolTable.find(operand) != symbolTable.end()) {
            string tmpValue = symbolTable.find(operand)->second.address;
            info.address = tmpValue;
            info.type = symbolTable.find(operand)->second.type;
        }
        else throw invalid_argument("Invalid value");
    }
    return info;
}

int LoopManager::hexToDecimal(string hexValue) {
    int decimalValue;
    std::stringstream ss;
    ss  << hexValue;
    ss >> std::hex >> decimalValue;

    return decimalValue;
}

string LoopManager::decimalToHex(int decimalValue) {
    std::stringstream ss;
    ss << std::hex << decimalValue;
    std::string res ( ss.str() );
    std::transform(res.begin(), res.end(), res.begin(), ::toupper);
    return res;
}