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

    ofstream outfile ("/home/saraheldafrawy/CLionProjects/SIC-Assembler/logout.txt", ios::out | ios::app);

    outfile << "Logger "+ type + ": " + msg << std::endl;

    outfile.close();

}

void Logger:: clear() {
    std::ofstream logFile;
    logFile.open("/home/saraheldafrawy/CLionProjects/SIC-Assembler/logout.txt", std::ofstream::out | std::ofstream::trunc);
    logFile.close();
}
