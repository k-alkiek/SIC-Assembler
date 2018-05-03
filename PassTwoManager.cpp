#include "PassTwoManager.h"
#include "CommandIdentifier.h"

vector<string> PassTwoManager::generateObjectCode(PrimaryData primaryData) {

    vector<Command> commands = primaryData.commands;
    Command cursor = commands[0];
    vector<string> objectCode;

    if (cursor.mnemonic == "Start") {
        objectCode.push_back("H");
        if (cursor.label != "") { //hwa yenfa3 mayeb2ash fei label?!
            objectCode.push_back(cursor.label);
        } else {
            //error program name not found
        }
        objectCode.push_back(primaryData.startingAddress);
        objectCode.push_back(primaryData.programLength);
    } else {
        //error not found
    }
     objectCode.push_back("T");
     cursor = commands[1];
     int itr = 1;
     while (cursor.mnemonic != "END") {
         objectCode.push_back(getObjectCode(cursor));

          cursor = commands[++itr];
     }
     objectCode.push_back("E");
     objectCode.push_back(primaryData.startingAddress); //first excutable instruction

}

string PassTwoManager::getObjectCode(Command cursor) {
    CommandIdentifier opTable;

    if (cursor.mnemonic == "BYTE" || cursor.mnemonic == "WORD") {
        //convert constant to object code
    } else if (opTable.isInTable(cursor.mnemonic)) {
        string str = opTable.getInfo(cursor.mnemonic).code;
//             if(isSymbol(cursor.operands)) {
//                 /**
//                  * if there is a symbol in OPERAND field then
//     *               begin
//     *                 search SYMTAB for OPERAND
//     *                 if found then
//     *                    store symbol value as operand address
//     *                 else
//     *                    begin
//     *                      store 0 as operand address
//     *                      set error flag (undefined symbol)
//     *                    end
//     *               end {if symboll}
//     *              else
//                  */
//             } else {
//                 //store 0 as operand address
//             }
        //assemble the object code instruction
    }
}
