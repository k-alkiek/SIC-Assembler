#include "FileWriter.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"

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


        if (it->label == "KITTEN") {
            file << "   ____" << endl;
            file <<         "  (.   \\"<< endl;
            file << "    \\  |   "<< endl;
            file << "     \\ |___(\\--/)"<< endl;
            file << "   __/    (  . . )      meow!"<< endl;
            file << "  \"\'._.    '-.O.'"<< endl;
            file << "       \'-.  \\ \"|\\"<< endl;
            file << "          \'.,,/\'.,,"<< endl;
        }
        if (it->label == "INFWAR") {
            file << endl;
            file << "iron man and black Panther will die at the end of infinity war" << endl;
            file << endl;
        }
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



void FileWriter::generateObjectCodeFile(string fileName , vector<string> objectCode,
                                        PrimaryData data,vector<ModificationRecord> modifications){
    HexaConverter hexaConverter;
    string result;
    result = "H";
    result += data.commands[0].label;

    int tmp = data.commands[0].label.length();
    while(tmp < 6){
        result += " ";
        tmp++;
    }

    tmp = data.startingAddress.length();
    while(tmp < 6){
        result += "0";
        tmp++;
    }
    result += data.startingAddress;

    tmp = data.programLength.length();
    while(tmp < 6){
        result += "0";
        tmp++;
    }
    result += data.programLength +"\n";
    int length = 0;
    int LIMIT = 62;
    string textRecord = "T";
    textRecord += data.startingAddress;
    string tmpRecord ="";
    int currentAddress = hexaConverter.hexToDecimal(data.startingAddress);
    vector<string>::iterator it = objectCode.begin();
    while(it != objectCode.end()){
        while (it != objectCode.end() && ((length + (*it).size())< LIMIT)){
            length += (*it).length();
            tmpRecord += (*it);
            ++it;
        }
        textRecord = "T" + hexaConverter.decimalToHex(currentAddress);
        currentAddress = currentAddress+length/2;
        if(hexaConverter.decimalToHex(length/2).size() == 1)
            textRecord += "0";
        textRecord += hexaConverter.decimalToHex(length/2)+tmpRecord;
        tmpRecord = "";
        result += textRecord +"\n";

        length = 0;
    }

    for(vector<ModificationRecord>::iterator it = modifications.begin();
            it != modifications.end();++it){
        result += "M";
//        string length = "05";
        ModificationRecord* record = &(*it);
        (*it).address = data.commands.at((*it).index).address;
        if(objectCode.at((*it).index).length() == 8) {
            (*it).address = hexaConverter.decimalToHex(hexaConverter.hexToDecimal((*it).address) + 3);
        } else {
            (*it).address = hexaConverter.decimalToHex(hexaConverter.hexToDecimal((*it).address) + 1);
        }
        tmp = (*it).address.length();
        while(tmp < 6){
            result += "0";
            tmp++;
        }
//        result += (*it).address + length;(*it).labelToBeAdded
        result += (*it).address + (*it).halfBytes;
        if((*it).labelToBeAdded != ""){
            result += (*it).operation + (*it).labelToBeAdded +"\n";
        } else {
            result += "+" + data.commands[0].label +"\n";
        }

    }

    result += "E";
    tmp = data.startingAddress.length();
    while(tmp < 6){
        result += "0";
        tmp++;
    }
    result += data.startingAddress;
    ofstream file;
    file.open (fileName);
    file<<result;
    file.close();

}






















