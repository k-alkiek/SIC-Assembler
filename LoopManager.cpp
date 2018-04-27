#include "LoopManager.h"

PrimaryData LoopManager::loop(vector<Command> commands) {
    string startingAddress;
    int programLength;
    string nameOfProgram;
    vector<Command>::iterator it;
    vector<Command> finalCommands;
    Command command;
    it = commands.begin();
    command = *it;
    vector<string> literalsBuffer;

    if(command.mnemonic != "START") {
        //TODO error
    }
    startingAddress = command.operands.at(0);
    nameOfProgram = command.label;
    command.address = startingAddress;
    locationCounter = hexToDecimal(startingAddress);
    finalCommands.push_back(command);
    ++it;
    while (it != commands.end()) {
        command = *it;
        command.address = getCurrentLocation();
        finalCommands.push_back(command);
        if(command.mnemonic.compare("END") == 0){
            dumpLiterals(literalsBuffer);
            break;
        }
        else if (command.mnemonic.compare("ORG") == 0) {
            locationCounter = hexToDecimal(getOperandValue(command.operands.front()).address);
        }
        else if (command.mnemonic.compare("LTORG") == 0){
            dumpLiterals(literalsBuffer);
        }

        if (command.operands.front()[0] == '=') {     //literal operand
            literalsBuffer.push_back(command.operands.front());
        }
        if(command.label.compare("") != 0){
            if(command.label.compare(nameOfProgram) == 0) {
                //TODO error
            }
            if (command.mnemonic.compare("EQU") == 0) {
                symbolTable[command.label] = getOperandValue(command.operands.front());
            }
            else {
                if(LoopManager::symbolTable.find(command.label) != symbolTable.end()){
                    //TODO error
                }
                string temp = getCurrentLocation();
                labelInfo info;
                info.address = temp;
                info.type = "relative";
                std::pair<std::string,labelInfo> trying = std::make_pair(command.label,info);
                 symbolTable.insert(trying);
            }
        }
        locationCounter += command.getNeededSpace();
        it++;
    }
    programLength = locationCounter - hexToDecimal(startingAddress);
    PrimaryData data;
    data.symbolTable = symbolTable;
    data.programLength = programLength;
    data.startingAddress = startingAddress;
    data.commands = finalCommands;
    return data;
}

void LoopManager::dumpLiterals(vector<string> literalsBuffer) {
    for(vector<string>::iterator it = literalsBuffer.begin(); it != literalsBuffer.end(); it++)    {
        string literal = *it;
        labelInfo info;
        info.address = getCurrentLocation();
        info.type = "relative";

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
        info.address = operand;
        info.type = "Absolute";
    } catch (invalid_argument e) {
        if (LoopManager::symbolTable.find(operand) != symbolTable.end()) {
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

    return res;
}