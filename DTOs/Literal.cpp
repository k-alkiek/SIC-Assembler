//
// Created by khaled on 6/8/18.
//

#include "Literal.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"

Literal::Literal(string name, int currentLocation, int instructionSize) {
    this->name = name;
    process(currentLocation, instructionSize);
}

void Literal::process(int currentLocation, int instructionSize) {
    HexaConverter hexaConverter = HexaConverter();
    char type = name[1];
    address = hexaConverter.decimalToHex(currentLocation);

    if (name[0] != '=') {     // if literal is an =*
        value = hexaConverter.decimalToHex( hexaConverter.hexToDecimal(name) + instructionSize);
        while (value.length() < 6) {
            value = "0" + value;
        }
        this->name = "=*";
        space = 3;
    }
    else {
        string rawValue = name.substr(3, name.length() - 4);

        if (type == 'C') {
            value = hexaConverter.stringToHex(rawValue);
            space = rawValue.length();
        }
        else if (type == 'X') {
            if (rawValue.length() % 2 == 1) {
                rawValue = "0" + rawValue;
            }
            value = rawValue;
            space = rawValue.length()/2;
        }
        else if (type == 'W') {
            // TODO Khaled: Handle multiple words

            value = hexaConverter.decimalToHex(stoi(rawValue));
            if (value.length() > 6) {
                throw exception();
            }
            else {
                while (value.length() < 6) {
                    value = "0" + value;
                }
            }
            space = 3;
        }

    }


    while (address.length() < 4) {
        address = "0" + address;
    }


}


string Literal::getName() {
    return name;
}

string Literal::getAddress() {
    return address;
}

string Literal::getValue() {
    return value;
}

int Literal::getSpace() {
    return space;
}
