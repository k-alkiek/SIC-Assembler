#include <iostream>
#include <vector>

using namespace std;

class ErrorMsg {
public:
    int index;
    string msg;
    void setAttrib(int index, string msg);
};