#include "LoopManager.h"

PrimaryData LoopManager::loop(string file) {
    string startingAddress;
    int programLength;
    vector<Command> commands = parser.parseFile(LoopManager::reader.readFile(file));
    vector<Command>::iterator it;
    Command command;
    it = commands.begin();
    command = *it;
    vector<string> literalsBuffer;

    if(command.mnemonic != "START") {
        //error
    }
    startingAddress = command.operands.at(0);
    locationCounter = stoi(startingAddress);
    ++it;
    while (it != commands.end()) {
        command = *it;
        if(command.mnemonic == "END"){
            dumpLiterals(literalsBuffer);
            break;
        }
        else if (command.mnemonic == "ORG") {
            locationCounter = getOperandValue(command.operands.front());
        }
        else if (command.mnemonic == "LTORG") {
            dumpLiterals(literalsBuffer);
        }

        if (command.operands.front()[0] == '=') {     //literal operand
            literalsBuffer.push_back(command.operands.front());
        }
        if(command.label != ""){
            if (command.mnemonic == "EQU") {
                symbolTable[command.label] = getOperandValue(command.operands.front());
            }
            else {
                if(LoopManager::symbolTable.find(command.label) != symbolTable.end()){
                    //TODO error
                }
                string temp = getCurrentLocation();
                symbolTable.insert(make_pair(command.label,temp));
            }
        }
        locationCounter += command.getNeededSpace();
        it++;
    }
    programLength = locationCounter - stoi(startingAddress);
    PrimaryData data;
    data.symbolTable = symbolTable;
    data.programLength = programLength;
    data.startingAddress = startingAddress;
    return data;
}

void LoopManager::dumpLiterals(vector<string> literalsBuffer) {
    for(vector<string>::iterator it = literalsBuffer.begin(); it != literalsBuffer.end(); it++)    {
        string literal = *it;
        literalTable.insert(make_pair(literal, getCurrentLocation()));

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
    string temp = to_string(locationCounter);
    while (temp.length() < 4){
        temp = "0" + temp;
    }
}

int LoopManager::getOperandValue(string operand) {
    int value;
    try {
        value = stoi(operand);
    } catch (invalid_argument e) {
        if (LoopManager::symbolTable.find(operand) != symbolTable.end()) {
            value = stoi(symbolTable.find(operand)->second);
        }
        else throw invalid_argument("Invalid value");
    }

    return value;

}