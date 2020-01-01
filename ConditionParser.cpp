//
// Created by nerya on 16/12/2019.
//

#include <iostream>
#include "ConditionParser.h"

bool ConditionParser::checkCondition(vector<vector<std::__cxx11::string> > detailsOfTheCommand, unsigned int index) {
  //find index of operator
  unsigned int indexOfOperator = findIndexOfOperator(detailsOfTheCommand[index]);
  //operator
  string operatorBetweenSidesOfCondition = detailsOfTheCommand[index][indexOfOperator];
  string leftSideOfCondition = unionAllStringsTillOperator(detailsOfTheCommand[index], indexOfOperator);
  string rightSideOfCondition = unionAllStringsFromOperatorTillEnd(detailsOfTheCommand[index], indexOfOperator);
  double numberOfLeftSideOfCondition, numberOfRightSideOfCondition;
  //interpret both conditions
  numberOfLeftSideOfCondition = interpretStringAndCalculateIt(leftSideOfCondition);
  numberOfRightSideOfCondition = interpretStringAndCalculateIt(rightSideOfCondition);
  leftSideOfCondition = to_string(numberOfLeftSideOfCondition);
  rightSideOfCondition = to_string(numberOfRightSideOfCondition);
  if (operatorBetweenSidesOfCondition == "==") {
    //the interpreter returns accurate value in double type so sometimes it returns for 0.2
    //0.2000000001 and sometime 0.200000006 and they are not equal, so that's why we need to use
    //the function to_string, because the "to_string" of both of them will be "0.2000000"
    return to_string(numberOfLeftSideOfCondition) == to_string(numberOfRightSideOfCondition)
        || (numberOfLeftSideOfCondition == numberOfRightSideOfCondition);
  }
  if (operatorBetweenSidesOfCondition == "<=") {
    return (numberOfLeftSideOfCondition <= numberOfRightSideOfCondition);
  }
  if (operatorBetweenSidesOfCondition == ">=") {
    return to_string(numberOfLeftSideOfCondition) >= to_string(numberOfRightSideOfCondition)
        || (numberOfLeftSideOfCondition >= numberOfRightSideOfCondition);
  }
  if (operatorBetweenSidesOfCondition == ">") {
    return (numberOfLeftSideOfCondition > numberOfRightSideOfCondition);
  }
  if (operatorBetweenSidesOfCondition == "<") {
    return (numberOfLeftSideOfCondition < numberOfRightSideOfCondition);
  }
  //operatorBetweenSidesOfCondition == "!="
  return to_string(numberOfLeftSideOfCondition) != to_string(numberOfRightSideOfCondition)
      || (numberOfLeftSideOfCondition != numberOfRightSideOfCondition);
}

int ConditionParser::increaseIndexByEnterConditionOrNot(bool didEnterTheLoop, unsigned int index,
                                                        vector<vector<string>> &detailsOfTheCommand) {
  //did we enter the loop or if or not
  if (didEnterTheLoop) {
    //we want to pass the vector of "}"
    if (detailsOfTheCommand[index + 1][0] == "}") {
      return index + 2;
    }
    return index + 1;
  } else {
    //so we need to pass all vectors of while/if
    unsigned int i = index + 2;
    unsigned int j = 1;
    int countSpecialParenthesis = 1;
    while (countSpecialParenthesis) {
      if (detailsOfTheCommand[i][0] == "{") {
        countSpecialParenthesis++;
      } else if (detailsOfTheCommand[i][0] == "}" && countSpecialParenthesis == 1) {
        i++;
        break;
      } else if (detailsOfTheCommand[i][0] == "}") {
        countSpecialParenthesis--;
      }
      j++;
      i++;
    }
    return i;
  }
}

double ConditionParser::interpretStringAndCalculateIt(std::__cxx11::string nameOfVar) {
  VariablesSingelton *variablesSingelton = variablesSingelton->getInstanceOfVariablesSingelton();
  return variablesSingelton->calculateStringInInterpret(nameOfVar);
}

bool ConditionParser::isThisStringIsOperator(std::__cxx11::string operatorToCondition) {
  return operatorToCondition == "=="
      || operatorToCondition == "<="
      || operatorToCondition == ">="
      || operatorToCondition == "<"
      || operatorToCondition == ">"
      || operatorToCondition == "!=";
}

unsigned int ConditionParser::findIndexOfOperator(vector<std::__cxx11::string> detailsOfCondition) {
  unsigned int i = 1;
  while (!isThisStringIsOperator(detailsOfCondition[i]) || i == detailsOfCondition.size()) {
    i++;
  }
  //in case we didn't find the index operator
  if (i == detailsOfCondition.size()) {
    return -1;
  }
  return i;
}

string ConditionParser::unionAllStringsTillOperator(vector<std::__cxx11::string> detailsOfCondition,
                                                    unsigned int indexOfOperator) {
  //this function takes all strings from the beginning till operator and union them to one string without spaces
  unsigned int i = 1;
  unsigned int j = 0;
  string newLeftCondition, withoutSpaces;
  for (; i != indexOfOperator; ++i) {
    for (; j < detailsOfCondition[i].size(); ++j) {
      //cancel spaces from strings
      if (detailsOfCondition[i][j] == ' ') {
        continue;
      }
      withoutSpaces += detailsOfCondition[i][j];
    }
    newLeftCondition += withoutSpaces;
    withoutSpaces.clear();
  }
  return newLeftCondition;
}

string ConditionParser::unionAllStringsFromOperatorTillEnd(vector<std::__cxx11::string> detailsOfCondition,
                                                           unsigned int indexOfOperator) {
  //this function takes all strings from operator till the end and union them to one string without spaces
  unsigned int i = indexOfOperator + 1;
  unsigned int j = 0;
  string newRightCondition, withoutSpaces;
  for (; i < detailsOfCondition.size(); ++i) {
    for (; j < detailsOfCondition[i].size(); ++j) {
      //cancel spaces from strings
      if (detailsOfCondition[i][j] == ' ') {
        continue;
      }
      withoutSpaces += detailsOfCondition[i][j];
    }
    newRightCondition += detailsOfCondition[i];
    withoutSpaces.clear();
  }
  return newRightCondition;
}

ConditionParser::ConditionParser() {}

ConditionParser::~ConditionParser() {}
