

#ifndef ERRORMSG_H
#define ERRORMSG_H


#include <iostream>
#include <vector>

using namespace std;

class ErrorMsg {
public:
    int index;
    string msg;
    void setAttrib(int index, string msg);
};

#endif