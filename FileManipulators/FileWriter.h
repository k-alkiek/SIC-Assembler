#include<iostream>
#include <fstream>
#include "../DTOs/PrimaryData.h"
#include "../DTOs/ModificationRecord.h"

using namespace std;

class FileWriter {
public:
    void writeToFile(string fileName,vector<PrimaryData> data);
    void generateObjectCodeFile(string fileName , vector<vector<string>> objectCode,
                                vector<PrimaryData> data,vector<vector<ModificationRecord>> modifications);
    void generateObjectCodeFileWithSeparators(string fileName , vector<vector<string>> objectCode,
            vector<PrimaryData> data,vector<vector<ModificationRecord>> modifications);
};


