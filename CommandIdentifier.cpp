#include "CommandIdentifier.h"

CommandIdentifier::CommandIdentifier(){
    //initialize
}

bool CommandIdentifier::isInTable(string command) {
    return !(CommandIdentifier::operationsTable.find(command) == operationsTable.end());
}

OperationInfo CommandIdentifier::getInfo(string command) {
    return CommandIdentifier::operationsTable.at(command);
}