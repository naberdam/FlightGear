//
// Created by nerya on 15/12/2019.
//

#ifndef EX3_VARDEFINITIONLEFT_H
#define EX3_VARDEFINITIONLEFT_H

#include "Command.h"

class VarDefinitionLeft : public Command {

public:
    int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;
};


#endif //EX3_VARDEFINITIONLEFT_H
