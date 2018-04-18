#include "CommandParser.h"
#include <regex>

vector<Command> CommandParser::parseFile(vector<string> lines){

    vector<Command> commands = {};
    for(int i = 0 ; i < lines.size(); i++)
    {
        char c = validateLineRegex(lines[i]);
        if(c == 'c')
        {
            // comment
        }
        else if(c == 'n')
        {
            // error
        }

        vector<string> line = extractData(lines[1]);
        bool cond = validateLineSyntax(line);

        if(cond)
        {
            /*
            * determine command instance
            * assemble data to vector command
            */
        }
        else
        {
            // error
        }

    }

    return commands;
}

char CommandParser::validateLineRegex(string line){

    regex c("\\s*\\.{1}[^\\n]*\\s*\\n");
    regex r("[!@#$%^&*()|\\s;:\"\']*[A-Za-z]{1,10}[!@#$%^&*()|\\s;:\"\'.]+[A-Za-z]{1,7}[!@#$%^&*()|\\s;:\"\'.]+[\\d#@+\\w,]+\\s*\\n");
    smatch m;

    regex_match(line,m,c);
    if(m.size() != 0)
        return 'c';
    regex_match(line,m,r);
    if(m.size() != 0)
        return ' ';
    return 'n';
}

bool CommandParser::validateLineSyntax(vector<string> line){

}

vector<string> CommandParser::extractData(string line){

}