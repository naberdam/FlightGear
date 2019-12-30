

//
// Created by nerya on 20/12/2019.
//

#include <iostream>
#include "KnownFuncCommand.h"
#include "StringToCommands.h"

KnownFuncCommand::KnownFuncCommand(const vector<vector<string>> &detailsOfFuncDeclarationFromUnknownFunc)
        : detailsOfFuncDeclaration(detailsOfFuncDeclarationFromUnknownFunc) {}

int KnownFuncCommand::execute(vector<vector<string> > &detailsOfTheCommand, unsigned int index) {
    auto item = detailsOfTheCommand.begin();
    advance(item, ++index);
    detailsOfTheCommand.insert(item, this->detailsOfFuncDeclaration.begin(),
                               this->detailsOfFuncDeclaration.end());
    detailsOfTheCommand[index].push_back(detailsOfTheCommand[index - 1][1]);
    return index;
}
