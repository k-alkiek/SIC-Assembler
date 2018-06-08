#ifndef ASSEMBLER_PASSTWOMANAGER_H
#define ASSEMBLER_PASSTWOMANAGER_H

#include <iostream>
#include <vector>
#include "../DTOs/PrimaryData.h"
#include "../DTOs/ModificationRecord.h"

using namespace std;

class PassTwoManager {

private:
    void checkForErrors(Command cursor);
    void update(Command cursor);
    void calculateLitrals();
    string convertCToObjCode(string str);
public:
    vector<string> generateObjectCode(PrimaryData primaryData);
    vector<ModificationRecord> getModifiactionRecords();
    vector<vector<string>> getDefRecord();
};


#endif