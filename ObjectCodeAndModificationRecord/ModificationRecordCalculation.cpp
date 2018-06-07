//
// Created by saraheldafrawy on 07/06/18.
//

#include "ModificationRecordCalculation.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"
#include "../ConvertersAndEvaluators/ExpressionEvaluator.h"

string progName;
map<string, labelInfo> symbolTable;
HexaConverter hexaConverter;
vector<ModificationRecord> modificationRecords;

void ModificationRecordCalculation::setPrimaryDataNeeded (string name, map<string, labelInfo> symbolTab) {
    progName = name;
    symbolTable = symbolTab;
}

bool ModificationRecordCalculation::containsExternalReference (string expression, vector<string> extReferences) {
    for (int i = 0; i < extReferences.size(); i++) {
        if (expression.find(extReferences[i]) != string::npos) {
            return true;
        }
    }
    return false;
}

vector<vector<string>> ModificationRecordCalculation::setDefRecord(map<string, string> defRecordsUnsorted, vector<string> definitions) {
    vector<vector<string>> DefRecord;
    for (int i = 0; i < definitions.size(); i++) {
        if (defRecordsUnsorted.find(definitions[i]) != defRecordsUnsorted.end()) {
            vector<string> rec;
            rec.push_back(definitions[i]);
            rec.push_back(defRecordsUnsorted.at(definitions[i]));
            DefRecord.push_back(rec);
        } else {
            __throw_runtime_error("not all definitions are defined in the PROG");
        }
    }
}

vector<ModificationRecord> ModificationRecordCalculation::getModificationRecords() {
    return modificationRecords;
}

/**
 * 1) check for ExtRef and add them sorted to the ModRecVector with their corresponding sign
 *  -check it it's constant change halfBytes to ^06 else set it to ^05 (boolean variable)
 * ///////
 * 2) check relative wala absolute men 3adad ExtDef
 *  -if odd add PROG name else do nth
 *  -if so add prog Name in ModRec at the end of the record
 */

void ModificationRecordCalculation::evaluateModificationRecordExpression(bool constant,int itr, string expression,
                                                                         vector<string> extReferences, string addressInput,
                                                                         vector<string> definitions) {
    //TODO Gamal needs to skip extReferences in evaluation and set expression to absolute
    string address;
    string halfBytes;
    if (!constant) {
        address = hexaConverter.decimalToHex((hexaConverter.hexToDecimal(addressInput) + 1));
        halfBytes = "05";
    } else {
        address = addressInput;
        halfBytes = "06";
    }

    for (int i = 0; i < definitions.size(); i++) {
        string exp = expression;
        while (exp.find(definitions[i]) != string::npos) { // not sorted
            int position = exp.find(definitions[i]);
            ModificationRecord modRecord;
            modRecord.index = itr;
            modRecord.labelToBeAdded = extReferences[i];
            modRecord.address = address;
            modRecord.halfBytes = halfBytes;
            if (exp.at(position - 1) == '+') {
                modRecord.operation = "+";
            } else if (exp.at(position - 1) == '-') {
                modRecord.operation = "-";
            }
            modificationRecords.push_back(modRecord);
            exp = exp.substr(position + definitions[i].size(), exp.size());
        }
    }

    int counter = checkAddProgName(expression,definitions);
    while (counter != 0) {
        ModificationRecord modRecord;
        modRecord.index = itr;
        modRecord.labelToBeAdded = progName;
        if (counter > 0) {
            modRecord.operation = "+";
            counter--;
        } else {
            modRecord.operation = "-";
            counter ++;
        }
        modRecord.address = address;
        modRecord.halfBytes = halfBytes;
        modificationRecords.push_back(modRecord);
    }
}
//TODO kelma wa7da fei byte needs modification record
void ModificationRecordCalculation::addModificationRecord(Command cursor, int index, vector<string> definitions,
                                           vector<string> references) {
    ExpressionEvaluator expressionEvaluator(symbolTable, hexaConverter);
    if (cursor.mnemonic[0] == '+') {
        //dosent have ext ref
        if ((!(isExpression(cursor.operands[0])) && !containsExternalReference(cursor.operands[0], references))
            || ((isExpression(cursor.operands[0]))
                && !containsExternalReference(cursor.operands[0], references))) {
            ModificationRecord modRecord;
            modRecord.index = index;
            modRecord.labelToBeAdded = progName;
            modRecord.operation = "+";
            modRecord.address = hexaConverter.decimalToHex((hexaConverter.hexToDecimal(cursor.address) + 1));
            modRecord.halfBytes = "05";
            modificationRecords.push_back(modRecord);
        } else {
            //have ext ref
            evaluateModificationRecordExpression(false,index, cursor.operands[0], references, cursor.address, definitions);
        }
    } else if (cursor.mnemonic == "WORD") {
        /**
         * 1) check if it contains an expression
         * 2) check if it's a valid label
         * 3) if it's an expression get it's value and store it in obcode while skipping extRef
         * 4) add Modifications for extRef if it exists.
         */
        //there is a * or it has absolute expression that dosent contain ext ref
        //TODO check if it's a valid label
        string address = cursor.address;
        for(int i = 0; i < cursor.operands.size(); i++) {
            //TODO momken tedarab exception law kelma wa7da extRef
            if ((cursor.operands[i].size() == 1 && cursor.operands[i] == "*")
                || (isExpression(cursor.operands[i])
                    &&!containsExternalReference(cursor.operands[0], references)
                    && (symbolTable.at(cursor.label)).type == 0)) {
                ModificationRecord modRecord;
                modRecord.index = index;
                modRecord.labelToBeAdded = progName;
                modRecord.operation = "+";
                modRecord.address = hexaConverter.decimalToHex((hexaConverter.hexToDecimal(address) + 1));
                modRecord.halfBytes = "06";
                modificationRecords.push_back(modRecord);
            } else if (isExpression(cursor.operands[i]) && containsExternalReference(cursor.operands[0], references)) {
                // it has absolute expression that contains ext ref
                evaluateModificationRecordExpression(true, index, cursor.operands[0], references, cursor.address,
                                                     definitions);
            }
            address = hexaConverter.decimalToHex((hexaConverter.hexToDecimal(address) + 3));
        }

    } else if(isExpression(cursor.operands[0])){
        // format 3 with 3 half bytes
        if(containsExternalReference(cursor.operands[0], references)){
            evaluateModificationRecordExpression(false, index, cursor.operands[0], references, cursor.address,definitions);
        } else if(expressionEvaluator.evaluateExpression(cursor.operands[0], cursor.address).type == 0) {
            ModificationRecord modRecord;
            modRecord.index = index;
            modRecord.labelToBeAdded = progName;
            modRecord.operation = "+";
            modRecord.address = hexaConverter.decimalToHex((hexaConverter.hexToDecimal(cursor.address) + 1));
            modRecord.halfBytes = "03";
            modificationRecords.push_back(modRecord);
        }
        //TODO 3'alat
        //TODO check *= and * to modify
    } else if(cursor.operands[0] == "*"){
        //the star mod rec
        ModificationRecord modRecord;
        modRecord.index = index;
        modRecord.labelToBeAdded = progName;
        modRecord.operation = "+";
        modRecord.address = hexaConverter.decimalToHex((hexaConverter.hexToDecimal(cursor.address) + 1));
        modRecord.halfBytes = "003";
        modificationRecords.push_back(modRecord);
    }

}

int ModificationRecordCalculation::checkAddProgName(string expression,vector<string> definitions) {
    //checking if adding program name is needed (if the PROG's labels in expression are odd return true)
    //LISTA - ENDA is considered a pair, LISTA + ENDA isn't pair and add PROGA twice
    int counter = 0;
    for (int i = 0; i < definitions.size(); i++) {
        string exp = expression;
        while (exp.find(definitions[i]) != string::npos) {
            int position = exp.find(definitions[i]);
            if (exp.at(position-1) == '+') {
                counter ++;
            } else if (exp.at(position-1) == '-') {
                counter --;
            }
            exp = exp.substr(position+definitions[i].size(),exp.size());
        }
    }
    return counter;

}

bool ModificationRecordCalculation::isExpression(string operand) {
    if (operand.find('+') != std::string::npos || operand.find('-') != std::string::npos ||
        (operand.find('*') != std::string::npos && operand.length() != 1) || operand.find('/') != std::string::npos) {
        return true;
    }
    return false;
}
