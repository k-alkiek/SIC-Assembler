#ifndef ASSEMBLER_PASSTWOMANAGER_H
#define ASSEMBLER_PASSTWOMANAGER_H

#include <iostream>
#include <vector>
#include "../DTOs/PrimaryData.h"
#include "../DTOs/ModificationRecord.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"
#include "../CommandsAndUtilities/CommandIdentifier.h"
#include "../DTOs/PassTwoData.h"


using namespace std;

class PassTwoManager {

private:
    CommandIdentifier commandIdentifier;
    void checkForErrors(Command cursor,PassTwoData data);
//    string convertCToObjCode(string str,PassTwoData data);
    bool noObjCode(string mnemonic);
    vector<string> updateDataVectors(Command cursor,vector<string> data);
public:
    PassTwoData  generateObjectCode(PrimaryData primaryData);

};


#endif