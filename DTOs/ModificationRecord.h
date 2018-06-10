#ifndef ASSEMBLER_MODIFICATIONRECORD_H
#define ASSEMBLER_MODIFICATIONRECORD_H

#include <iostream>
using namespace std;

class ModificationRecord{
public:
    int index;
    string address;
    string labelToBeAdded;
    string operation;
    string halfBytes;
};
#endif