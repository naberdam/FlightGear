//
// Created by nerya on 20/12/2019.
//

#include "UnknownFuncCommand.h"
#include "StringToCommands.h"

int UnknownFuncCommand::execute(vector<vector<string> > &detailsOfTheCommand, unsigned int index) {
  vector<vector<string> > detailsOfFuncDeclaration;
  //nameOfFunc will be in the map of string Commands in StringToCommands for the declaration of func
  string nameOfFunc = detailsOfTheCommand[index][0];
  string varCommand = detailsOfTheCommand[index][1];
  string expression = detailsOfTheCommand[index][2];
  //add '=' to var so when we will do execute to our vectors, we will know to go VarDefinitionEqual
  varCommand += '=';
  //resize detailsOfFuncDeclaration with a random number
  detailsOfFuncDeclaration.resize(20);
  detailsOfFuncDeclaration[0].push_back(varCommand);
  detailsOfFuncDeclaration[0].push_back(expression);
  unsigned int i = index + 2;
  unsigned int j = 1;
  unsigned k;
  int countSpecialParenthesis = 1;
  while (countSpecialParenthesis) {
    if (detailsOfTheCommand[i][0] == "{") {
      countSpecialParenthesis++;
      //end of specialParenthesis
    } else if (detailsOfTheCommand[i][0] == "}" && countSpecialParenthesis == 1) {
      i++;
      break;
    } else if (detailsOfTheCommand[i][0] == "}") {
      countSpecialParenthesis--;
    }
    for (k = 0; k < detailsOfTheCommand[i].size(); ++k) {
      detailsOfFuncDeclaration[j].push_back(detailsOfTheCommand[i][k]);
    }
    j++;
    i++;
  }
  //update the size of detailsOfFuncDeclaration in real size
  detailsOfFuncDeclaration.resize(j);
  //add nameOfFunc to map in stringToCommands
  StringToCommands *stringToCommands = stringToCommands->getInstanceOfStringToCommands();
  stringToCommands->addKnownFuncCommandToMap(nameOfFunc, detailsOfFuncDeclaration);
  return i;

}