//
// Created by saraheldafrawy on 04/06/18.
//

#ifndef ASSEMBLER_OBJECTCODECALCULATION_H
#define ASSEMBLER_OBJECTCODECALCULATION_H

#include <iostream>
#include <vector>
#include <map>
#include "../CommandsAndUtilities/Command.h"
#include "../DTOs/labelInfo.h"


using namespace std;

class ObjectCodeCalculation {

private:
    int getRegisterNumber(string registerr);

    string completeObjCodeFormat2(int uncompletedObjCode, vector <string> operands);

    string completeObjCodeFormat3(int uncompletedObjCode, vector <string> operands);

    string completeObjCodeFormat4(int uncompletedObjCode, vector <string> operands);

    vector<int> getFlagsCombination(vector <string> operands, int format, bool PCRelative);

    vector<int> getSimpleDisplacement(string TA, string progCounter);

    bool isExpression(string operand);

    string convertCToObjCode(basic_string<char, char_traits<char>, allocator<char>> basic_string);

public:
    string getObjectCode(Command cursor, string locationCounter, map<string, labelInfo> symbolTable);

};


#endif //ASSEMBLER_OBJECTCODECALCULATION_H
