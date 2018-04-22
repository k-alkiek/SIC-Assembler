#include <unordered_map>
#include "CommandIdentifier.h"

CommandIdentifier::CommandIdentifier(){
//    OperationInfo(code, hasOperand, numberOfOperands)
    opTable.insert(pair <string, OperationInfo> ("ADD", OperationInfo("18",true,1)));
    opTable.insert(pair <string, OperationInfo> ("ADDF", OperationInfo("58",true,1)));
    opTable.insert(pair <string, OperationInfo> ("ADDR", OperationInfo("90",true,2)));
    opTable.insert(pair <string, OperationInfo> ("AND", OperationInfo("40",true,1)));
    opTable.insert(pair <string, OperationInfo> ("CLEAR", OperationInfo("B4",true,1)));
    opTable.insert(pair <string, OperationInfo> ("COMP", OperationInfo("28",true,1)));
    opTable.insert(pair <string, OperationInfo> ("COMPF", OperationInfo("88",true,1)));
    opTable.insert(pair <string, OperationInfo> ("COMPR", OperationInfo("A0",true,2)));
    opTable.insert(pair <string, OperationInfo> ("DIV", OperationInfo("24",true,1)));
    opTable.insert(pair <string, OperationInfo> ("DIVF", OperationInfo("64",true,1)));
    opTable.insert(pair <string, OperationInfo> ("DIVR", OperationInfo("9C",true,2)));
    opTable.insert(pair <string, OperationInfo> ("FIX", OperationInfo("C4",false,0)));
    opTable.insert(pair <string, OperationInfo> ("FLOAT", OperationInfo("C0",false,0)));
    opTable.insert(pair <string, OperationInfo> ("HIO", OperationInfo("F4",false,0)));
    opTable.insert(pair <string, OperationInfo> ("J", OperationInfo("3C",true,1)));
    opTable.insert(pair <string, OperationInfo> ("JEQ", OperationInfo("30",true,1)));
    opTable.insert(pair <string, OperationInfo> ("JGT", OperationInfo("34",true,1)));
    opTable.insert(pair <string, OperationInfo> ("JLT", OperationInfo("38",true,1)));
    opTable.insert(pair <string, OperationInfo> ("JSUB", OperationInfo("48",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDA", OperationInfo("00",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDB", OperationInfo("68",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDCH", OperationInfo("50",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDF", OperationInfo("70",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDL", OperationInfo("08",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDS", OperationInfo("6C",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDT", OperationInfo("74",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDX", OperationInfo("04",true,1)));
    opTable.insert(pair <string, OperationInfo> ("LDS", OperationInfo("D0",true,1)));
    opTable.insert(pair <string, OperationInfo> ("MUL", OperationInfo("20",true,1)));
    opTable.insert(pair <string, OperationInfo> ("MULF", OperationInfo("60",true,1)));
    opTable.insert(pair <string, OperationInfo> ("MULR", OperationInfo("98",true,2)));
    opTable.insert(pair <string, OperationInfo> ("NORM", OperationInfo("C8",false,0)));
    opTable.insert(pair <string, OperationInfo> ("OR", OperationInfo("44",true,1)));
    opTable.insert(pair <string, OperationInfo> ("RD", OperationInfo("D8",true,1)));
    opTable.insert(pair <string, OperationInfo> ("RMO", OperationInfo("AC",true,2)));
    opTable.insert(pair <string, OperationInfo> ("RSUB", OperationInfo("4C",false,0)));
    opTable.insert(pair <string, OperationInfo> ("SHIFTL", OperationInfo("A4",true,2)));
    opTable.insert(pair <string, OperationInfo> ("SHIFTR", OperationInfo("A8",true,2)));
    opTable.insert(pair <string, OperationInfo> ("SIO", OperationInfo("F0",false,0)));
    opTable.insert(pair <string, OperationInfo> ("SSK", OperationInfo("EC",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STA", OperationInfo("0C",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STB", OperationInfo("78",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STCH", OperationInfo("54",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STF", OperationInfo("80",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STI", OperationInfo("D4",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STL", OperationInfo("14",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STS", OperationInfo("7C",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STSW", OperationInfo("E8",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STT", OperationInfo("84",true,1)));
    opTable.insert(pair <string, OperationInfo> ("STX", OperationInfo("10",true,1)));
    opTable.insert(pair <string, OperationInfo> ("SUB", OperationInfo("1C",true,1)));
    opTable.insert(pair <string, OperationInfo> ("SUBF", OperationInfo("5C",true,1)));
    opTable.insert(pair <string, OperationInfo> ("SUBR", OperationInfo("94",true,2)));
    opTable.insert(pair <string, OperationInfo> ("SVC", OperationInfo("B0",true,1)));
    opTable.insert(pair <string, OperationInfo> ("TD", OperationInfo("E0",true,1)));
    opTable.insert(pair <string, OperationInfo> ("TIO", OperationInfo("F8",false,0)));
    opTable.insert(pair <string, OperationInfo> ("TIX", OperationInfo("2C",true,1)));
    opTable.insert(pair <string, OperationInfo> ("TIXR", OperationInfo("B8",true,1)));
    opTable.insert(pair <string, OperationInfo> ("WD", OperationInfo("DC",true,1)));

    opTable.insert(pair <string, string> ("RESW", NULL));

}

bool CommandIdentifier::isInTable(string command) {
    return !(CommandIdentifier::opTable.find(command) == opTable.end());
}

OperationInfo CommandIdentifier::getInfo(string command) {
    return CommandIdentifier::opTable.at(command);
}