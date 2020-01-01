//
// Created by nerya on 16/12/2019.
//

#ifndef EX3_CONDITIONPARSER_H
#include "Command.h"
#include "VariablesSingelton.h"
#include "ex1.h"
#define EX3_CONDITIONPARSER_H

class ConditionParser : public Command {
 protected:
  Command *command;

  bool checkCondition(vector<vector<string>> detailsOfTheCommand, unsigned int index);
 public:
  ConditionParser();

  virtual int execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) = 0;
  bool isThisStringIsOperator(string operatorToCondition);
  unsigned int findIndexOfOperator(vector<string> detailsOfCondition);
  string unionAllStringsTillOperator(vector<string> detailsOfCondition, unsigned int indexOfOperator);
  string unionAllStringsFromOperatorTillEnd(vector<string> detailsOfCondition, unsigned int indexOfOperator);

  double interpretStringAndCalculateIt(string nameOfVar);
  int increaseIndexByEnterConditionOrNot(bool didEnterTheLoop,
                                         unsigned int index,
                                         vector<vector<string>> &detailsOfTheCommand);

  virtual ~ConditionParser();

};

#endif //EX3_CONDITIONPARSER_H
