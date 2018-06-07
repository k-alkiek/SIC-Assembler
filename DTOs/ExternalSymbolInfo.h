

#ifndef ASSEMBLER_EXTERNALSYMBOLINFO_H
#define ASSEMBLER_EXTERNALSYMBOLINFO_H

#include <iostream>
using namespace std;

class ExternalSymbolInfo {

public:
    string sectionName;
    string address;
    int index;
};
#endif