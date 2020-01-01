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
      string printString = variablesSingelton->deleteSpacesFromNewSetVariables(detailsOfTheCommand[index][1]);
      try {
        //maybe it is an expression and not string
        mtx.try_lock();
        //need to check where to print
        cout << variablesSingelton->getValueFromSetVariablesOfInterpreter(printString) << endl;
        mtx.unlock();
      } catch (char const *exp1) {
        //it is not a number or any expression, it is a string
        mtx.try_lock();
        cout << detailsOfTheCommand[index][i] << endl;
        mtx.unlock();
      }

    }
  }
  return ++index;
}