#include "PassTwoManager.h"
#include "../CommandsAndUtilities/CommandIdentifier.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"
#include "../ConvertersAndEvaluators/ExpressionEvaluator.h"
#include "../ObjectCodeAndModificationRecord/ObjectCodeCalculation.h"
#include <cmath>


string locationCounter;
map<string, labelInfo> symbolTable;
HexaConverter hexaConverter;
vector<vector<string>> DefRecord;
vector<ModificationRecord> modificationRecords;
string progName;

//TODO handle base counter
//TODO if operands contains *
//TODO EQU --> must check modification records too
//TODO LTORG and litteral pools
//TODO ORG, BASE, NOBASE, RSUB
//TODO handel L erros

vector<string> PassTwoManager::generateObjectCode(PrimaryData primaryData) {
    ObjectCodeCalculation objectCodeCalculator;
    map<string, string> extDefinitions;
    vector<string> definitions; // TODO modify it to be vector from primary data
    vector<string> references; // TODO modify it to be vector from primary data
    //extDefinitions = PrimaryData.getExtDef()
    //extReferences = PrimaryData.getExtRef()
    progName = primaryData.commands[0].label;  // PROG name set at index 0
    vector<string> textRecord;
    map<string, string> defRecordUnsorted;

    Command cursor;
    vector<Command> commands = primaryData.commands;
    cursor = commands[1];
    locationCounter = primaryData.startingAddress;
    int itr = 1;
    symbolTable = primaryData.symbolTable;
    while (cursor.mnemonic != "END") {
        if (extDefinitions.find(cursor.label) != extDefinitions.end()) { // D^LISTA^000040
            vector<string> dRec;
            defRecordUnsorted.insert(cursor.label, cursor.address);
        }
        addModificationRecord(cursor, itr, definitions, modificationRecords, references);

        locationCounter = commands[itr + 1].address;
        textRecord.push_back(objectCodeCalculator.getObjectCode(cursor,locationCounter,symbolTable));
        cursor = commands[++itr];
    }
    setDefRecord(defRecordUnsorted, definitions);
}

/////////////////////Modification Records////////////////////////////////////

bool PassTwoManager::containsExternalReference (string expression, vector<string> extReferences) {
    for (int i = 0; i < extReferences.size(); i++) {
        if (expression.find(extReferences[i]) != string::npos) {
            return true;
        }
    }
    return false;
}

void PassTwoManager::setDefRecord(map<string, string> defRecordsUnsorted, vector<string> definitions) {
    for (int i = 0; i < definitions.size(); i++) {
        if (defRecordsUnsorted.find(definitions[i]) != defRecordsUnsorted.end()) {
            vector<string> rec;
            rec.push_back(definitions[i]);
            rec.push_back(defRecordsUnsorted.at(definitions[i]));
            DefRecord.push_back(rec);
        } else {
            //TODO ERROR not all definitions are defined in the PROG
        }
    }
}

/**
 * 1) check for ExtRef and add them sorted to the ModRecVector with their corresponding sign
 *  -check it it's contant change halfBytes to ^06 else set it to ^05 (boolean variable)
 * ///////
 * 2) check relative wala absolute men 3adad ExtDef
 *  -if odd add PROG name else do nth
 *  -if so add prog Name in ModRec at the end of the record
 */

void PassTwoManager::evaluateModificationRecordExpression(bool constant,int itr, string expression, vector<string> extReferences, string addressInput) {
    string address;
    string halfBytes;
    if (!constant) {
        address = hexaConverter.decimalToHex((hexaConverter.hexToDecimal(address) + 1));
        halfBytes = "05";
    } else {
        address = addressInput;
        halfBytes = "06";
    }
    for (int i = 0; i < extReferences.size(); i++) { //get them then sort them //TODO momken 2a7ot nafs el reference mareteen? yes:(
        if (expression.find(extReferences[i]) != string::npos) {
            int position = expression.find(extReferences[i]);
            ModificationRecord modRecord;
            modRecord.index = itr;
            modRecord.labelToBeAdded = expression[i];
            modRecord.address = address;
            modRecord.halfBytes = halfBytes;
            if (expression.at(position - 1) == '+') {
                modRecord.operation = "+";
            } else if (expression.at(position - 1) == '-') {
                modRecord.operation = "-";
            }
            modificationRecords.push_back(modRecord);
        }
    }
    if (checkAddProgName(expression)) { //TODO check if we don't add it to contants
        ModificationRecord modRecord;
        modRecord.index = itr;
        modRecord.labelToBeAdded = progName;
        modRecord.operation = "+";
        modRecord.address = address;
        modRecord.halfBytes = halfBytes;
        modificationRecords.push_back(modRecord);
    }
}

void PassTwoManager::addModificationRecord(Command cursor, int itr, vector<string> definitions,
                                      vector<ModificationRecord> modificationRecords, vector<string> references) {
    if (cursor.mnemonic[0] == '+' &&
        !containsExternalReference(cursor.operands[0], definitions)) { // TODO check (handeled as all operands sizes are 1 or 2 format 2 only)
        if (cursor.operands.size() == 1) {
            ModificationRecord modRecord;
            modRecord.index = itr;
//            modRecord.labelToBeAdded = "START"; // modify to PROG name
            modRecord.labelToBeAdded = progName; // PROG name set at index 0
            modRecord.operation = "+";
            modRecord.address = hexaConverter.decimalToHex((hexaConverter.hexToDecimal(cursor.address) + 1));
            modRecord.halfBytes = "05"; //zawedto
            modificationRecords.push_back(modRecord);
        } else {
            evaluateModificationRecordExpression(false,itr, cursor.operands[0], references, cursor.address);
        }
    } else if (cursor.mnemonic == "WORD") {
        //TODO implementation
        /**
         * 1) check if it contains an expression
         * 2) check if it's a valid label
         * 3) if it's an expression get it's value and store it in obcode while skipping extRef
         * 4) add Modifications for extRef if it exists.
         */
        evaluateModificationRecordExpression(true,itr, cursor.operands[0], references, cursor.address);
    } else if (cursor.mnemonic == "BYTE") {
        //TODO implementation
        /**
         * 1) check if it contains an expression
         * 2) check if it's a valid label
         * 3) if it's an expression get it's value and store it in obcode while skipping extRef
         * 4) add Modifications for extRef if it exists.
         */
        evaluateModificationRecordExpression(true,itr, cursor.operands[0], references, cursor.address);
    }

}
