//
// Created by nerya on 15/12/2019.
//

#include <iostream>
#include "PrintCommand.h"
#include "VariablesSingelton.h"
extern mutex mtx;
int PrintCommand::execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) {
    unsigned int i = 1;
    for (; i < detailsOfTheCommand[index].size(); ++i) {
        VariablesSingelton *variablesSingelton = variablesSingelton->getInstanceOfVariablesSingelton();
        if ((variablesSingelton->doIHaveThisVarInMapLeft(detailsOfTheCommand[index][i]))
            || (variablesSingelton->doIHaveThisVarInMapRight(detailsOfTheCommand[index][i]))) {
            mtx.try_lock();
            //need to check where to print
            cout << variablesSingelton->getValueFromSetVariablesOfInterpreter(detailsOfTheCommand[index][1]) << endl;
            mtx.unlock();
        } else {
            mtx.try_lock();
            cout << detailsOfTheCommand[index][i] << endl;
            mtx.unlock();
        }
    }
    return ++index;
}