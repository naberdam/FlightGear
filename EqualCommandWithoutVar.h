//
// Created by nerya on 17/12/2019.
//

#ifndef EX3_EQUALCOMMANDWITHOUTVAR_H
#define EX3_EQUALCOMMANDWITHOUTVAR_H

#include "Command.h"


class EqualCommandWithoutVar : public Command {
public:
    int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;
};


#endif //EX3_EQUALCOMMANDWITHOUTVAR_H
