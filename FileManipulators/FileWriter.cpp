#include "FileWriter.h"
#include "../ConvertersAndEvaluators/HexaConverter.h"
#include "../Logger/Logger.h"

void FileWriter::writeToFile(string fileName, vector<PrimaryData> data) {
    Logger logWriterIntermediate;

    ofstream file;
    int count = 0;
    int lines = 0;
    file.open (fileName);
    vector<PrimaryData>::iterator dataIterator;
    for (dataIterator = data.begin(); dataIterator !=  data.end() ; ++dataIterator) {

        count = 0;
        file << "program name: " << (*dataIterator).programName << "\nProgram size: " << (*dataIterator).programLength
             << "\n\n";
        file << "line    address     label     mnemonics     operands\n";
        for (vector<Command>::iterator it = (*dataIterator).commands.begin();
             it != (*dataIterator).commands.end(); ++it) {
            count++;
            lines++;
            file << lines;
            int tmp = to_string(lines).size();
            while (tmp < 8) {
                file << " ";
                tmp++;
            }
            file << (*it).address;
            tmp = (*it).address.size();
            while (tmp < 12) {
                file << " ";
                tmp++;
            }
            file << (*it).label;


            tmp = (*it).label.size();
            while (tmp < 10) {
                file << " ";
                tmp++;
            }
            file << (*it).mnemonic;
            tmp = (*it).mnemonic.size();
            while (tmp < 14) {
                file << " ";
                tmp++;
            }
            if ((*it).operands.size() != 0) {
                for (int i = 0; i < (*it).operands.size() - 1; ++i) {
                    file << (*it).operands.at(i) << ",";
                }
                file << (*it).operands.at((*it).operands.size() - 1) << "\n";
            } else {
                file << "\n";
            }
            if ((*dataIterator).errorMsgsMap.find(count - 1) != (*dataIterator).errorMsgsMap.end())
                file << "             ***" << (*dataIterator).errorMsgsMap.find(count - 1)->second << "\n";


            if (it->label == "KITTEN") {
                file << "   ____" << endl;
                file << "  (.   \\" << endl;
                file << "    \\  |   " << endl;
                file << "     \\ |___(\\--/)" << endl;
                file << "   __/    (  . . )      meow!" << endl;
                file << "  \"\'._.    '-.O.'" << endl;
                file << "       \'-.  \\ \"|\\" << endl;
                file << "          \'.,,/\'.,," << endl;
            }
            if (it->label == "INFWAR") {
                file << endl;
                file << "iron man and black Panther will die at the end of infinity war" << endl;
                file << endl;
            }
        }
        if ((*dataIterator).errorMsgsMap.size() != 0) {
            logWriterIntermediate.infoMsg("FileWriter: Unsuccessful Compilation");
            file << "\n\n        UNSUCCESSFUL COMPILATION !\n";

        } else {
            int tmp;
            file << "\n\nlabel        type          address\n";
            for (map<string, labelInfo>::const_iterator it = (*dataIterator).symbolTable.begin();
                 it != (*dataIterator).symbolTable.end(); ++it) {
                file << it->first;
                tmp = it->first.size();
                while (tmp < 13) {
                    file << " ";
                    tmp++;
                }
                file << it->second.type;
                tmp = it->second.type.size();
                while (tmp < 14) {
                    file << " ";
                    tmp++;
                }
                file << it->second.address;
                file << "\n";
            }

            file << "\n Literals\n";
            file << "\nname            value          address\n";
            for (map<string, Literal>::const_iterator it = (*dataIterator).literalTable.begin();
                 it != (*dataIterator).literalTable.end(); ++it) {
                string name = it->first;
                if (name[0] != '=') {
                    name = "=* @" + name;
                }
                file << name;
                tmp = name.size();
                while (tmp < 16) {
                    file << " ";
                    tmp++;
                }

                Literal literal = it->second;
                file << literal.getValue();
                tmp = literal.getValue().length();
                while (tmp < 15) {
                    file << " ";
                    tmp++;
                }
                file << literal.getAddress();
                file << "\n";
            }
            logWriterIntermediate.infoMsg("FileWriter: Successful Compilation");
            file << "\n\n        SUCCESSFUL COMPILATION !\n\n";
        }
    }
    file.close();
}



void FileWriter::generateObjectCodeFile(string fileName , vector<vector<string>> objectCode,
                                        vector<PrimaryData> data,vector<vector<ModificationRecord>> modifications){
    HexaConverter hexaConverter;
    string result = "";
    for (int i = 0; i < objectCode.size(); ++i) {
        result += "H";
        result += data[i].commands[0].label;
        int tmp = data[i].commands[0].label.length();
        while (tmp < 6) {
            result += " ";
            tmp++;
        }

        tmp = data[i].startingAddress.length();
        while (tmp < 6) {
            result += "0";
            tmp++;
        }
        result += data[i].startingAddress;

        tmp = data[i].programLength.length();
        while (tmp < 6) {
            result += "0";
            tmp++;
        }
        result += data[i].programLength + "\n";
        if(data[i].externalSymbols.size() != 0) {
            result += "D";
            int defLength = 0;
            int defLengthLimit = 73 - 7;
            for (std::map<string, ExternalSymbolInfo>::iterator iter = data[i].externalSymbols.begin();
                 iter != data[i].externalSymbols.end(); ++iter) {
                string symbol = iter->first;
                string externalDef = symbol;
                tmp = symbol.length();
                while (tmp < 6) {
                    externalDef += " ";
                    tmp++;
                }
                tmp = iter->second.address.length();
                while (tmp < 6) {
                    externalDef += "0";
                    tmp++;
                }
                externalDef += iter->second.address;

                if (defLength + externalDef.length() <= defLengthLimit) {
                    result += externalDef;
                    defLength += externalDef.length();
                } else {
                    result += "\nD" + externalDef;
                    defLength = externalDef.length();
                }
            }
            result += "\n";
        }
        if(data[i].externalReference.size() != 0) {
            result += "R";
            int refLength = 0;
            int refLengthLimit = 73 - 7;

            for (std::map<string, ExternalSymbolInfo>::iterator iter = data[i].externalReference.begin();
                 iter != data[i].externalReference.end(); ++iter) {
                string symbol = iter->first;
                string externalRef = symbol;
                tmp = symbol.length();
                while (tmp < 6) {
                    externalRef += " ";
                    tmp++;
                }
                if (refLength + externalRef.length() <= refLengthLimit) {
                    result += externalRef;
                    refLength += externalRef.length();
                } else {
                    result += "\nR" + externalRef;
                    refLength = externalRef.length();
                }
            }
            result += "\n";
        }
        int length = 0;
        int LIMIT = 60;
        string textRecord = "T";
        textRecord += data[i].startingAddress;
        string tmpRecord = "";
        int currentAddress = hexaConverter.hexToDecimal(data[i].startingAddress);
        vector<string>::iterator it = objectCode[i].begin();
        vector<Command>::iterator comIt = data[i].commands.begin();
        bool broke = false;
        while (it != objectCode[i].end()) {
            while (it != objectCode[i].end() && ((length + (*it).size()) < LIMIT)) {
                length += (*it).length();
                if((*it).length() != 0) {
                    tmpRecord += (*it);
                }
                if((*comIt).mnemonic == "RESW" || (*comIt).mnemonic == "RESB") {
                    if(tmpRecord.length() != 0) {
                        ++comIt;
                        ++it;
                        broke = true;
                        break;
                    } else {
                        comIt++;
                        currentAddress = hexaConverter.hexToDecimal(((*comIt).address));
                        comIt--;
                    }
                }
                ++comIt;
                ++it;
            }
            if(tmpRecord.length() !=0) {
                textRecord = "T";

                string add = hexaConverter.decimalToHex(currentAddress);
                tmp = add.length();
                while (tmp < 6) {
                    textRecord += "0";
                    tmp++;
                }
                textRecord += add;
                currentAddress = currentAddress + length / 2;
                if (hexaConverter.decimalToHex(length / 2).size() == 1)
                    textRecord += "0";
                textRecord += hexaConverter.decimalToHex(length / 2) + tmpRecord;
                tmpRecord = "";
                result += textRecord + "\n";
            }
            length = 0;
        }
        for (vector<ModificationRecord>::iterator it = modifications[i].begin();
             it != modifications[i].end(); ++it) {
            result += "M";
            ModificationRecord *record = &(*it);
            tmp = (*it).address.length();
            while (tmp < 6) {
                result += "0";
                tmp++;
            }
            result += (*it).address ;
            result +=  (*it).halfBytes;
            if ((*it).labelToBeAdded != "") {
                result += (*it).operation + (*it).labelToBeAdded + "\n";
            } else {
                result += "+" + data[i].commands[0].label + "\n";
            }

        }

        result += "E";
        tmp = data[i].commands[data[i].commands.size() - 1].operands.size();
        if (tmp != 0) {
            string name = data[i].commands[data[i].commands.size() - 1].operands.at(0);
            string address = data[i].symbolTable.find(name)->second.address;
            tmp = address.length();
            while (tmp < 6) {
                result += "0";
                tmp++;
            }

            result += data[i].symbolTable.find(data[i].commands[data[i].commands.size() - 1].operands.at(0))->second.address;
        }
        result +="\n\n";

    }
    ofstream file;
    file.open (fileName);
    file<<result;
    file.close();

}

void FileWriter::generateObjectCodeFileWithSeparators(string fileName , vector<vector<string>> objectCode,
                                        vector<PrimaryData> data,vector<vector<ModificationRecord>> modifications){
    HexaConverter hexaConverter;
    string result = "";
    string separator = "^";
    for (int i = 0; i < objectCode.size(); ++i) {
        result += "H" + separator;
        result += data[i].commands[0].label;
        int tmp = data[i].commands[0].label.length();
        while (tmp < 6) {
            result += " ";
            tmp++;
        }
        result +=separator;
        tmp = data[i].startingAddress.length();
        while (tmp < 6) {
            result += "0";
            tmp++;
        }
        result += data[i].startingAddress +separator;

        tmp = data[i].programLength.length();
        while (tmp < 6) {
            result += "0";
            tmp++;
        }
        result += data[i].programLength + separator+"\n";
        if(data[i].externalSymbols.size() != 0) {
            result += "D" + separator;
            int defLength = 0;
            int defLengthLimit = 73 - 7;
            for (std::map<string, ExternalSymbolInfo>::iterator iter = data[i].externalSymbols.begin();
                 iter != data[i].externalSymbols.end(); ++iter) {
                string symbol = iter->first;
                string externalDef = symbol;
                tmp = symbol.length();
                while (tmp < 6) {
                    externalDef += " ";
                    tmp++;
                }
                externalDef += separator;
                tmp = iter->second.address.length();
                while (tmp < 6) {
                    externalDef += "0";
                    tmp++;
                }
                externalDef += iter->second.address +separator;

                if (defLength + externalDef.length() <= defLengthLimit) {
                    result += externalDef;
                    defLength += externalDef.length();
                } else {
                    result += "\nD"+separator + externalDef;
                    defLength = externalDef.length();
                }
            }
            result += "\n";
        }
        if(data[i].externalReference.size() != 0) {
            result += "R" + separator;
            int refLength = 0;
            int refLengthLimit = 73 - 7;

            for (std::map<string, ExternalSymbolInfo>::iterator iter = data[i].externalReference.begin();
                 iter != data[i].externalReference.end(); ++iter) {
                string symbol = iter->first;
                string externalRef = symbol;
                tmp = symbol.length();
                while (tmp < 6) {
                    externalRef += " ";
                    tmp++;
                }
                externalRef += separator;
                if (refLength + externalRef.length() <= refLengthLimit) {
                    result += externalRef;
                    refLength += externalRef.length();
                } else {
                    result += "\nR"+separator + externalRef;
                    refLength = externalRef.length();
                }
            }
            result += "\n";
        }
        int length = 0;
        int LIMIT = 60;
        string textRecord = "T"+separator;
        textRecord += data[i].startingAddress;
        string tmpRecord = "";
        int currentAddress = hexaConverter.hexToDecimal(data[i].startingAddress);
        vector<string>::iterator it = objectCode[i].begin();
        vector<Command>::iterator comIt = data[i].commands.begin();
        bool broke = false;
        while (it != objectCode[i].end()) {
            while (it != objectCode[i].end() && ((length + (*it).size()) < LIMIT)) {
                length += (*it).length();
                if((*it).length() != 0) {
                    tmpRecord += (*it) + separator;
                }
                if(comIt != data[i].commands.end()&&((*comIt).mnemonic == "RESW" || (*comIt).mnemonic == "RESB")) {
                    if(tmpRecord.length() != 0) {
                        ++comIt;
                        ++it;
                        broke = true;
                        break;
                    } else {
                        comIt++;
                        currentAddress = hexaConverter.hexToDecimal(((*comIt).address));
                        comIt--;
                    }
                }
                ++comIt;
                ++it;
            }
            if(tmpRecord.length() != 0) {
                textRecord = "T" + separator;

                string add = hexaConverter.decimalToHex(currentAddress);
                tmp = add.length();
                while (tmp < 6) {
                    textRecord += "0";
                    tmp++;
                }
                textRecord += add + separator;
                if (!broke) {
                    currentAddress = currentAddress + length / 2;
                } else if(comIt != data[i].commands.end()){
                    currentAddress = hexaConverter.hexToDecimal((*comIt).address);
                } else {
                    currentAddress = 0;
                }
                if (hexaConverter.decimalToHex(length / 2).size() == 1)
                    textRecord += "0";
                textRecord += hexaConverter.decimalToHex(length / 2) + separator + tmpRecord;
                tmpRecord = "";
                result += textRecord + "\n";
            }
            length = 0;
        }
        for (vector<ModificationRecord>::iterator it = modifications[i].begin();
             it != modifications[i].end(); ++it) {
            result += "M" +separator;
            ModificationRecord *record = &(*it);
            tmp = (*it).address.length();
            while (tmp < 6) {
                result += "0";
                tmp++;
            }
            result += (*it).address +separator;
            result +=  (*it).halfBytes + separator;
            if ((*it).labelToBeAdded != "") {
                result += (*it).operation + (*it).labelToBeAdded + "\n";
            } else {
                result += "+" + data[i].commands[0].label + "\n";
            }

        }

        result += "E" + separator;
        tmp = data[i].commands[data[i].commands.size() - 1].operands.size();
        if (tmp != 0) {
            string name = data[i].commands[data[i].commands.size() - 1].operands.at(0);
            string address = data[i].symbolTable.find(name)->second.address;
            tmp = address.length();
            while (tmp < 6) {
                result += "0";
                tmp++;
            }

            result += data[i].symbolTable.find(data[i].commands[data[i].commands.size() - 1].operands.at(0))->second.address;
        }
        result +=separator +"\n\n";

    }
    ofstream file;
    file.open (fileName);
    file<<result;
    file.close();

}




















