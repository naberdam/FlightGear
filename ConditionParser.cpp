//
// Created by nerya on 16/12/2019.
//

#include <iostream>
#include "ConditionParser.h"



bool ConditionParser::checkCondition(vector<vector<std::__cxx11::string> > detailsOfTheCommand, unsigned int index) {
    unsigned int indexOfOperator = findIndexOfOperator(detailsOfTheCommand[index]);
    if (indexOfOperator == -1) {
        return false;
    }
    string operatorBetweenSidesOfCondition = detailsOfTheCommand[index][indexOfOperator];
    string leftSideOfCondition = unionAllStringsTillOperator(detailsOfTheCommand[index], indexOfOperator);
    string rightSideOfCondition = unionAllStringsFromOperatorTillEnd(detailsOfTheCommand[index], indexOfOperator);
    double numberOfLeftSideOfCondition, numberOfRightSideOfCondition;
    numberOfLeftSideOfCondition = interpretStringAndCalculateIt(leftSideOfCondition);
    numberOfRightSideOfCondition = interpretStringAndCalculateIt(rightSideOfCondition);
    if (operatorBetweenSidesOfCondition == "==") {
        return numberOfLeftSideOfCondition == numberOfRightSideOfCondition;
    }
    if (operatorBetweenSidesOfCondition == "<=") {
        return numberOfLeftSideOfCondition <= numberOfRightSideOfCondition;
    }
    if (operatorBetweenSidesOfCondition == ">=") {
        return numberOfLeftSideOfCondition >= numberOfRightSideOfCondition;
    }
    if (operatorBetweenSidesOfCondition == ">") {
        return numberOfLeftSideOfCondition > numberOfRightSideOfCondition;
    }
    if (operatorBetweenSidesOfCondition == "<") {
        return numberOfLeftSideOfCondition < numberOfRightSideOfCondition;
    }
    if (operatorBetweenSidesOfCondition == "!=") {
        return numberOfLeftSideOfCondition != numberOfRightSideOfCondition;
    }

}

string ConditionParser::deleteOpenParanthesis(std::__cxx11::string text) {
    int countOpen = 0;
    int countClose = 0;
    unsigned int i = 0;
    for (; i < text.size(); ++i) {
        if (text[i] == '(') {
            ++countOpen;
        } else if (text [i] == ')') {
            ++countClose;
        }
    }
    if (countClose == countOpen) {
        return text;
    } else if (countOpen < countClose) {
        return text.erase(text.size() - 2, text.size() - 1);
    } else {
        return text.erase(0, 1);
    }

}

double ConditionParser::interpretStringAndCalculateIt(std::__cxx11::string nameOfVar) {
    VariablesSingelton* variablesSingelton = variablesSingelton->getInstanceOfVariablesSingelton();
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
    if (i == detailsOfCondition.size()) {
        return -1;
    }
    return i;
}

string ConditionParser::unionAllStringsTillOperator(vector<std::__cxx11::string> detailsOfCondition, unsigned int indexOfOperator) {
    unsigned int i = 1;
    unsigned int j = 0;
    string newLeftCondition, withoutSpaces;
    for (; i != indexOfOperator ; ++i) {
        for (; j < detailsOfCondition[i].size(); ++j) {
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

string ConditionParser::unionAllStringsFromOperatorTillEnd(vector<std::__cxx11::string> detailsOfCondition, unsigned int indexOfOperator) {
    unsigned int i = indexOfOperator + 1;
    unsigned int j = 0;
    string newRightCondition, withoutSpaces;
    for (; i < detailsOfCondition.size() ; ++i) {
        for (; j < detailsOfCondition[i].size(); ++j) {
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
