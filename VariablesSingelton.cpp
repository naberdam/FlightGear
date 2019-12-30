//
// Created by nerya on 15/12/2019.
//

#include <iostream>
#include "VariablesSingelton.h"

#define DEFAULT_INITIALIZATION_OF_DOUBLE_VALUE_IN_VAR_MAP_RIGHT 0

VariablesSingelton *VariablesSingelton::instance = 0;

mutex mtx;


//i took it from geeksforgeeks
string VariablesSingelton::convertCharArrayToString(char * buffer) {
    unsigned int i;
    string s = "";
    for (i = 0; buffer[i] != '\0'; i++) {
        s += buffer[i];
    }
    return s;
}



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
    auto i = this->mapOfVarRight.begin();
    for (; i != this->mapOfVarRight.end(); i++) {
        if (!i->second->getSim().compare(simAddress)) {
            setNewVariableToInterpreterWithStringString(i->first, to_string(value));
            return;
        }
    }
    auto item = this->mapOfVarLeft.begin();
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
    } catch (char const * exc1) {
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
        mtx.try_lock();
        string simToMsg = this->mapOfVarRight[nameOfVar]->getSim().substr(1);
        mtx.unlock();
        mtx.try_lock();
        this->mapOfVarRight[nameOfVar]->setValue(valueFromString);
        mtx.unlock();
        mtx.try_lock();
        msgForClient.push("set " + simToMsg + " " + convertDoubleValueToStringForSetVariables(valueFromString) + "\r\n");
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

void VariablesSingelton::setNewVariableToInterpreterWithStringDouble(std::__cxx11::string nameOfVar, double valueFromString) {
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
    return str;
}

Var *VariablesSingelton::getVarFromMapOfVarRight(std::__cxx11::string nameOfVar) {
    if (doIHaveThisVarInMapRight(nameOfVar)) {
        mtx.try_lock();
        return this->mapOfVarRight[nameOfVar];
        mtx.unlock();
    }
    return nullptr;
}

Var *VariablesSingelton::getVarFromMapOfVarLeft(std::__cxx11::string nameOfVar) {
    if (doIHaveThisVarInMapLeft(nameOfVar)) {
        mtx.try_lock();
        return this->mapOfVarLeft[nameOfVar];
        mtx.unlock();
    }
    return nullptr;
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
    Var *addressOfVarWeAlreadyHave = this->mapOfSimToValue[simAddress];
    double valueOfVarWeAlreadyHave;
    if (addressOfVarWeAlreadyHave) {
        addToMapRightWithVarPointer(nameOfVar, addressOfVarWeAlreadyHave);
        valueOfVarWeAlreadyHave = (double) addressOfVarWeAlreadyHave->getValue();
        setNewVariableToInterpreterWithStringDouble(nameOfVar, valueOfVarWeAlreadyHave);
    } else if (!doIHaveThisVarInMapRight(nameOfVar)) {
        Var *var = new Var(DEFAULT_INITIALIZATION_OF_DOUBLE_VALUE_IN_VAR_MAP_RIGHT, simAddress);
        this->mapOfVarRight[nameOfVar] = var;
        setNewVariableToInterpreterWithStringDouble(nameOfVar, 0);
    }
}

void VariablesSingelton::addToMapLeftWithSim(std::__cxx11::string nameOfVar, std::__cxx11::string simAddress) {
    Var *addressOfVarWeAlreadyHave = this->mapOfSimToValue[simAddress];
    double valueOfVarWeAlreadyHave;
    if (addressOfVarWeAlreadyHave) {
        addToMapLeftWithVarPointer(nameOfVar, addressOfVarWeAlreadyHave);
        valueOfVarWeAlreadyHave = (double) addressOfVarWeAlreadyHave->getValue();
        setNewVariableToInterpreterWithStringDouble(nameOfVar, valueOfVarWeAlreadyHave);
    } else if (!doIHaveThisVarInMapLeft(nameOfVar)) {
        Var *var = new Var(DEFAULT_INITIALIZATION_OF_DOUBLE_VALUE_IN_VAR_MAP_RIGHT, simAddress);
        mtx.try_lock();
        this->mapOfVarLeft[nameOfVar] = var;
        mtx.unlock();
        setNewVariableToInterpreterWithStringDouble(nameOfVar, 0);
    }
}


unordered_map<string, Var *> VariablesSingelton::getMapOfVarRight() {
    return this->mapOfVarRight;
}

unordered_map<string, Var *> VariablesSingelton::getMapOfVarLeft() {
    return this->mapOfVarLeft;
}

VariablesSingelton::~VariablesSingelton() {
    delete this->interpreter;

}

bool VariablesSingelton::isThisStringIsInInterpreter(std::__cxx11::string nameOfVar) {
    mtx.try_lock();
    bool valueOrString = this->interpreter->isThisStringIsVariableOrJustString(nameOfVar);
    mtx.unlock();
    return valueOrString;
}

double VariablesSingelton::getValueFromSetVariablesOfInterpreter(std::__cxx11::string nameOfVar) {
    mtx.try_lock();
    double stringToDouble = stod(this->interpreter->getValueOfVariable(nameOfVar));
    mtx.unlock();
    return stringToDouble;
}

double VariablesSingelton::calculateStringInInterpret(std::__cxx11::string valueString) {
    valueString = deleteSpacesFromNewSetVariables(valueString);
    Expression* expression;
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

void VariablesSingelton::printValuesInSim() {
    /*for(auto item = this->mapOfSimToValue.begin(); item != this->mapOfSimToValue.end(); ++item) {
        cout << item->second->getSim() << item->second->getValue() << endl;
    }*/
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

