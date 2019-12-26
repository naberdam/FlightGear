//
// Created by nerya on 16/12/2019.
//

#include "WhileCommand.h"
#include "StringToCommands.h"


int WhileCommand::execute(vector<vector<std::__cxx11::string> > &detailsOfTheCommand, unsigned int index) {
    //we want to pass the vector of "{"
    unsigned indexToWhile = index;
    bool didEnterTheLoop = false;
    StringToCommands *stringToCommands = stringToCommands->getInstanceOfStringToCommands();
    while (this->checkCondition(detailsOfTheCommand, index)) {
        didEnterTheLoop = true;
        //initialize indexToWhile in each iteration
        indexToWhile = index + 2;
        while (detailsOfTheCommand[indexToWhile][0] != "}") {
            this->command = stringToCommands->getCommandFromMapOfCommandsByString(detailsOfTheCommand[indexToWhile][0]);
            if (this->command) {
                indexToWhile = this->command->execute(detailsOfTheCommand, indexToWhile);
            } else {
                indexToWhile++;
            }
        }
    }
    return increaseIndexByEnterConditionOrNot(didEnterTheLoop, indexToWhile, detailsOfTheCommand);

}

WhileCommand::WhileCommand() {}


