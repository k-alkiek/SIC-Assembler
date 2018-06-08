//
// Created by khaled on 6/8/18.
//

#ifndef ASSEMBLER_LITERAL_H
#define ASSEMBLER_LITERAL_H

#include <string>
using namespace std;

class Literal {
private:
    string name;
    string value;
    unsigned long space;
    void process(string currentLocation);
public:
    Literal(string name, string currentLocation);
    string getName();
    string getValue();
    int getSpace();

};

#endif //ASSEMBLER_LITERAL_H
