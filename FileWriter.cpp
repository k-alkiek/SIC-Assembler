#include "FileWriter.h"
void FileWriter::writeToFile(string fileName, PrimaryData data) {
    ofstream file;
    int count = 0;
    file.open (fileName);
    file<<"program name: "<<data.commands.at(0).label<<"\nProgram size: "<<data.programLength<<"\n\n";
    file<<"line    address     label     mnemonics     operands\n";
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
        if((*it).operands.size() != 0) {
            for (int i = 0; i < (*it).operands.size()-1; ++i) {
                file<<(*it).operands.at(0)<<",";
            }
            file<<(*it).operands.at((*it).operands.size() - 1)<<"\n";
        } else {
            file<<"\n";
        }
        if(data.errorMsgsMap.find(count-1) != data.errorMsgsMap.end())
            file<<"             ***"<<data.errorMsgsMap.find(count - 1)->second<<"\n";
    }
    if(data.errorMsgsMap.size() != 0){
        file<<"\n\n        UNSUCCESSFUL COMPILATION !\n";

    } else{
        int tmp;
        file<<"\n\nlabel        type          address\n";
        for(map<string,labelInfo >::const_iterator it = data.symbolTable.begin();
            it != data.symbolTable.end(); ++it)
        {
            file << it->first;
            tmp = it->first.size();
            while (tmp < 13) {
                file<<" ";
                tmp++;
            }
            file << it->second.type;
            tmp = it->second.type.size();
            while (tmp < 14) {
                file<<" ";
                tmp++;
            }
            file << it->second.address;
            file<<"\n";
        }

        file<<"\n\n        SUCCESSFUL COMPILATION !\n";
    }

    file.close();
}