#include <algorithm>
#include "PassOneManager.h"
#include "../DTOs/ErrorMsg.h"
#include "../DTOs/SymbolPosition.h"
#include "../DTOs/ExternalSymbolInfo.h"
#include <stdio.h>
PrimaryData PassOneManager::loop(vector<Command> commands, vector<ErrorMsg> wrongCommands) {
    string startingAddress;
    string nameOfProgram;
    vector<Command>::iterator it;
    vector<Command> finalCommands;
    vector<ErrorMsg> newWrongCommands;
    Command command;
    it = commands.begin();
    command = *it;
    programLength = 0;
    vector<pair<string, int>> literalsBuffer;
    int count = 0;
    vector<ErrorMsg>::iterator wrongCommandsIterator = wrongCommands.begin();
    map<int, string> errorMsgsMap;

    vector<SymbolPosition> externalDefinitions;
    map<string, ExternalSymbolInfo> externalSymbolsInfo;

    vector<SymbolPosition> externalReferences;
    map<string, ExternalSymbolInfo> externalReferenceInfo;

    bool endFound = false;

    if (wrongCommandsIterator != wrongCommands.end() && (*wrongCommandsIterator).index == count) {
        startingAddress = "0000";
        nameOfProgram = "NONE";
        command.address = "0000";
        locationCounter = 0;
        ++wrongCommandsIterator;
        ++it;
        finalCommands.push_back(command);
    } else if (command.mnemonic.compare("START") != 0 && command.mnemonic.compare("CSECT") != 0) {
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
        if (command.mnemonic.compare("CSECT") != 0) {
            startingAddress = command.operands.at(0);
        } else {
            command.operands.clear();
            startingAddress = "0000";
        }
        while (startingAddress.length() < 4) {
            startingAddress = "0" + startingAddress;
        }
        std::transform(startingAddress.begin(), startingAddress.end(), startingAddress.begin(), ::toupper);
        nameOfProgram = command.label;
        command.address = startingAddress;

        if(command.label.size() != 0){
            labelInfo info;
            info.address = command.address;
            info.type = "Relative";
            std::pair<std::string,labelInfo> trying = std::make_pair(command.label,info);
            symbolTable.insert(trying);
        }
        locationCounter = hexaConverter.hexToDecimal(startingAddress);
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
            if(command.operands.size() != 0){
                if(symbolTable.find(command.operands.at(0)) == symbolTable.end()){
                    command.operands.clear();
                }
            }

            vector<ErrorMsg> literalErrorMessages = dumpLiterals(literalsBuffer);
            for (auto literalErrorMessage : literalErrorMessages) {
                newWrongCommands.push_back(literalErrorMessage);
            }

            endFound = true;
            if (++it != commands.end()) {
                ErrorMsg msg;
                msg.index = count;
                msg.msg = "Detected lines after END statement";
                newWrongCommands.push_back(msg);
            }
            break;
        }
        else if (command.mnemonic.compare("ORG") == 0) {
            try {
                locationCounter = hexaConverter.hexToDecimal(getOperandValue(command.operands.front()).address);
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
            vector<ErrorMsg> literalErrorMessages = dumpLiterals(literalsBuffer);
            for (auto literalErrorMessage : literalErrorMessages) {
                newWrongCommands.push_back(literalErrorMessage);
            }
        }

        if (command.operands.front()[0] == '=') {     //literal operand
            if (symbolTable.find(command.operands.front()) == symbolTable.end()) {
                literalsBuffer.push_back(make_pair(command.operands.front(), count));
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
            if (command.mnemonic.compare("EXTREF") == 0||command.mnemonic.compare("EXTDEF") == 0) {
                //TODO error
                ErrorMsg msg;
                msg.index = count;
                msg.msg = "The label is not allowed by this mnemonic ";
                newWrongCommands.push_back(msg);
                ++it;
                continue;
            }
            char firstChar = command.label[0];
            bool accepted = isalpha(firstChar);
            if (!accepted) {
                ErrorMsg msg;
                msg.index = count;
                msg.msg = "The label " + command.label + " is invalid";
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
                if(PassOneManager::symbolTable.find(command.label) != symbolTable.end()){
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


        if (command.mnemonic.compare("EXTDEF") == 0) {
            for (int i = 0; i < command.operands.size(); ++i) {
                SymbolPosition p;
                p.name = command.operands.at(i);
                p.index = count;
                externalDefinitions.push_back(p);
            }
        }
        if (command.mnemonic.compare("EXTREF") == 0) {
            for (int i = 0; i < command.operands.size(); ++i) {
                SymbolPosition p;
                p.name = command.operands.at(i);
                p.index = count;
                externalReferences.push_back(p);
            }
        }




        locationCounter += command.getNeededSpace();
        programLength += command.getNeededSpace();
        it++;
    }

    if (!endFound) {
        ErrorMsg msg;
        msg.index = count;
        msg.msg = "No END statement found";
        newWrongCommands.push_back(msg);
    }

    for (int j = 0; j < externalDefinitions.size(); ++j) {
        if (symbolTable.find(externalDefinitions.at(j).name) == symbolTable.end()) {
            ErrorMsg msg;
            msg.index = externalDefinitions.at(j).index;
            msg.msg = "didn't find the label \" " + externalDefinitions.at(j).name + " \" for external definition";
            newWrongCommands.push_back(msg);
        } else if (externalSymbolsInfo.find(externalDefinitions.at(j).name) != externalSymbolsInfo.end()) {
            ErrorMsg msg;
            msg.index = externalDefinitions.at(j).index;
            msg.msg = "repeated external label \" " + externalDefinitions.at(j).name + " \" for external definition";
            newWrongCommands.push_back(msg);
        } else {
            ExternalSymbolInfo info;
            info.index = externalDefinitions.at(j).index;
            info.address = symbolTable.find(externalDefinitions.at(j).name)->second.address;
            info.sectionName = nameOfProgram;
            externalSymbolsInfo.insert(make_pair(externalDefinitions.at(j).name, info));
        }
    }

    for (int j = 0; j < externalReferences.size(); ++j) {
        if (externalReferenceInfo.find(externalReferences.at(j).name) != externalReferenceInfo.end()) {
            ErrorMsg msg;
            msg.index = externalReferences.at(j).index;
            msg.msg = "repeated external Reference \" " + externalReferences.at(j).name + " \"";
            newWrongCommands.push_back(msg);
        } else {
            ExternalSymbolInfo info;
            info.index = externalReferences.at(j).index;
            externalReferenceInfo.insert(make_pair(externalReferences.at(j).name, info));
        }
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

    data.externalSymbols = externalSymbolsInfo;
    data.externalReference = externalReferenceInfo;
    data.errorMsgsMap = errorMsgsMap;
    data.symbolTable = symbolTable;
    data.literalTable = literalTable;
    data.programName = nameOfProgram;
    data.programLength = hexaConverter.decimalToHex(programLength);
    data.startingAddress = startingAddress;
    data.commands = finalCommands;
    return data;
}

vector<ErrorMsg> PassOneManager::dumpLiterals(vector<pair<string, int>> literalsBuffer) {
    vector<ErrorMsg> errorMessages;
    for(vector<pair<string, int>>::iterator it = literalsBuffer.begin(); it != literalsBuffer.end(); it++) {
        string literalName = it->first;
        int count = it->second;

        try {
            Literal literal = Literal(literalName, getCurrentLocation());
            literalTable.insert(make_pair(literalName, literal));
            locationCounter += literal.getSpace();
            programLength += literal.getSpace();
        } catch (exception e) {
            ErrorMsg msg;
            msg.index = count;
            msg.msg = "Bad literal";
            errorMessages.push_back(msg);
        }

    }
    literalsBuffer.clear();
}

string PassOneManager::getCurrentLocation() {
    string temp = hexaConverter.decimalToHex(locationCounter);
    while (temp.length() < 4){
        temp = "0" + temp;
    }
    return temp;
}

labelInfo PassOneManager::getOperandValue(string operand) {
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
        else if (PassOneManager::symbolTable.find(operand) != symbolTable.end()) {
            string tmpValue = symbolTable.find(operand)->second.address;
            info.address = tmpValue;
            info.type = symbolTable.find(operand)->second.type;
        }
        else throw invalid_argument("Invalid value");
    }
    return info;
}
