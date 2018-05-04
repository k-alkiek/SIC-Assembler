#ifndef ASSEMBLER_PASSTWOMANAGER_H
#define ASSEMBLER_PASSTWOMANAGER_H

#include <iostream>
#include <vector>
#include "../DTOs/PrimaryData.h"

using namespace std;

class PassTwoManager {

private:
    vector<string> objectCode;
    string getObjectCode(Command command, PrimaryData primaryData);
    int getRegisterNumber(string registerr);
    string completeObjCodeFormat2(int uncompletedObjCode, vector<string> operands);
    string completeObjCodeFormat3(int uncompletedObjCode,vector<string> operands);
    string completeObjCodeFormat4(int uncompletedObjCode,vector<string> operands,PrimaryData primaryData);
    vector<int> getFlagsCombination(string operand, int format);
    int getDisplacement();

public:
    vector<string> generateObjectCode(PrimaryData primaryData);
};


#endif