//
// Created by nerya on 20/12/2019.
//

#ifndef EX3_UNKNOWNFUNCCOMMAND_H
#define EX3_UNKNOWNFUNCCOMMAND_H

#include "Command.h"

class UnknownFuncCommand : public Command {

 public:
  int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;

};

#endif //EX3_UNKNOWNFUNCCOMMAND_H
