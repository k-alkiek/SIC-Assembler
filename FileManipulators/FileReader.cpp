#include "FileReader.h"

vector<string> FileReader::readFile(string fileName){
    ifstream infile(fileName);
    if(!infile) {
        cout << "Cannot open input file.\n";
    }
    std::string line;
    vector<string> commands;
    while (getline(infile, line)) {
        istringstream iss(line);
        for (string::size_type i=0; i<line.length(); ++i)
            line[i] = toupper(line[i]);
        commands.push_back(line);
    }
    return commands;
}
