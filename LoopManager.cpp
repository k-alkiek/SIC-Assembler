#include "LoopManager.h"

PrimaryData LoopManager::loop(string file) {
    string startingAddress;
    int programLength;
    vector<Command> commands = parser.parseFile(LoopManager::reader.readFile(file));
    vector<Command>::iterator it;
    Command command;
    it = commands.begin();
    command = *it;
    if(command.mnemonic != "START") {
        //error
    }
    startingAddress = command.operands.at(0);
    locationCounter = stoi(startingAddress);
    ++it;
    while (it != commands.end()) {
        command = *it;
        if(command.mnemonic == "END"){
            break;
        }
        if(command.label != ""){
            if(LoopManager::symbolTable.find(command.label) != symbolTable.end()){
                //TODO error
            }
            string temp = to_string(locationCounter);
            while (temp.length() < 4){
                temp = "0" + temp;
            }
            symbolTable.insert(make_pair(command.label,temp));
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