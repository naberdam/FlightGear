//
// Created by nerya on 16/12/2019.
//

#ifndef EX3_IFCOMMAND_H
#define EX3_IFCOMMAND_H

#include "ConditionParser.h"



class IfCommand : public ConditionParser {

public:
    int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;

    IfCommand();
};


#endif //EX3_IFCOMMAND_H
