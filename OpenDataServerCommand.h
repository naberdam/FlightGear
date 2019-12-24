//
// Created by nerya on 18/12/2019.
//

#ifndef EX3_OPENDATASERVERCOMMAND_H

#include "unordered_map"
#include "Command.h"
#include "Var.h"

#define EX3_OPENDATASERVERCOMMAND_H

using namespace std;

class OpenDataServerCommand : public Command{
private:
    vector<string> vectorOfNameValue;
public:
    OpenDataServerCommand();

    int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) override;

};


#endif //EX3_OPENDATASERVERCOMMAND_H
