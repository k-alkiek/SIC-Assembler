//
// Created by saraheldafrawy on 09/06/18.
//

#include <string>
#include <iostream>
#include <fstream>
#include "Logger.h"

using namespace std;

void Logger::infoMsg (string msg) {
    writeToFile(msg,"Info");
}

void Logger::errorMsg (string msg) {
    writeToFile(msg,"Error");
}

void Logger::writeToFile(string msg, string type) {

    ofstream outfile ("test.txt", ios::out | ios::app);

    outfile << "Logger "+ type + ": "+ msg << std::endl;

    outfile.close();

}
