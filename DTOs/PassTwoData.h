//
// Created by mostafa on 09/06/18.
//

#include <array>
#include <vector>
#include <map>
#include "ModificationRecord.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"

#ifndef ASSEMBLER_PASSTWODATA_H
#define ASSEMBLER_PASSTWODATA_H
using namespace std;
class PassTwoData{
public:
    string nextInstructionAddress;
    vector<vector<string>> DefRecord;
    vector<ModificationRecord> modificationRecords;
    vector<string> definitions;
    vector<string> references;
    map<string, string> defRecordUnsorted;
    map<string, string> extDefinitions;
    vector<string> litrals;
    vector<string> textRecord;
    HexaConverter hexaConverter;
    bool baseAvailable;
};
#endif //ASSEMBLER_PASSTWODATA_H
