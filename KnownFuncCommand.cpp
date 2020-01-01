

//
// Created by nerya on 20/12/2019.
//

#include <iostream>
#include "KnownFuncCommand.h"
#include "StringToCommands.h"

//this vector contains the commands of the function we created
KnownFuncCommand::KnownFuncCommand(const vector<vector<string>> &detailsOfFuncDeclarationFromUnknownFunc)
    : detailsOfFuncDeclaration(detailsOfFuncDeclarationFromUnknownFunc) {}

int KnownFuncCommand::execute(vector<vector<string> > &detailsOfTheCommand, unsigned int index) {
  auto item = detailsOfTheCommand.begin();
  //advance item to be one cell after the calling function so we will be able to insert the commands of the function
  //that we have in detailsOfFuncDeclarationFromUnknownFunc
  advance(item, ++index);
  //insert the commands of the function after the cell of calling function and before the next commands we already
  //have in detailsOfTheCommand
  detailsOfTheCommand.insert(item, this->detailsOfFuncDeclaration.begin(),
                             this->detailsOfFuncDeclaration.end());
  //enter the value of var x
  detailsOfTheCommand[index].push_back(detailsOfTheCommand[index - 1][1]);
  return index;
}
