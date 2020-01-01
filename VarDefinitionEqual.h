//
// Created by nerya on 17/12/2019.
//

#ifndef EX3_VARDEFINITIONEQUAL_H
#define EX3_VARDEFINITIONEQUAL_H

#include "Command.h"

class VarDefinitionEqual : public Command {

 public:
  int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;
};

#endif //EX3_VARDEFINITIONEQUAL_H
