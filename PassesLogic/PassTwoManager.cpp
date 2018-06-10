#include "PassTwoManager.h"
#include "../ConvertersAndEvaluators/ExpressionEvaluator.h"
#include "../ObjectCodeAndModificationRecord/ObjectCodeCalculation.h"
#include "../ObjectCodeAndModificationRecord/ModificationRecordCalculation.h"
#include "../DTOs/PassTwoData.h"
#include "../Logger/Logger.h"
#include <cmath>
#include <sstream>

//TODO test cases
/**
 * check general expression evaluation
 * Add EQU and ORG and test modRec and Object Code (we don't add modRec or ObjectCode)
 * check Astrick and different Modification records
 * test literals
 * test lecture examples
 */
CommandIdentifier commandIdentifier;
HexaConverter hexaConverter;
Logger loggerPassTwo;
PassTwoData PassTwoManager::generateObjectCode(PrimaryData primaryData) {
    ModificationRecordCalculation modificationRecordCalculation;
    ObjectCodeCalculation objectCodeCalculator;
    PassTwoData data;
    modificationRecordCalculation.setPrimaryDataNeeded(primaryData.commands[0].label, primaryData.symbolTable);

    Command cursor;
    vector<Command> commands = primaryData.commands;
    cursor = commands[0];
    data.nextInstructionAddress = primaryData.startingAddress;
    int itr = 0;
    bool isBaseHasValue = false;
    int baseValue;
    string ldbOperand;
    while (cursor.mnemonic != "END") {
        try {
            checkForErrors(cursor,data);
            if(cursor.mnemonic == "EXTREF"){
                data.references = updateDataVectors(cursor,data.references);
            } else if(cursor.mnemonic == "EXTDEF"){
                data.definitions = updateDataVectors(cursor,data.definitions);
            } else if(cursor.mnemonic == "BASE"){
                if(checkBase(cursor.operands[0],ldbOperand)) {
                    data.baseAvailable = true;
                    objectCodeCalculator.setBaseCounter(baseValue);
                } else{
                    loggerPassTwo.errorMsg("PassTwoManager: Invalid operand with BASE instruction");
                    __throw_runtime_error("Invalid operand with BASE instruction");
                }

            } else if(cursor.mnemonic == "NOBASE"){
                data.baseAvailable = false;
            } else if(cursor.mnemonic == "LTORG"){
                for(int i = 0; i < data.litrals.size(); i++) {
                    data.textRecord.push_back(primaryData.literalTable.at(data.litrals[i]).getValue());
                }
                data.litrals.clear();
            } else if(cursor.mnemonic == "LDB"){
                baseValue = getBaseValue(cursor,primaryData.symbolTable);
                ldbOperand = cursor.operands[0];
                isBaseHasValue = true;
            }
            if(cursor.operands.size() != 0 && cursor.operands[0][0] == '='){
                data.litrals.push_back(cursor.operands[0]);
            }
            if (noObjCode(cursor.mnemonic)) {
                if(cursor.mnemonic == "EQU" || cursor.mnemonic == "ORG"){
                    if(cursor.operands[0] == "=*"){
                        modificationRecordCalculation.addModificationRecord(cursor, itr, data.definitions, data.references);
                    }
                }
                data.textRecord.push_back("");
                cursor = commands[++itr];
                continue;
            }
            if (cursor.operands.size() != 0) {
                modificationRecordCalculation.addModificationRecord(cursor, itr, data.definitions, data.references);
            }
            data.nextInstructionAddress = commands[itr + 1].address;
            data.textRecord.push_back(
                    objectCodeCalculator.getObjectCode(cursor, data.nextInstructionAddress, commands[itr].address,
                                                       primaryData.symbolTable, primaryData.literalTable, data.baseAvailable,
                                                       data.references));
            cursor = commands[++itr];
        }catch (const runtime_error& error) {
            loggerPassTwo.errorMsg("PassTwoManager: Caught exception at line " + to_string(itr+1));
            std::cout << "Caught exception \"" << error.what() << " at line " << itr +1 << "\"\n";
            exit(0);
        }
    }
    for(int i = 0; i < data.litrals.size(); i++) {
        data.textRecord.push_back(primaryData.literalTable.at(data.litrals[i]).getValue());
    }
    data.litrals.clear();
//    DefRecord = modificationRecordCalculation.setDefRecord(defRecordUnsorted, definitions);
    data.modificationRecords = modificationRecordCalculation.getModificationRecords();
    modificationRecordCalculation.clearModRecVector();
    return data;
}

//vector<vector<string>> PassTwoManager::getDefRecord() {
//    return DefRecord;
//}

void PassTwoManager::checkForErrors(Command cursor,PassTwoData data){
    for (int i = 0; i < cursor.operands.size(); i++){
        if(commandIdentifier.isInTable(cursor.operands[i])){
            loggerPassTwo.errorMsg("PassTwoManager: Invalid operand ");
            __throw_runtime_error("Invalid operand");
        }
    }
    if(cursor.mnemonic.substr(0,1) == "st" && cursor.mnemonic.length() == 3 && cursor.operands[0][0] == '='){
        loggerPassTwo.errorMsg("PassTwoManager: Cannt store in literal ");
        __throw_runtime_error("Invalid Cannt store in literal");
    }
}

vector<string> PassTwoManager::updateDataVectors(Command cursor,vector<string> data){
    vector<string> temp = data;
    for(int i = 0; i < cursor.operands.size(); i++) {
        temp.push_back(cursor.operands[i]);
    }
    return temp;
}


//string PassTwoManager::convertCToObjCode(string str,PassTwoData data) {
//    string asciiString = "";
//    for (int i = 0; i < str.length(); i++) {
//        asciiString += data.hexaConverter.decimalToHex(str[i]);
//    }
//    return asciiString;
//}

bool PassTwoManager::noObjCode(string mnemonic){
    if(mnemonic == "RESB" || mnemonic == "RESW" || mnemonic == "LTORG" || mnemonic == "EXTREF" || mnemonic == "EXTDEF" ||
       mnemonic == "BASE" || mnemonic == "NOBASE" || mnemonic == "EQU" || mnemonic == "ORG" || mnemonic == "CSET" || mnemonic == "START"){
        return true;
    }
    return false;
}

int PassTwoManager::getBaseValue(Command cursur, map<string, labelInfo> symbolTable){
    if(cursur.operands[0] == "*"){
        return hexaConverter.hexToDecimal(cursur.address);
    } else if(cursur.operands[0][0] == '#'){
        if(symbolTable.find(cursur.operands[0].substr(1,cursur.operands[0].size()-1)) != symbolTable.end()){
            return hexaConverter.hexToDecimal((symbolTable.at(cursur.operands[0].substr(1,cursur.operands[0].size()-1)).address));
        } else if(is_number(cursur.operands[0].substr(1,cursur.operands[0].size()-1))){
            return stoi(cursur.operands[0].substr(1,cursur.operands[0].size()-1));
        } else{
            loggerPassTwo.errorMsg("PassTwoManager: invalid value in base register ");
            __throw_runtime_error("invalid value in base register ");
        }
    } else if(cursur.operands[0][0] == '='){
        return stoi(cursur.operands[0].substr(3,cursur.operands[0].size()-2));
    } else{
        loggerPassTwo.errorMsg("PassTwoManager: invalid value in base register ");
        __throw_runtime_error("invalid value in base register ");
    }
}

bool PassTwoManager::is_number(string s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool PassTwoManager::checkBase(string baseOperand, string ldbOperand){
    if(ldbOperand.find(baseOperand) != string::npos){
        return true;
    }
    return false;
}