#include <iostream>
#include "FileManipulators/FileWriter.h"
#include "CommandsAndUtilities/CommandParser.h"
#include "PassesLogic/PassOneManager.h"
#include "PassesLogic/PassTwoManager.h"
#include "FileManipulators/FileReader.h"

int main() {

    FileReader reader;
    CommandParser commandParser;
    PassOneManager manager;
    vector<PrimaryData> data;

    vector<string> lines = reader.readFile("/home/mostafa/CLionProjects/SIC-Assembler/test2.txt");
    if (lines.size() == 0) {

        ofstream file;
        file.open("Output.txt");
        file << "the input file is empty";
        return 0;
    }
    CommandParser parser;
    FileWriter writer;
    vector<vector<Command>> commands = parser.parseFile(lines);
    vector<vector<ErrorMsg>> wrongCommands = parser.getWrongCommands();
    vector<vector<ModificationRecord>> modificationRecords;
    vector<vector<string>> objectCode;

    bool noError = true;
    for (int i = 0; i < commands.size(); i++) {
        PassOneManager passOne;
        PrimaryData primaryData = passOne.loop(commands[i], wrongCommands[i]);
        if (primaryData.errorMsgsMap.size() != 0)
            noError = false;
        data.push_back(primaryData);
    }

    writer.writeToFile("IntermediateFile.txt", data);

    if (noError) {
        for (int i = 0; i < data.size(); i++) {

            PassTwoManager passTwo;
            PassTwoData results = passTwo.generateObjectCode(data[i]);
            vector<ModificationRecord> mod = results.modificationRecords;
            vector<string> textRec = results.textRecord;
            objectCode.push_back(textRec);
            modificationRecords.push_back(mod);
        }
        writer.generateObjectCodeFileWithSeparators("Output.txt", objectCode, data, modificationRecords);
    } else {
        ofstream file;
        file.open("Output.txt");
        file << "there are errors";
        return 0;
    }


//    for(int i = 0; i < textRec.size(); i++){
//        std::cout << (textRec[i]) << "\n";
//    }
    return 0;

}