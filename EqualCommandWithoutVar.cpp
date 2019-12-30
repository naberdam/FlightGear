//
// Created by nerya on 17/12/2019.
//

#include <iostream>
#include "EqualCommandWithoutVar.h"
#include "VariablesSingelton.h"

int EqualCommandWithoutVar::execute(vector<vector<std::__cxx11::string> > &detailsOfTheCommand, unsigned int index) {
    VariablesSingelton* variablesSingelton = variablesSingelton->getInstanceOfVariablesSingelton();
    string nameOfVarWeAlreadyHave = detailsOfTheCommand[index][0];
    string valueStringOfVar = detailsOfTheCommand[index][2];
    variablesSingelton->addVariableToInterpreter(nameOfVarWeAlreadyHave, valueStringOfVar);
    if (variablesSingelton->doIHaveThisVarInMapLeft(nameOfVarWeAlreadyHave)) {
        variablesSingelton->setMapLeftOfVarByValue(nameOfVarWeAlreadyHave, valueStringOfVar);
    } else if (variablesSingelton->doIHaveThisVarInMapRight(nameOfVarWeAlreadyHave)) {
        variablesSingelton->setMapRightOfVarByValue(nameOfVarWeAlreadyHave, valueStringOfVar);
    }
    return ++index;
}