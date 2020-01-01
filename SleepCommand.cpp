//
// Created by nerya on 15/12/2019.
//

#include <chrono>
#include <thread>
#include "SleepCommand.h"
#include "VariablesSingelton.h"

extern mutex mtx;

int SleepCommand::execute(vector<vector<string>> &detailsOfTheCommand, unsigned int index) {
  VariablesSingelton *variablesSingelton = variablesSingelton->getInstanceOfVariablesSingelton();
  //if we have this variable in one of our two maps so we need just to take it from setVariables of interpreter
  if ((variablesSingelton->doIHaveThisVarInMapLeft(detailsOfTheCommand[index][1]))
      || (variablesSingelton->doIHaveThisVarInMapRight(detailsOfTheCommand[index][1]))) {
    //need to check where to print
    mtx.try_lock();
    this_thread::sleep_for(chrono::milliseconds((int) variablesSingelton->getValueFromSetVariablesOfInterpreter(
        detailsOfTheCommand[index][1])));
    mtx.unlock();
  } else {
    mtx.try_lock();
    this_thread::sleep_for(chrono::milliseconds((int) variablesSingelton->calculateStringInInterpret(detailsOfTheCommand[index][1])));
    mtx.unlock();
  }
  return ++index;
}