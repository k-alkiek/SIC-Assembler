#include "PassTwoManager.h"
#include "../ConvertersAndEvaluators/ExpressionEvaluator.h"
#include "../ObjectCodeAndModificationRecord/ObjectCodeCalculation.h"
#include "../ObjectCodeAndModificationRecord/ModificationRecordCalculation.h"
#include <cmath>
#include <sstream>


string nextInstructionAddress;
vector<vector<string>> DefRecord;
vector<ModificationRecord> modificationRecords;
vector<string> definitions;
vector<string> references;
CommandIdentifier commandIdentifier;
map<string, string> defRecordUnsorted;
map<string, string> extDefinitions;
vector<string> litrals;
vector<string> textRecord;
HexaConverter hexaConverter;
bool baseAvailable = false;

vector<string> PassTwoManager::generateObjectCode(PrimaryData primaryData) {
    ModificationRecordCalculation modificationRecordCalculation;
    ObjectCodeCalculation objectCodeCalculator;

    modificationRecordCalculation.setPrimaryDataNeeded(primaryData.commands[0].label, primaryData.symbolTable);

    Command cursor;
    vector<Command> commands = primaryData.commands;
    cursor = commands[1];
    nextInstructionAddress = primaryData.startingAddress;
    int itr = 1;

    while (cursor.mnemonic != "END") {
        checkForErrors(cursor);
        update(cursor);
        if(noObjCode(cursor.mnemonic)){
            textRecord.push_back("");
            cursor = commands[++itr];
            continue;
        }
        if (cursor.operands.size() != 0) {
            modificationRecordCalculation.addModificationRecord(cursor, itr, definitions, references);
        }
        nextInstructionAddress = commands[itr + 1].address;
        textRecord.push_back(objectCodeCalculator.getObjectCode(cursor,nextInstructionAddress,commands[itr].address,primaryData.symbolTable,baseAvailable,references));
        cursor = commands[++itr];
    }
    calculateLitrals();
    DefRecord = modificationRecordCalculation.setDefRecord(defRecordUnsorted, definitions);
    modificationRecords = modificationRecordCalculation.getModificationRecords();
}

vector<vector<string>> PassTwoManager::getDefRecord() {
    return DefRecord;
}
vector<ModificationRecord> PassTwoManager::getModifiactionRecords() {
    return modificationRecords;
}
vector<string> PassTwoManager::getTextRecord(){
    return textRecord;
}

void PassTwoManager::checkForErrors(Command cursor){
    for (int i = 0; i < cursor.operands.size(); i++){
        if(commandIdentifier.isInTable(cursor.operands[i])){
            __throw_runtime_error("Invalid operand");
        }
    }
}

void PassTwoManager::update(Command cursor){
    if(cursor.mnemonic == "EXTDEF"){

        for(int i = 0; i < cursor.operands.size(); i++) {
            extDefinitions.insert(std::pair<string, string>(cursor.operands[i], cursor.operands[i])); //TODO shofiha sa7 wala 3'alat
            definitions.push_back(cursor.operands[i]);
        }
    }
    if(cursor.mnemonic == "EXTREF"){
        for(int i = 0; i < cursor.operands.size(); i++) {
            references.push_back(cursor.operands[i]);
        }
    }
    if(cursor.mnemonic == "BASE"){
        baseAvailable = true;
    }
    if(cursor.mnemonic == "NOBASE"){
        baseAvailable = false;
    }
    if(cursor.operands.size() != 0 && cursor.operands[0][0] == '='){
        litrals.push_back(cursor.operands[0]);
    }
    if(cursor.mnemonic == "LTORG"){
        calculateLitrals();
    }
    if (extDefinitions.find(cursor.label) != extDefinitions.end()) { // D^LISTA^000040
//        vector<string> dRec;
        defRecordUnsorted.insert(make_pair(cursor.label, cursor.address));
    }

}

void PassTwoManager::calculateLitrals(){
    for(int i = 0; i < litrals.size(); i++) {
        string operand;
        string litObCode;
        operand = litrals[i];
        string temp;
        if (operand.front() == 'X' && operand[1] == '\'') {
            if(hexaConverter.hexToDecimal(operand.substr(2, operand.length() - 2)) < 0){
                temp = "FFFFFF";
            } else{
                temp = "000000";
            }
            litObCode = temp.substr(operand.length() - 8, operand.length() - 2);
        } else if (operand.front() == 'C' && operand[1] == '\'') {
            litObCode = convertCToObjCode(operand.substr(2, operand.length() - 2));
        } else {
            __throw_runtime_error("Invalid type");
        }
        textRecord.push_back(litObCode);
    }
}

string PassTwoManager::convertCToObjCode(string str) {
    string asciiString = "";
    for (int i = 0; i < str.length(); i++) {
        asciiString += hexaConverter.decimalToHex(str[i]);
    }
    return asciiString;
}
bool PassTwoManager::noObjCode(string mnemonic){
    if(mnemonic == "RESB" || mnemonic == "RESW" || mnemonic == "LTORG" || mnemonic == "EXTREF" || mnemonic == "EXTDEF"){
        return true;
    }
    return false;
}