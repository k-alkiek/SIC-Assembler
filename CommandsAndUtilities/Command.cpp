//
// Created by abdelrahman on 16/04/18.
//

#include "Command.h"
#include "CommandIdentifier.h"

int Command::getNeededSpace() {
    CommandIdentifier opTable;
    if (mnemonic.find('+') != std::string::npos) {
        return 4;
    } else if(mnemonic.compare("WORD") == 0) {
        return operands.size() * 3;
    } else if(mnemonic.compare("BYTE") == 0) {
        string str = operands.front();
        if (str.front() == 'X') {
            return (str.size() - 3) / 2;
        } else {
                return str.size() - 3;
        }
    } else if(mnemonic.compare("RESW") == 0) {
        return stoi(operands.front())*3;
    } else if(mnemonic.compare("RESB") == 0) {
        return stoi(operands.front());
    } else if(mnemonic.compare("ORG") == 0) {
        return 0;
    } else if(mnemonic.compare("EQU") == 0) {
        return 0;
    } else if(mnemonic.compare("START") == 0) {
        return 0;
    } else if(mnemonic.compare("END") == 0) {
        return 0;
    } else if(mnemonic.compare("BASE") == 0) {
        return 0;
    } else if(mnemonic.compare("NOBASE") == 0) {
        return 0;
    } else if (opTable.isInTable(mnemonic)) {
        return opTable.getInfo(mnemonic).format;
    }
    return 0;
}
