//
// Created by nerya on 17/12/2019.
//

#include <iostream>
#include "ParserCommands.h"
#include "StringToCommands.h"


ParserCommands::ParserCommands() {}

void ParserCommands::parser(vector<vector<std::__cxx11::string> > lexerVector) {
    unsigned int index = 0;
    StringToCommands* stringToCommand = stringToCommand->getInstanceOfStringToCommands();
    Command* command;
    while (index < lexerVector.size()) {
        command = stringToCommand->getCommandFromMapOfCommandsByString(lexerVector[index][0]);
        if (command != nullptr) {
            try {
                index = command->execute(lexerVector, index);
            } catch (char const * exc1) {
                cout << exc1 << endl;
                index++;
            }
        }
    }
    VariablesSingelton::getInstanceOfVariablesSingelton()->disconnectMe();
    delete stringToCommand;

}
