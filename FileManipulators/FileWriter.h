#include<iostream>
#include <fstream>
#include "../DTOs/PrimaryData.h"
#include "../DTOs/ModificationRecord.h"

using namespace std;

class FileWriter {
public:
    void writeToFile(string fileName,PrimaryData data);
    void generateObjectCodeFile(string fileName , vector<string> objectCode,PrimaryData data,vector<ModificationRecord> modifications);
};


