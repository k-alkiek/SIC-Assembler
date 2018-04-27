#include <iostream>
#include <vector>

using namespace std;

class Command {
public:
    string label;
    string mnemonic;
    vector<string> operands;
    int getNeededSpace();
    string address;
};