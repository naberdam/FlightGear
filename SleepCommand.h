//
// Created by nerya on 15/12/2019.
//

#ifndef EX3_SLEEPCOMMAND_H
#define EX3_SLEEPCOMMAND_H

#include "Command.h"

class SleepCommand : public Command {

 public:
  int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;
};

#endif //EX3_SLEEPCOMMAND_H
