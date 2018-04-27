#include "CommandParser.h"
#include "CommandIdentifier.h"
#include <regex>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <stdlib.h>

vector<Command> CommandParser::parseFile(vector<string> lines){

    CommandIdentifier commandIdentifier;
    vector<Command> commands = {};
    wrongCommands = {};
    int commentCount = 0;

    for(int i = 0 ; i < lines.size(); i++)
    {
        char c = validateLineRegex(lines[i]);
        if(c == 'n') {
            wrongCommands.push_back(i - commentCount);
            continue;
        } else if (c == 'c')
        {
            commentCount++;
            continue;
        }

        Command line = extractData(lines[i]);
        bool cond = validateLineSyntax(line);

        if(cond)
            commands.push_back(line);

        else
            wrongCommands.push_back(i);

    }

    return commands;
}

char CommandParser::validateLineRegex(string line){

    regex c("\\s*\\.+[^\\n]*\\s*");
    regex r("[!@#$%^&*()|\\s;:\"']*[A-Za-z0-9]{0,10}[!@#$%^&*()|\\s;:\"']*\\+?[A-Za-z]{1,7}[!@#$%^&*()|\\s;:\"']*[\\d#@+\\w=',]*\\s*[!@#$%^&*()|\\s;:\"'\\.A-Za-z0-9]*");
    smatch m;

    regex_match(line,m,c);
    int commentSize = m.size();

    regex_match(line,m,r);
    if(m.size() != 0)
        return ' ';
    else if(commentSize != 0)
        return 'c';
    return 'n';
}

bool CommandParser::validateLineSyntax(Command line){
    CommandIdentifier commandIdentifier;
    if(line.mnemonic.length() == 0){
        return false;
    }
    string mnemonic = line.mnemonic;
    std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), ::toupper);

    if(mnemonic == "WORD")
    {
        if(line.operands.size() == 0)
            return false;
        return validateWord(line);
    }
    else if(mnemonic == "END")
        if(line.operands.size() != 0 && line.operands.size() != 1)
            return false;
    else
    {
        if(line.operands.size() != commandIdentifier.getInfo(mnemonic).numberOfOperands){
            return false;
        }
        else if(mnemonic == "RESW" || mnemonic == "RESB")
            return validateRes(line);
        else if(mnemonic == "BYTE")
            return validateByte(line);
        else if(mnemonic == "START")
            return validateStart(line);

    }
    return true;

}

Command CommandParser::extractData(string line) {
    CommandIdentifier commandIdentifier;
    Command commandData; // the vector that will e returned
    std::vector<std::string> splitedCommand; // vector helps making processing
    istringstream splitedData(line);
    do {
        string data;
        splitedData >> data;
        splitedCommand.push_back(data);
    } while (splitedData);
    std::string canBeOperation = splitedCommand[0];
    if(canBeOperation[0] == '+') {
        canBeOperation = canBeOperation.substr(1,canBeOperation.length()-1);
    }
    bool isOperation = commandIdentifier.isInTable(canBeOperation);
    if (isOperation) {
        commandData.label = "";
        commandData.mnemonic = splitedCommand[0];
        if (commandIdentifier.getInfo(canBeOperation).hasOperand) {
            if(commandIdentifier.getInfo(canBeOperation).numberOfOperands == 1){
                commandData.operands.push_back(splitedCommand[1]);
            }
            else{
                std::istringstream ss(splitedCommand[1]);
                std::string operand;
                while(std::getline(ss, operand, ',')) {
                    commandData.operands.push_back(operand);
                }
            }
        }
        return commandData;
    }
    commandData.label = splitedCommand[0];
    canBeOperation = splitedCommand[1];
    if(canBeOperation[1] == '+') {
        canBeOperation = canBeOperation.substr(1,canBeOperation.length()-1);
    }
    isOperation = commandIdentifier.isInTable(splitedCommand[1]);
    if (isOperation) {
        commandData.mnemonic = splitedCommand[1];
        if (commandIdentifier.getInfo(canBeOperation).hasOperand) {
            if(commandIdentifier.getInfo(canBeOperation).numberOfOperands == 1){
                commandData.operands.push_back(splitedCommand[2]);
            }
            else{
                std::istringstream ss(splitedCommand[2]);
                std::string operand;
                while(std::getline(ss, operand, ',')) {
                    commandData.operands.push_back(operand);
                }
            }
        }
        return commandData;
    }
    else{
        commandData.mnemonic = "";
        return commandData;
    }
}

bool CommandParser::validateWord(Command command) {


    for(int i = 0 ; i < command.operands.size() ; i++)
    {
        string operand = command.operands[i];
        if(operand.length() >  4)
        {
            if(operand.at(0) != '-')
                return false;
        }

        int j = 0;
        if(operand.at(0) != '-')
            j=1;

        for(j ; j < operand.length() ; j++)
            if(!isdigit(operand.at(j)))
                return false;
    }
    return true;
}

bool CommandParser::validateRes(Command command) {

    string operand = command.operands[0];

    if(operand.length() > 4)
        return false;

    for(int i = 0 ; i < operand.length() ; i++)
        if(!isdigit(operand.at(i)))
            return false;

    return true;
}

bool CommandParser::validateByte(Command command) {

    string operand = command.operands[0];

    if(operand.at(0) != 'X' && operand.at(0) != 'C')
        return false;
    if(operand.at(1) != '\'' && operand.back() != '\'')
        return false;

    char mode;
    if(operand.at(0) == 'X')
    {
        if(operand.length() > 17)
            return false;
        mode = 'x';
    }
    else if(operand.at(0) == 'C')
    {
        if(operand.length() > 18)
            return false;
        mode = 'c';
    }

    if(mode == 'c')
        return true;

    vector<char> hexTab = {'A', 'B', 'C', 'D', 'E', 'F'};
    for(int i = 2 ; i < operand.length()-3 ; i++)
        if(!isdigit(operand.at(i)) && !(std::find(hexTab.begin(), hexTab.end(),operand.at(i)) != hexTab.end()))
            return false;

    return true;
}

vector<int> CommandParser::getWrongCommands() {
    return wrongCommands;
}

bool CommandParser::validateStart(Command command) {

    string operand = command.operands[0];

    vector<char> hexTab = {'A', 'B', 'C', 'D', 'E', 'F'};
    for(int i = 0 ; i < operand.length() ; i++)
        if(!isdigit(operand.at(i)) && !(std::find(hexTab.begin(), hexTab.end(),operand.at(i)) != hexTab.end()))
        {
            char l = operand.at(i);
            return false;
        }

    return true;
}