//
// Created by khaled on 6/8/18.
//

#include "Literal.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"

Literal::Literal(string name, string currentLocation) {
    this->name = name;
    process(currentLocation);
}

void Literal::process(string currentLocation) {
    char type = name[1];
    address = currentLocation;
    HexaConverter hexaConverter = HexaConverter();

    if (type == '*' && name.length() == 2) {
        value = currentLocation;
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
            space = rawValue.length();
        }
        else if (type == 'W') {
            // TODO Khaled: Handle multiple words
            if (rawValue.length() != 6) {
                throw exception();
            }
            value = hexaConverter.decimalToHex(stoi(rawValue));
            space = 3;
        }

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
