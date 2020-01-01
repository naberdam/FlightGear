//
// Created by nerya on 19/12/2019.
//

#ifndef EX3_CONNECTCLIENTCOMMAND_H
#define EX3_CONNECTCLIENTCOMMAND_H

#include "Command.h"

class ConnectClientCommand : public Command {

 public:
  int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;

};

#endif //EX3_CONNECTCLIENTCOMMAND_H
