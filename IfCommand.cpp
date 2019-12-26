//
// Created by nerya on 16/12/2019.
//

#include "IfCommand.h"
#include "StringToCommands.h"

int IfCommand::execute(vector<vector<std::__cxx11::string> > &detailsOfTheCommand, unsigned int index) {
    unsigned indexToIf = index;
    bool didEnterTheLoop = false;
    if (this->checkCondition(detailsOfTheCommand, index)) {
        didEnterTheLoop = true;
        //initialize indexToIf in each iteration
        indexToIf = index + 2;
        while (detailsOfTheCommand[indexToIf][0] != "}") {
            this->command = StringToCommands::getInstanceOfStringToCommands()->getCommandFromMapOfCommandsByString(detailsOfTheCommand[indexToIf][0]);
            if (this->command) {
                indexToIf = this->command->execute(detailsOfTheCommand, indexToIf);
            } else {
                ++indexToIf;
            }
        }
    }
    return increaseIndexByEnterConditionOrNot(didEnterTheLoop, indexToIf, detailsOfTheCommand);
}

IfCommand::IfCommand() {}
