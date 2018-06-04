#ifndef ASSEMBLER_PASSTWOMANAGER_H
#define ASSEMBLER_PASSTWOMANAGER_H

#include <iostream>
#include <vector>
#include "../DTOs/PrimaryData.h"
#include "../DTOs/ModificationRecord.h"

using namespace std;

class PassTwoManager {

private:
    void setDefRecord(map<string, string> defRecordsUnsorted, vector<string> definitions);
    bool containsExternalReference(string operands, vector<string> definitions);
    void evaluateModificationRecordExpression(bool constant,int itr, string expression, vector<string> extReferences, string addressInput);
    void addModificationRecord(Command cursor, int itr, vector<string> definitions,
                               vector<ModificationRecord> modificationRecords, vector<string> references);

public:
    vector<string> generateObjectCode(PrimaryData primaryData);

    bool checkAddProgName(string basic_string);
};


#endif