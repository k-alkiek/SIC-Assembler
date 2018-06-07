#ifndef ASSEMBLER_PASSTWOMANAGER_H
#define ASSEMBLER_PASSTWOMANAGER_H

#include <iostream>
#include <vector>
#include "../DTOs/PrimaryData.h"
#include "../DTOs/ModificationRecord.h"

using namespace std;

class PassTwoManager {

public:
    vector<string> generateObjectCode(PrimaryData primaryData);
    vector<vector<string>> getModifiactionRecords();
    vector<vector<string>> getDefRecord();
};


#endif