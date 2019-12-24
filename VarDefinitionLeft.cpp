//
// Created by nerya on 15/12/2019.
//

#include "VarDefinitionLeft.h"
#include "VariablesSingelton.h"
#include "StringToCommands.h"

int VarDefinitionLeft::execute(vector<vector<std::__cxx11::string> > &detailsOfTheCommand, unsigned int index) {
    VariablesSingelton* variablesSingelton = variablesSingelton->getInstanceOfVariablesSingelton();
    string nameOfVar = detailsOfTheCommand[index][1];
    string simAddress = detailsOfTheCommand[index][3];
    variablesSingelton->addToMapLeftWithSim(nameOfVar, simAddress);
    StringToCommands* stringToCommands = stringToCommands->getInstanceOfStringToCommands();
    stringToCommands->addCommandEqualWithoutVarToMap(nameOfVar);
    return ++index;
}