#ifndef ASSEMBLER_HEXACONVERTER_H
#define ASSEMBLER_HEXACONVERTER_H

#include <iostream>
using namespace std;

class HexaConverter {

public:
    int hexToDecimal(string hexValue);

    string decimalToHex(int decimalValue);

    string stringToHex(string str);
};


#endif