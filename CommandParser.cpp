#include "CommandParser.h"
#include "CommandIdentifier.h"
#include "ErrorMsg.h"
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
        if(c == 's')
        {
            commentCount++;
            continue;
        }
        else if(c == 'n') {
            if(lines[i] == "")
            {
                commentCount++;
                continue;
            }
            Command line = extractData(lines[i]);
            ErrorMsg errorMsg;
            errorMsg.setAttrib(i - commentCount, "Invalid line");
            wrongCommands.push_back(errorMsg);
            commands.push_back(line);
            continue;
        } else if (c == 'c')
        {
            commentCount++;
            continue;
        }

        Command line = extractData(lines[i]);
        string cond = validateLineSyntax(line);

        if(cond == " ")
            commands.push_back(line);
        else
        {
            ErrorMsg errorMsg;
            errorMsg.setAttrib(i - commentCount, cond);
            wrongCommands.push_back(errorMsg);
            commands.push_back(line);
        }

    }

    return commands;
}

char CommandParser::validateLineRegex(string line){

    regex c("\\s*\\.+[^\\n]*\\s*");
    regex r("[!@#$%^&*()|\\s;:\"']*[A-Za-z0-9]{0,10}[!@#$%^&*()|\\s;:\"']*\\+?[A-Za-z]{1,7}[!@#$%^&*()|\\s;:\"']*[\\d#@+\\w=',-]*\\s*[!@#$%^&*()|\\s;:\"'\\.A-Za-z0-9]*");
    regex s("\\s+");
    smatch m;

    regex_match(line,m,c);
    int commentSize = m.size();

    regex_match(line,m,s);
    if(m.size() != 0)
        return 's';

    regex_match(line,m,r);
    if(m.size() != 0)
        return ' ';
    else if(commentSize != 0)
        return 'c';
    return 'n';
}

string CommandParser::validateLineSyntax(Command line){
    CommandIdentifier commandIdentifier;
    if(line.mnemonic.length() == 0){
        return "No mnemonic";
    }
    string mnemonic = line.mnemonic;
    std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), ::toupper);

    if(mnemonic == "WORD")
    {
        if(line.operands.size() == 0)
            return "No operands exist";
        return validateWord(line);
    }
    else if(mnemonic == "END") {
        if (line.operands.size() != 0 && line.operands.size() != 1)
            return "Wrong operands number";
    } else {
        if(mnemonic.at(0) == '+')
        {
            mnemonic = mnemonic.substr(1,mnemonic.length()-1);
            if(commandIdentifier.getInfo(mnemonic).format != 3)
                return "Invalid format 4";

        }
        if(line.operands.size() != commandIdentifier.getInfo(mnemonic).numberOfOperands){
            return "Wrong operands number";
        }
        else if(mnemonic == "RESW" || mnemonic == "RESB")
            return validateRes(line);
        else if(mnemonic == "BYTE")
            return validateByte(line);
        else if(mnemonic == "START")
            return validateStart(line);

    }

    return " ";

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
                if(splitedCommand[1].find('\'') != std::string::npos){
                    int operandIndex = line.find('\'') - 1;
                    string commaOperand;
                    int flag = 0;
                    while (true){
                        char x = line[operandIndex];
                        commaOperand += x;
                        if(line[operandIndex] == '\'' ){
                            if(flag == 0)
                                flag++;
                            else
                                break;
                        }
                        operandIndex++;
                    }
                    commandData.operands.push_back(commaOperand);
                }
                else {
                    if(splitedCommand[1].length() != 0) {
                        commandData.operands.push_back(splitedCommand[1]);
                    }
                }
            }
            else{
                if(splitedCommand[1].length() != 0) {
                    std::istringstream ss(splitedCommand[1]);
                    std::string operand;
                    while (std::getline(ss, operand, ',')) {
                        commandData.operands.push_back(operand);
                    }
                }
            }
        }
        return commandData;
    }
    commandData.label = splitedCommand[0];
    canBeOperation = splitedCommand[1];
    if(canBeOperation[0] == '+') {
        canBeOperation = canBeOperation.substr(1,canBeOperation.length()-1);
    }
    isOperation = commandIdentifier.isInTable(canBeOperation);
    if (isOperation) {
        commandData.mnemonic = splitedCommand[1];
        if (commandIdentifier.getInfo(canBeOperation).hasOperand) {
            if(commandIdentifier.getInfo(canBeOperation).numberOfOperands == 1){
                if(splitedCommand[2].find('\'') != std::string::npos){
                    int operandIndex = line.find('\'') - 1;
                    if(line.find('=') != std::string::npos)
                        operandIndex = line.find('=');
                    string commaOperand;
                    int flag = 0;
                    while (true){
                        char x = line[operandIndex];
                        commaOperand += x;
                        if(line[operandIndex] == '\'' ){
                            if(flag == 0)
                                flag++;
                            else
                                break;
                        }
                        operandIndex++;
                    }
                    commandData.operands.push_back(commaOperand);
                }
                else {
                    if(splitedCommand[2].length() != 0) {
                        commandData.operands.push_back(splitedCommand[2]);
                    }
                }
            }
            else{
                if(splitedCommand[2].length() != 0) {
                    std::istringstream ss(splitedCommand[2]);
                    std::string operand;
                    while (std::getline(ss, operand, ',')) {
                        commandData.operands.push_back(operand);
                    }
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

string CommandParser::validateWord(Command command) {


    for(int i = 0 ; i < command.operands.size() ; i++)
    {
        string operand = command.operands[i];
        if(operand.length() >  4)
        {
            if(operand.at(0) != '-')
                return "Not compatible length";
            if(operand.length() > 5)
                return "Not compatible length";
        }

        int j = 0;
        if(operand.at(0) == '-')
            j=1;

        for(j ; j < operand.length() ; j++)
            if(!isdigit(operand.at(j)))
                return "Not a digit";
    }
    return " ";
}

string CommandParser::validateRes(Command command) {

    string operand = command.operands[0];

    if(operand.length() > 4)
        return "Not compatible length";

    for(int i = 0 ; i < operand.length() ; i++)
        if(!isdigit(operand.at(i)))
            return "Not a diigit";

    return " ";
}

string CommandParser::validateByte(Command command) {

    string operand = command.operands[0];

    if(operand.at(0) != 'X' && operand.at(0) != 'C')
        return "Type hex or char not specified";
    if(operand.at(1) != '\'' && operand.back() != '\'')
        return "Syntax error";

    char mode;
    if(operand.at(0) == 'X')
    {
        if(operand.length() > 17)
            return "Too long operand";
        mode = 'x';
    }
    else if(operand.at(0) == 'C')
    {
        if(operand.length() > 18)
            return "Too long operand";
        mode = 'c';
    }

    if(mode == 'c')
        return " ";

    vector<char> hexTab = {'A', 'B', 'C', 'D', 'E', 'F'};
    for(int i = 2 ; i < operand.length()-3 ; i++)
        if(!isdigit(operand.at(i)) && !(std::find(hexTab.begin(), hexTab.end(),operand.at(i)) != hexTab.end()))
            return "Not a hexadecimal digit";

    return " ";
}

vector<ErrorMsg> CommandParser::getWrongCommands() {
    return wrongCommands;
}

string CommandParser::validateStart(Command command) {

    string operand = command.operands[0];

    vector<char> hexTab = {'A', 'B', 'C', 'D', 'E', 'F'};
    for(int i = 0 ; i < operand.length() ; i++)
        if(!isdigit(operand.at(i)) && !(std::find(hexTab.begin(), hexTab.end(),operand.at(i)) != hexTab.end()))
            return "Not a hexadecimal digit";
    return " ";
}