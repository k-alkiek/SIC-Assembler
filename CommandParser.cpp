#include "CommandParser.h"
#include "CommandIdentifier.h"
#include <regex>

vector<Command> CommandParser::parseFile(vector<string> lines){

    CommandIdentifier commandIdentifier;
    vector<Command> commands = {};
    for(int i = 0 ; i < lines.size(); i++)
    {
        char c = validateLineRegex(lines[i]);
        if(c == 'c')
        {
            // comment
        }
        else if(c == 'n')
        {
            // error
        }

        vector<string> line = extractData(lines[1]);
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
    regex r("[!@#$%^&*()|\\s;:\"\']*[A-Za-z]{1,10}[!@#$%^&*()|\\s;:\"\'.]+[A-Za-z]{1,7}[!@#$%^&*()|\\s;:\"\'.]+[\\d#@+\\w,]+\\s*\\n");
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
    //kamel anta ba2a
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






























