#include "CommandParser.h"
#include "CommandIdentifier.h"
#include <regex>
#include <algorithm>
#include <string>

vector<Command> CommandParser::parseFile(vector<string> lines){

    CommandIdentifier commandIdentifier;
    vector<Command> commands = {};
    for(int i = 0 ; i < lines.size(); i++)
    {
        char c = validateLineRegex(lines[i]);
        if(c == 'n')
        {

        }

        Command line = extractData(lines[i]);
        bool cond = validateLineSyntax(line);

        if(cond)
        {
            /*
            * determine command instance
            * assemble data to vector command
            */
        }
        else
        {
            // error
        }

    }

    return commands;
}

char CommandParser::validateLineRegex(string line){

    regex c("\\s*\\.{1}[^\\n]*\\s*\\n");
    regex r("[!@#$%^&*()|\\s;:\"\']*[A-Za-z]{1,10}[!@#$%^&*()|\\s;:\"\'.]+\\+?[A-Za-z]{1,7}[!@#$%^&*()|\\s;:\"\'.]+[\\d#@+\\w,]+\\s*[!@#$%^&*()|\\s;:\"\'\\.A-Za-z0-9]*");
    smatch m;

    regex_match(line,m,c);
    if(m.size() != 0)
        return 'c';
    regex_match(line,m,r);
    if(m.size() != 0)
        return ' ';
    return 'n';
}

bool CommandParser::validateLineSyntax(Command line){
    CommandIdentifier commandIdentifier;
    if(line.mnemonic.length() == 0){
        return false;
    }
    if(line.operands.size() != commandIdentifier.getInfo(line.mnemonic).numberOfOperands){
        return false;
    }

    string mnemonic = line.mnemonic;
    std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), ::toupper);
    if(mnemonic == "WORD")
        return validateWord(line);
    else if(mnemonic == "RESW" || mnemonic == "RESB" || mnemonic == "START")
        return validateRes(line);
    else if(mnemonic == "BYTE")
        return validateByte(line);

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
    bool isOperation = commandIdentifier.isInTable(splitedCommand[0]);
    if (isOperation) {
        commandData.label = "";
        commandData.mnemonic = splitedCommand[0];
        if (commandIdentifier.getInfo(splitedCommand[0]).hasOperand) {
            if(commandIdentifier.getInfo(splitedCommand[0]).numberOfOperands == 1){
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
    isOperation = commandIdentifier.isInTable(splitedCommand[1]);
    if (isOperation) {
        commandData.mnemonic = splitedCommand[1];
        if (commandIdentifier.getInfo(splitedCommand[1]).hasOperand) {
            if(commandIdentifier.getInfo(splitedCommand[1]).numberOfOperands == 1){
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

    string operand = command.operands[0];

    if(operand.length() >  4)
    {
        if(operand.at(0) != '-')
            return false;
    }

    int i = 0;
    if(operand.at(0) != '-')
        i=1;

    for(i ; i < operand.length() ; i++)
        if(!isdigit(operand.at(i)))
            return false;

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

    if(operand.at(0) != 'X' || operand.at(0) != 'C')
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
        if(!isdigit(operand.at(i)) || !(std::find(hexTab.begin(), hexTab.end(),operand.at(i)) != hexTab.end()))
            return false;

    return true;
}



