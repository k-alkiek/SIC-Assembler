//
// Created by saraheldafrawy on 07/06/18.
//

#include "ModificationRecordCalculation.h"
#include "../ConvertersAndEvaluators/ExpressionEvaluator.h"
#include "../Logger/Logger.h"
#include "../DTOs/Literal.h"

Logger loggerModRec;
string progName;
map<string, labelInfo> symTab;
HexaConverter hexConvertor;
vector<ModificationRecord> modificationRecord;

ExpressionEvaluator expressionEvaluator(symTab, hexConvertor);
void ModificationRecordCalculation::setPrimaryDataNeeded (string name, map<string, labelInfo> symbolTab) {
    progName = name;
    symTab = symbolTab;
    expressionEvaluator = ExpressionEvaluator(symTab, hexConvertor);
}

bool ModificationRecordCalculation::containsExternalReference (string expression, vector<string> extReferences) {
    for (int i = 0; i < extReferences.size(); i++) {
        if (expression.find(extReferences[i]) != string::npos) {
            if(expression.find(extReferences[i]) + extReferences[i].size() == expression.size()){
                return true;
            } else {
                char nextChar = expression.at(expression.find(extReferences[i]) + extReferences[i].size());
                if (nextChar == '-' || nextChar == '+' || nextChar == '*' || nextChar == '/') {
                    return true;
                }
            }
        }
    }
    return false;
}

//vector<vector<string>> ModificationRecordCalculation::setDefRecord(map<string, string> defRecordsUnsorted, vector<string> definitions) {
//    vector<vector<string>> DefRecord;
//    for (int i = 0; i < definitions.size(); i++) {
//        if (defRecordsUnsorted.find(definitions[i]) != defRecordsUnsorted.end()) {
//            vector<string> rec;
//            rec.push_back(definitions[i]);
//            rec.push_back(defRecordsUnsorted.at(definitions[i]));
//            DefRecord.push_back(rec);
//        } else {
//            __throw_runtime_error("not all definitions are defined in the PROG");
//        }
//    }
//}

vector<ModificationRecord> ModificationRecordCalculation::getModificationRecords() {
    return modificationRecord;
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
    expressionEvaluator.extref_tab = extReferences;
    string address;
    string halfBytes;
    if (!constant) {
        address = hexConvertor.decimalToHex((hexConvertor.hexToDecimal(addressInput) + 1));
        halfBytes = "05";
    } else {
        address = addressInput;
        halfBytes = "06";
    }

    for (int i = 0; i < extReferences.size(); i++) {
        string exp = expression;
        while (exp.find(extReferences[i]) != string::npos) { // not sorted
            int position = exp.find(extReferences[i]);
            ModificationRecord modRecord;
            modRecord.index = itr;
            modRecord.labelToBeAdded = extReferences[i];
            modRecord.address = address;
            modRecord.halfBytes = halfBytes;
            if (position == 0 || exp.at(position - 1) == '+') {
                modRecord.operation = "+";
            } else if (exp.at(position - 1) == '-') {
                modRecord.operation = "-";
            }
            modificationRecord.push_back(modRecord);
            exp = exp.substr(position + extReferences[i].size(), exp.size());
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
        modificationRecord.push_back(modRecord);
    }
}

void ModificationRecordCalculation::addModificationRecord(Command cursor, int index, vector<string> definitions,
                                           vector<string> references) {
    expressionEvaluator.extref_tab = references;
    checkForErrors(cursor,references);
    if (cursor.mnemonic[0] == '+') {
        //add * case here in format 4
        if(cursor.operands[0] == "*") {
            astrickModificationRecord(index,cursor,"05");
        }
        //dosent have ext ref
        else if ((!(isExpression(cursor.operands[0])) && !containsExternalReference(cursor.operands[0], references))
            || ((isExpression(cursor.operands[0]))
                && !containsExternalReference(cursor.operands[0], references))) {
            ModificationRecord modRecord;
            modRecord.index = index;
            modRecord.labelToBeAdded = progName;
            modRecord.operation = "+";
            modRecord.address = hexConvertor.decimalToHex((hexConvertor.hexToDecimal(cursor.address) + 1));
            modRecord.halfBytes = "05";
            modificationRecord.push_back(modRecord);
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
        string address = cursor.address;
        for(int i = 0; i < cursor.operands.size(); i++) {
            //there is a * or it has absolute expression that dosent contain ext ref
            if ((cursor.operands[i].size() == 1
                 && (cursor.operands[i] == "=*"|| cursor.operands[i] == "*"))
                || (isExpression(cursor.operands[i])
                    &&!containsExternalReference(cursor.operands[0], references)
                    && (symTab.at(cursor.label)).type != "Relative")) {
                ModificationRecord modRecord;
                modRecord.index = index;
                modRecord.labelToBeAdded = progName;
                modRecord.operation = "+";
                modRecord.address = address;
                modRecord.halfBytes = "06";
                modificationRecord.push_back(modRecord);
            } else if (isExpression(cursor.operands[i]) && containsExternalReference(cursor.operands[0], references)) {
                // it has absolute expression that contains ext ref
                evaluateModificationRecordExpression(true, index, cursor.operands[0], references, cursor.address,
                                                     definitions);
            }
            address = hexConvertor.decimalToHex((hexConvertor.hexToDecimal(address) + 3));
        }
    }
}

void ModificationRecordCalculation::astrickModificationRecord(int index, Command cursor, string halfBytes) {
    ModificationRecord modRecord;
    modRecord.index = index;
    modRecord.labelToBeAdded = progName;
    modRecord.operation = "+";
    modRecord.address = hexConvertor.decimalToHex((hexConvertor.hexToDecimal(cursor.address)));
    modRecord.halfBytes = halfBytes;
    modificationRecord.push_back(modRecord);
}

void ModificationRecordCalculation::astrickLiteralModificationRecord(vector<string> astrickAddresses, string halfBytes,
                                                                     map<string, Literal> literalTable) {
    for(int i = 0; i < astrickAddresses.size(); i++) {
        ModificationRecord modRecord;
        modRecord.labelToBeAdded = progName;
        modRecord.operation = "+";
        modRecord.address = literalTable.at(astrickAddresses[i]).getAddress();
        modRecord.halfBytes = halfBytes;
        modificationRecord.push_back(modRecord);
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
void ModificationRecordCalculation::clearModRecVector(){
    modificationRecord.clear();
}

void ModificationRecordCalculation::checkForErrors(Command cursor,vector<string> references){

    if(isExpression(cursor.operands[0]) && !(cursor.mnemonic == "WORD" || cursor.mnemonic == "EQU" || cursor.mnemonic == "ORG")) {
        if (expressionEvaluator.evaluateExpression(cursor.operands[0],cursor.address).type == 0
            && cursor.mnemonic[0] != '+') {
            loggerModRec.errorMsg("ModificationRecordCalculation: can't have absolute expression with format other than 4");
            __throw_runtime_error("can't have absolute expression with format other than 4");

        }
    }
    if(containsExternalReference(cursor.operands[0], references)
       && cursor.mnemonic[0] != '+'
       && cursor.mnemonic != "EXTREF"
                             && !(cursor.mnemonic == "WORD" || cursor.mnemonic == "EQU" || cursor.mnemonic == "ORG")){
        loggerModRec.errorMsg("ModificationRecordCalculation: can't have external reference with format other than 4");
        __throw_runtime_error("can't have extRef with format other than 4");
    }
}

bool ModificationRecordCalculation::isExpression(string operand) {
    if (operand.find('+') != std::string::npos || operand.find('-') != std::string::npos ||
        (operand.find('*') != std::string::npos && (operand.length() != 1 && operand.length() != 2)) || operand.find('/') != std::string::npos) {
        return true;
    }
    return false;
}
