//
// Created by abdelrahman on 27/04/18.
//

#include<iostream>
#include <fstream>
#include "PrimaryData.h"

using namespace std;

class FileWriter {
public:
    void writeToFile(string fileName,PrimaryData data){
        ofstream file;
        int count = 0;
        file.open (fileName);
        file<<"line    address     label     mnemonics     operands      comments\n";
        for(vector<Command>::iterator it = data.commands.begin(); it != data.commands.end();++it){
            count++;
            file <<count;
            int tmp = to_string(count).size();
            while (tmp < 8) {
                file<<" ";
                tmp++;
            }
            file <<(*it).address;
            tmp = (*it).address.size();
            while (tmp < 12) {
                file<<" ";
                tmp++;
            }
            file <<(*it).label;
            tmp = (*it).label.size();
            while (tmp < 10) {
                file<<" ";
                tmp++;
            }
            file <<(*it).mnemonic;
            tmp = (*it).mnemonic.size();
            while (tmp < 14) {
                file<<" ";
                tmp++;
            }
            if((*it).operands.size()!=0)
                file<<(*it).operands.at(0)<<"\n";

        }
        file.close();
    }
};


