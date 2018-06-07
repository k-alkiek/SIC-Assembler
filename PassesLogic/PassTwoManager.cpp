#include "PassTwoManager.h"
#include "../CommandsAndUtilities/CommandIdentifier.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"
#include "../ConvertersAndEvaluators/ExpressionEvaluator.h"
#include "../ObjectCodeAndModificationRecord/ObjectCodeCalculation.h"
#include "../ObjectCodeAndModificationRecord/ModificationRecordCalculation.h"
#include <cmath>
#include <sstream>


string locationCounter;
vector<vector<string>> DefRecord;
vector<ModificationRecord> modificationRecords;


//TODO LTORG and litteral pools

vector<string> PassTwoManager::generateObjectCode(PrimaryData primaryData) {
    ModificationRecordCalculation modificationRecordCalculation;
    ObjectCodeCalculation objectCodeCalculator;
    map<string, string> extDefinitions;
    vector<string> definitions; // TODO modify it to be vector from primary data
    vector<string> references; // TODO modify it to be vector from primary data
    //extDefinitions = PrimaryData.getExtDef()
    //extReferences = PrimaryData.getExtRef()
    modificationRecordCalculation.setPrimaryDataNeeded(primaryData.commands[0].label, primaryData.symbolTable);
    vector<string> textRecord;
    map<string, string> defRecordUnsorted;

    Command cursor;
    vector<Command> commands = primaryData.commands;
    cursor = commands[1];
    locationCounter = primaryData.startingAddress;
    int itr = 1;
    while (cursor.mnemonic != "END") {
        if (extDefinitions.find(cursor.label) != extDefinitions.end()) { // D^LISTA^000040
            vector<string> dRec;
            defRecordUnsorted.insert(cursor.label, cursor.address);
        }
        modificationRecordCalculation.addModificationRecord(cursor, itr, definitions, references);

        locationCounter = commands[itr + 1].address;
        textRecord.push_back(objectCodeCalculator.getObjectCode(cursor,locationCounter,primaryData.symbolTable));
        cursor = commands[++itr];
    }
    DefRecord = modificationRecordCalculation.setDefRecord(defRecordUnsorted, definitions);
    modificationRecords = modificationRecordCalculation.getModificationRecords();
}

vector<vector<string>> getDefRecord() {
    return DefRecord;
}
vector<vector<string>> getModifiactionRecords() {
    return DefRecord;
}

