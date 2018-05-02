#include<iostream>
#include <fstream>
#include "PrimaryData.h"

using namespace std;

class FileWriter {
public:
    void writeToFile(string fileName,PrimaryData data);
    void generateObjectCodeFile(string fileName , vector<string> objectCode,PrimaryData data);
};


