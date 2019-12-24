//
// Created by nerya on 18/12/2019.
//

#include "StringToCommands.h"
#include "KnownFuncCommand.h"


StringToCommands *StringToCommands::instanceOfStringToCommands = 0;

Command* StringToCommands::getCommandFromMapOfCommandsByString(std::__cxx11::string nameOfCommand) {
    if (this->mapOfCommands.count(nameOfCommand)) {
        return this->mapOfCommands[nameOfCommand];
    }
    return this->mapOfCommands[""];
}

void StringToCommands::addCommandEqualWithoutVarToMap(std::__cxx11::string nameOfVar) {
    if (!this->mapOfCommands.count(nameOfVar)) {
        this->mapOfCommands[nameOfVar] = new EqualCommandWithoutVar();
    }
}

void StringToCommands::addKnownFuncCommandToMap(string nameOfVar, vector<vector<string>> vecExecutes) {
    if (!this->mapOfCommands.count(nameOfVar)) {
        this->mapOfCommands[nameOfVar] = new KnownFuncCommand(vecExecutes);
    }
}

