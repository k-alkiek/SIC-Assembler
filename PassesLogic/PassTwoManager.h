#ifndef ASSEMBLER_PASSTWOMANAGER_H
#define ASSEMBLER_PASSTWOMANAGER_H

#include <iostream>
#include <vector>
#include "../DTOs/PrimaryData.h"

using namespace std;

class PassTwoManager {

private:
    vector<string> objectCode;
    string getObjectCode(Command command);
    int getRegisterNumber(string registerr);
    string completeObjCodeFormat2(int uncompletedObjCode, vector<string> operands);
    string completeObjCodeFormat3(int uncompletedObjCode,vector<string> operands);
    string completeObjCodeFormat4(int uncompletedObjCode,vector<string> operands);
    vector<int> getFlagsCombination(vector<string> operands, int format, int PCRelative);
    vector<int> getSimpleDisplacement(string TA, string progCounter);
    bool isExpression(string operand);

public:
    vector<string> generateObjectCode(PrimaryData primaryData);

    int getdispRange();
};


#endif