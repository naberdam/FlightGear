//
// Created by nerya on 15/12/2019.
//

#include <iostream>
#include "VariablesSingelton.h"

#define DEFAULT_INITIALIZATION_OF_DOUBLE_VALUE_IN_VAR_MAP_RIGHT 0

VariablesSingelton *VariablesSingelton::instance = 0;

mutex mtx;

bool VariablesSingelton::doIHaveThisVarInMapLeft(std::__cxx11::string nameOfVar) {
  if (this->mapOfVarLeft.count(nameOfVar)) {
    return true;
  }
  return false;
}

bool VariablesSingelton::doIHaveThisVarInMapRight(std::__cxx11::string nameOfVar) {
  if (this->mapOfVarRight.count(nameOfVar)) {
    return true;
  }
  return false;
}

void VariablesSingelton::setVariableInInterpreterWhenReceiveFromServer(std::__cxx11::string simAddress, float value) {
  //we need to update the variable so we need do look for where is this variable - mapRight or mapLeft
  auto i = this->mapOfVarRight.begin();
  //check in mapRight
  for (; i != this->mapOfVarRight.end(); i++) {
    if (!i->second->getSim().compare(simAddress)) {
      setNewVariableToInterpreterWithStringString(i->first, to_string(value));
      return;
    }
  }
  auto item = this->mapOfVarLeft.begin();
  //check in mapLeft
  for (; item != this->mapOfVarLeft.end(); item++) {
    if (!item->second->getSim().compare(simAddress)) {
      setNewVariableToInterpreterWithStringString(item->first, to_string(value));
      return;
    }
  }
}

void VariablesSingelton::addVariableToInterpreter(std::__cxx11::string nameOfVar, std::__cxx11::string value) {
  Expression *expression;
  double valueFromString;
  try {
    mtx.try_lock();
    expression = this->interpreter->interpret(value);
    mtx.unlock();
    valueFromString = expression->calculate();
    //set variables for next time we use interpreter
    setNewVariableToInterpreterWithStringDouble(nameOfVar, valueFromString);
    if (expression) {
      delete expression;
    }
  } catch (char const *exc1) {
    cout << "error with addVariableToInterpreter in VariablesSingelton:" << exc1 << endl;
    if (expression) {
      delete expression;
    }
    exit(1);
  }
}

void VariablesSingelton::setMapLeftOfVarByValue(std::__cxx11::string nameOfVar, string value) {
  Expression *expression;
  double valueFromString;
  mtx.try_lock();
  expression = this->interpreter->interpret(value);
  mtx.unlock();
  valueFromString = expression->calculate();
  if (doIHaveThisVarInMapLeft(nameOfVar)) {
    mtx.try_lock();
    this->mapOfVarLeft[nameOfVar]->setValue(valueFromString);
    mtx.unlock();
  }
  //set variables for next time we use interpreter
  setNewVariableToInterpreterWithStringDouble(nameOfVar, valueFromString);
  if (expression) {
    delete expression;
  }
}

void VariablesSingelton::setMapRightOfVarByValue(std::__cxx11::string nameOfVar, string value) {
  Expression *expression;
  double valueFromString;
  mtx.try_lock();
  expression = this->interpreter->interpret(value);
  mtx.unlock();
  valueFromString = expression->calculate();
  if (doIHaveThisVarInMapRight(nameOfVar)) {
    //add the sim to message
    mtx.try_lock();
    string simToMsg = this->mapOfVarRight[nameOfVar]->getSim().substr(1);
    mtx.unlock();
    mtx.try_lock();
    this->mapOfVarRight[nameOfVar]->setValue(valueFromString);
    mtx.unlock();
    //add message to queue for client
    mtx.try_lock();
    msgForClient.push(
        "set " + simToMsg + " " + convertDoubleValueToStringForSetVariables(valueFromString) + "\r\n");
    mtx.unlock();
  }
  //set variables for next time we use interpreter
  setNewVariableToInterpreterWithStringDouble(nameOfVar, valueFromString);
  if (expression) {
    delete expression;
  }
}

string VariablesSingelton::getMsgFromQueue() {
  string msg;
  if (!this->msgForClient.empty()) {
    mtx.try_lock();
    msg = this->msgForClient.front();
    mtx.unlock();
    mtx.try_lock();
    this->msgForClient.pop();
    mtx.unlock();
    return msg;
  }
  return "";
}

bool VariablesSingelton::queueWithMsg() {
  return !this->msgForClient.empty();
}

void VariablesSingelton::setNewVariableToInterpreterWithStringDouble(std::__cxx11::string nameOfVar,
                                                                     double valueFromString) {
  //add '=' to nameOfVar so we can send it to function setVariables from Interpreter
  nameOfVar += "=";
  //setVariables only get strings and not double etc.
  nameOfVar += convertDoubleValueToStringForSetVariables(valueFromString);
  //setVariables with deleting spaces for sure, although we handle spaces in our lexer
  mtx.try_lock();
  this->interpreter->setVariables(deleteSpacesFromNewSetVariables(nameOfVar));
  mtx.unlock();
}

void VariablesSingelton::setNewVariableToInterpreterWithStringString(string nameOfVar, string valueString) {
  //add '=' to nameOfVar so we can send it to function setVariables from Interpreter
  nameOfVar += "=";
  nameOfVar += valueString;
  //setVariables with deleting spaces for sure, although we handle spaces in our lexer
  mtx.try_lock();
  this->interpreter->setVariables(deleteSpacesFromNewSetVariables(nameOfVar));
  mtx.unlock();
}

string VariablesSingelton::deleteSpacesFromNewSetVariables(std::__cxx11::string text) {
  unsigned int i = 0;
  string stringWithoutSpaces;
  for (; i < text.size(); ++i) {
    if (text[i] == ' ') {
      continue;
    }
    stringWithoutSpaces += text[i];
  }
  return stringWithoutSpaces;
}

//i took this algorithm from stackoverflow
string VariablesSingelton::convertDoubleValueToStringForSetVariables(double valueFromCalculateExpression) {
  ostringstream strs;
  strs << valueFromCalculateExpression;
  string str = strs.str();
  //check if there is 'e' in number
  int indexE = locationOfE(str);
  if (indexE != -1) {
    //we dont want 'e' to be in number so we change it with the value that hide behind of 'e'
    str = changeStringToBeWithoutE(str, indexE);
  }
  return str;
}

string VariablesSingelton::changeStringToBeWithoutE(string number, int indexE) {
  string valueOfNumber = number.substr(0, indexE);
  string e = number.substr(indexE + 1);
  //initialize the beginning of our new number that supposed to switch 'e'
  string eAfterChange = "0.";
  int exponentOfE = stoi(e);
  //add '0' to the beginning of our new number that supposed to switch 'e'
  while (exponentOfE != -1) {
    eAfterChange += '0';
    ++exponentOfE;
  }
  string valueAfterChange;
  unsigned int j = 0;
  //we want to keep a boolean that will tell us if it is a negative number or not
  bool negativeNumber = false;
  //initialize the number that comes after all the zero
  for (; j < valueOfNumber.size(); ++j) {
    if (!negativeNumber && valueOfNumber[j] == '-') {
      negativeNumber = true;
      continue;
    }
    //we dont want '.' in our value
    if (valueOfNumber[j] == '.') {
      continue;
    }
    valueAfterChange += valueOfNumber[j];
  }
  if (negativeNumber) {
    string neg = "-";
    neg += eAfterChange;
    eAfterChange = neg;
  }
  eAfterChange += valueOfNumber;
  return eAfterChange;
}

int VariablesSingelton::locationOfE(string number) {
  unsigned int i = 0;
  for (; i < number.size(); ++i) {
    //there is 'e' in number
    if (number[i] == 'e') {
      return i;
    }
  }
  //there is no 'e' in number
  return -1;
}

void VariablesSingelton::addToMapLeftWithVarPointer(std::__cxx11::string nameOfVar, class Var *&var) {
  if (!doIHaveThisVarInMapLeft(nameOfVar)) {
    mtx.try_lock();
    this->mapOfVarLeft[nameOfVar] = var;
    mtx.unlock();
  }
}

void VariablesSingelton::addToMapRightWithVarPointer(std::__cxx11::string nameOfVar, class Var *&var) {
  if (!doIHaveThisVarInMapRight(nameOfVar)) {
    mtx.try_lock();
    this->mapOfVarRight[nameOfVar] = var;
    mtx.unlock();
  }
}

void VariablesSingelton::addToMapRightWithSim(std::__cxx11::string nameOfVar, std::__cxx11::string simAddress) {
  //find address of the var we want to add to map right
  Var *addressOfVarWeAlreadyHave = this->mapOfSimToValue[simAddress];
  double valueOfVarWeAlreadyHave;
  if (addressOfVarWeAlreadyHave) {
    addToMapRightWithVarPointer(nameOfVar, addressOfVarWeAlreadyHave);
    valueOfVarWeAlreadyHave = (double) addressOfVarWeAlreadyHave->getValue();
    //update the variable in interpreter
    setNewVariableToInterpreterWithStringDouble(nameOfVar, valueOfVarWeAlreadyHave);
  } else if (!doIHaveThisVarInMapRight(nameOfVar)) {
    //default value=0
    Var *var = new Var(DEFAULT_INITIALIZATION_OF_DOUBLE_VALUE_IN_VAR_MAP_RIGHT, simAddress);
    mtx.try_lock();
    this->mapOfVarRight[nameOfVar] = var;
    mtx.unlock();
    //update the variable in interpreter
    setNewVariableToInterpreterWithStringDouble(nameOfVar, 0);
  }
}

void VariablesSingelton::addToMapLeftWithSim(std::__cxx11::string nameOfVar, std::__cxx11::string simAddress) {
  //find address of the var we want to add to map left
  Var *addressOfVarWeAlreadyHave = this->mapOfSimToValue[simAddress];
  double valueOfVarWeAlreadyHave;
  if (addressOfVarWeAlreadyHave) {
    addToMapLeftWithVarPointer(nameOfVar, addressOfVarWeAlreadyHave);
    valueOfVarWeAlreadyHave = (double) addressOfVarWeAlreadyHave->getValue();
    //update the variable in interpreter
    setNewVariableToInterpreterWithStringDouble(nameOfVar, valueOfVarWeAlreadyHave);
  } else if (!doIHaveThisVarInMapLeft(nameOfVar)) {
    //default value=0
    Var *var = new Var(DEFAULT_INITIALIZATION_OF_DOUBLE_VALUE_IN_VAR_MAP_RIGHT, simAddress);
    mtx.try_lock();
    this->mapOfVarLeft[nameOfVar] = var;
    mtx.unlock();
    //update the variable in interpreter
    setNewVariableToInterpreterWithStringDouble(nameOfVar, 0);
  }
}

VariablesSingelton::~VariablesSingelton() {
  delete this->interpreter;
  auto item = this->mapOfSimToValue.begin();
  //check in mapRight
  for (; item != this->mapOfSimToValue.end(); ++item) {
    delete item->second;
  }
}

double VariablesSingelton::getValueFromSetVariablesOfInterpreter(std::__cxx11::string nameOfVar) {
  mtx.try_lock();
  double stringToDouble = stod(this->interpreter->getValueOfVariable(nameOfVar));
  mtx.unlock();
  return stringToDouble;
}

double VariablesSingelton::calculateStringInInterpret(std::__cxx11::string valueString) {
  //the interpret function cant handle spaces, so we want to delete them
  valueString = deleteSpacesFromNewSetVariables(valueString);
  Expression *expression;
  double numberOfString;
  mtx.try_lock();
  expression = this->interpreter->interpret(valueString);
  mtx.unlock();
  numberOfString = expression->calculate();
  delete expression;
  return numberOfString;

}

void VariablesSingelton::addToMapSimToValue(std::__cxx11::string sim, float value) {
  mtx.try_lock();
  /*Var * var = new Var(value, sim);*/
  this->mapOfSimToValue[sim] = new Var(value, sim);
  mtx.unlock();
}

void VariablesSingelton::updateValueInMapSim(string sim, float value) {
  mtx.try_lock();
  this->mapOfSimToValue[sim]->setFloatValue(value);
  mtx.unlock();
  setVariableInInterpreterWhenReceiveFromServer(sim, value);
}

bool VariablesSingelton::isConnectSocket() const {
  return connectSocket;
}

void VariablesSingelton::disconnectMe() {
  mtx.try_lock();
  this->connectSocket = false;
  mtx.unlock();
}

void VariablesSingelton::connectMe() {
  mtx.try_lock();
  this->connectSocket = true;
  mtx.unlock();
}

