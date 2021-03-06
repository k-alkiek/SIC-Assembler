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
#include "../DTOs/Literal.h"

using namespace std;

class ModificationRecordCalculation {
private:

    bool containsExternalReference(string operands, vector<string> definitions);
    void evaluateModificationRecordExpression(bool constant,int itr, string expression, vector<string> extReferences,
                                              string addressInput, vector<string> definitions);
    void astrickModificationRecord(int index, Command cursor,string halfBytes);
    bool isExpression(string operand);
    int checkAddProgName(string basic_string,vector<string> definitions);
    void checkForErrors(Command cursor,vector<string> references);

public:
    void setPrimaryDataNeeded (string name, map<string, labelInfo> symbolTable);
    void addModificationRecord(Command cursor, int itr, vector<string> definitions, vector<string> references);
    void astrickLiteralModificationRecord(vector<string> astrickAddresses, string halfBytes,
                                          map<string, Literal> literalTable);
    void clearModRecVector();
//    vector<vector<string>> setDefRecord(map<string, string> defRecordsUnsorted, vector<string> definitions);
    vector<ModificationRecord> getModificationRecords();
};


#endif //ASSEMBLER_MODIFICATIONRECORDCALCULATION_H
