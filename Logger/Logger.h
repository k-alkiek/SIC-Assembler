//
// Created by saraheldafrawy on 09/06/18.
//

#ifndef ASSEMBLER_LOGGER_H
#define ASSEMBLER_LOGGER_H

using namespace std;

class Logger {

public:
    void errorMsg (string msg);
    void infoMsg (string msg);

private:
    void writeToFile(string msg, string type);

};


#endif //ASSEMBLER_LOGGER_H
