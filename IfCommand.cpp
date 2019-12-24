//
// Created by nerya on 16/12/2019.
//

#include "IfCommand.h"

int IfCommand::execute(vector<vector<std::__cxx11::string> > &detailsOfTheCommand, unsigned int index) {
    //we want to pass the vector of "{"
    unsigned indexToIf = index + 2;
    if (this->checkCondition(detailsOfTheCommand, index)) {
        //initialize indexToIf in each iteration
        indexToIf = index + 2;
        while (detailsOfTheCommand[index][indexToIf] != "}") {
            indexToIf = this->command->execute(detailsOfTheCommand, indexToIf);
        }
    }
    //we want to pass the vector of "}"
    if (detailsOfTheCommand[indexToIf + 1][0] == "}") {
        return indexToIf + 2;
    }
    return indexToIf + 1;
}

IfCommand::IfCommand() {}
