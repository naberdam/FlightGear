//
// Created by nerya on 20/12/2019.
//

#ifndef EX3_KNOWNFUNCCOMMAND_H
#define EX3_KNOWNFUNCCOMMAND_H

#include "Command.h"


class KnownFuncCommand : public Command {
private:
    vector<vector<string >> detailsOfFuncDeclaration;

public:
    KnownFuncCommand(const vector<vector<string>> &detailsOfFuncDeclaration);

    int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;

};


#endif //EX3_KNOWNFUNCCOMMAND_H
