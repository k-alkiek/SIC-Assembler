#include "PassTwoManager.h"
#include "../CommandsAndUtilities/CommandIdentifier.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"
#include "../DTOs/labelInfo.h"

string locationCounter;
map<string,labelInfo> symbolTable;
vector<string> PassTwoManager::generateObjectCode(PrimaryData primaryData) {

    vector<Command> commands = primaryData.commands;
    Command cursor ;//= commands[0];
    vector<string> objectCode;
     cursor = commands[1];
     int itr = 1;
     while (cursor.mnemonic != "END") {
         objectCode.push_back(getObjectCode(cursor));
         cursor = commands[++itr];
         locationCounter = cursor.address;
     }

}

//TODO ta7abeesh special cases
//TODO check for errors
//TODO ne7sseb amaken Modification records
//TODO format 2 need to be modified to handel one register

string PassTwoManager::getObjectCode(Command cursor) {
    CommandIdentifier opTable;
    HexaConverter hexaConverter;
    if (cursor.mnemonic == "WORD") {
        // WORD 30
        //
        //convert constant to object code
    } else if(cursor.mnemonic == "BYTE") {

    } else if (opTable.isInTable(cursor.mnemonic)) {

        OperationInfo operationInfo = opTable.getInfo(cursor.mnemonic);
        int commandObjCode = hexaConverter.hexToDecimal(operationInfo.code);
        int format = operationInfo.format;

        if(format == 1){
            return hexaConverter.decimalToHex(commandObjCode);
        } else if(format == 2){
            return completeObjCodeFormat2(commandObjCode,cursor.operands);
        } else if(format == 3 && cursor.operands[0].front() != '+'){
            return completeObjCodeFormat3(commandObjCode,cursor.operands);
        } else{
            return completeObjCodeFormat4(commandObjCode,cursor.operands);
        }
//             if(isSymbol(cursor.operands)) {
//                 /**
//                  * if there is a symbol in OPERAND field then
//     *               begin
//     *                 search SYMTAB for OPERAND
//     *                 if found then
//     *                    store symbol value as operand address
//     *                 else
//     *                    begin
//     *                      store 0 as operand address
//     *                      set error flag (undefined symbol)
//     *                    end
//     *               end {if symboll}
//     *              else
//                  */
//             } else {
//                 //store 0 as operand address
//             }
        //assemble the object code instruction
    }
}
string PassTwoManager::completeObjCodeFormat2(int uncompletedObjCode, vector<string> operands){
    HexaConverter hexaConverter;
    int registerCode = getRegisterNumber(operands[0]);
    if(operands.size() != 1) {
        registerCode = registerCode << 4;
        registerCode = registerCode | getRegisterNumber(operands[1]);
        uncompletedObjCode = uncompletedObjCode << 8;
    } else{
        uncompletedObjCode = uncompletedObjCode << 4;
    }
    uncompletedObjCode = uncompletedObjCode | registerCode;
    return hexaConverter.decimalToHex(uncompletedObjCode);
}

string PassTwoManager::completeObjCodeFormat3(int uncompletedObjCode,vector<string> operands){
    HexaConverter hexaConverter;
    labelInfo label = symbolTable.at(operands[0]);
    vector<int> results = getDisplacement(label.address, locationCounter);
    int isPC = results[0];
    int displacement = results[1];
    //(displacement < getdispRange()) boolean expression to indicate Base or PC relative ///TODO not correct //not needed anymore
    vector<int> nixbpe = getFlagsCombination(operands,3,isPC);// give me ni separated from xbpe
    int completedObjCode = ((uncompletedObjCode | nixbpe[0]) << 4) | nixbpe[1];
    completedObjCode = (completedObjCode << 12) | ((displacement << 20) >> 20);
    return hexaConverter.decimalToHex(completedObjCode);
}

string PassTwoManager::completeObjCodeFormat4(int uncompletedObjCode,vector<string> operands){
    HexaConverter hexaConverter;
    vector<int> nixbpe = getFlagsCombination(operands,4, false);// give me ni separated from xbpe
    string address = symbolTable.at(operands[0]).address;
    int completedObjCode = ((uncompletedObjCode | nixbpe[0]) << 4) | nixbpe[1];
    completedObjCode = (completedObjCode << 20) | ((completedObjCode << 12) >> 12);
    return hexaConverter.decimalToHex(completedObjCode);
}

vector<int> PassTwoManager::getFlagsCombination(vector<string> operands, int format, int PCRelative){
    int ni = 0;
    int xbpe = 0;
    if (operands.size() > 2) {
        //ERROR too many operands //mesh 3arfa handelled wala la2?!!
    }
    if (operands.size() == 2) {
        if (operands[1] == "X") {
            xbpe = 1;       //indexing
        } else {
            //ERROR wrong second operand
        }
    }
    if(format == 3){
        if (operands[0].front() == '#') {
            if (xbpe = 1) {
                //ERROR can't have immediate with X
            }
            ni = 1;             //01 immediate
        } else if (operands[0].front() == '@') {
            if (xbpe = 1) {
                //ERROR can't have indirect with X
            }
            ni = 1;
            ni = (ni << 1);     //10 indirect
        } else {
            ni = 1;
            ni = (ni << 1) | 1; //11 simple addressing
        }
        if (PCRelative == 1) {
            xbpe = ((xbpe << 2) | 1 ) << 1;
        } else {
            xbpe = ((xbpe << 1) | 1 ) << 2;
        }

    } else { //format 4
        ni = 1;
        ni = (ni << 1) | 1;
        xbpe = (xbpe << 3) | 1;
    }
    vector<int> returnedValue;
    returnedValue.push_back(ni);
    returnedValue.push_back(xbpe);
}

int PassTwoManager::getRegisterNumber(string registerr){
    if(registerr.compare("A") == 0){
        return 0;
    } else if(registerr.compare("X") == 0){
        return 1;
    }else if(registerr.compare("L") == 0){
        return 2;
    }else if(registerr.compare("B") == 0){
        return 3;
    }else if(registerr.compare("S") == 0){
        return 4;
    }else if(registerr.compare("T") == 0){
        return 5;
    }else if(registerr.compare("F") == 0){
        return 6;
    }else if(registerr.compare("PC") == 0){
        return 8;
    }else if(registerr.compare("SW") == 0){
        return 9;
    } else{
        return 10;
    }
}

vector<int> PassTwoManager::getDisplacement(string TA, string progCounter) {
    HexaConverter hexaConverter;
    vector<int> results;
    int displacement = 0;
    int isPC = 1;
    int targetAdd = hexaConverter.hexToDecimal(TA);
    int programCount = hexaConverter.hexToDecimal(progCounter);
    displacement = targetAdd - programCount;
    if(displacement > 4096){
        isPC = 0;
    } else if(displacement < - 2048){
        //error
    }
    results.push_back(isPC);
    results.push_back(displacement);
    return results;
}

/*int PassTwoManager::getdispRange() { //TODO not correct
    int x = 1;
    x = (x << 3 | x << 2 | x << 1 | 1);
    return x;
}*/
