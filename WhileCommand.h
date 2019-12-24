//
// Created by nerya on 16/12/2019.
//

#ifndef EX3_WHILECOMMAND_H
#include "ConditionParser.h"

#define EX3_WHILECOMMAND_H



class WhileCommand : public ConditionParser {

public:
    WhileCommand();

    virtual int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index);



};


#endif //EX3_WHILECOMMAND_H
