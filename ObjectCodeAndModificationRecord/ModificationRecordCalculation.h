//
// Created by saraheldafrawy on 07/06/18.
//

#ifndef ASSEMBLER_MODIFICATIONRECORDCALCULATION_H
#define ASSEMBLER_MODIFICATIONRECORDCALCULATION_H

#include <iostream>
#include <map>
#include <vector>
#include "../CommandsAndUtilities/Command.h"
#include "../DTOs/ModificationRecord.h"
#include "../DTOs/labelInfo.h"

using namespace std;

class ModificationRecordCalculation {
private:

    bool containsExternalReference(string operands, vector<string> definitions);
    void evaluateModificationRecordExpression(bool constant,int itr, string expression, vector<string> extReferences,
                                              string addressInput, vector<string> definitions);
    bool isExpression(string operand);
    bool checkStar(vector<string> operands);
    vector<string> parseExpression(string expression);
    bool findInExpression(string word,vector<string> expression);
    int checkAddProgName(string basic_string,vector<string> definitions);

public:
    void setPrimaryDataNeeded (string name, map<string, labelInfo> symbolTable);
    void addModificationRecord(Command cursor, int itr, vector<string> definitions, vector<string> references);
    vector<vector<string>> setDefRecord(map<string, string> defRecordsUnsorted, vector<string> definitions);
    vector<ModificationRecord> getModificationRecords();
};


#endif //ASSEMBLER_MODIFICATIONRECORDCALCULATION_H
