//
// Created by nerya on 15/12/2019.
//

#ifndef EX3_COMMAND_H
#include <vector>
#include <string>
#define EX3_COMMAND_H


using namespace std;

class Command {
public:
    virtual int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) = 0;

    virtual ~Command() {}

};


#endif //EX3_COMMAND_H
