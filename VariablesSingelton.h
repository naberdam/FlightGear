//
// Created by nerya on 15/12/2019.
//

#ifndef EX3_VARIABLESSINGELTON_H

#include <thread>
#include <mutex>
#include "ex1.h"
#include "unordered_map"
#include "Var.h"
#include "Command.h"

#define EX3_VARIABLESSINGELTON_H



class VariablesSingelton {
private:
    Interpreter *interpreter;
    unordered_map<string, Var *> mapOfVarRight;
    unordered_map<string, Var *> mapOfVarLeft;
    unordered_map<string, Var * > mapOfSimToValue;
    queue<string> msgForClient;

    bool connectSocket;
    static VariablesSingelton *instance;

    VariablesSingelton() {
        this->interpreter = new Interpreter();
        addToMapSimToValue("/instrumentation/airspeed-indicator/indicated-speed-kt", 0);
        addToMapSimToValue("/sim/time/warp", 0);
        addToMapSimToValue("/controls/switches/magnetos", 0);
        addToMapSimToValue("/instrumentation/heading-indicator/offset-deg", 0);
        addToMapSimToValue("/instrumentation/altimeter/indicated-altitude-ft", 0);
        addToMapSimToValue("/instrumentation/altimeter/pressure-alt-ft", 0);
        addToMapSimToValue("/instrumentation/attitude-indicator/indicated-pitch-deg", 0);
        addToMapSimToValue("/instrumentation/attitude-indicator/indicated-roll-deg", 0);
        addToMapSimToValue("/instrumentation/attitude-indicator/internal-pitch-deg", 0);
        addToMapSimToValue("/instrumentation/attitude-indicator/internal-roll-deg", 0);
        addToMapSimToValue("/instrumentation/encoder/indicated-altitude-ft", 0);
        addToMapSimToValue("/instrumentation/encoder/pressure-alt-ft", 0);
        addToMapSimToValue("/instrumentation/gps/indicated-altitude-ft", 0);
        addToMapSimToValue("/instrumentation/gps/indicated-ground-speed-kt", 0);
        addToMapSimToValue("/instrumentation/gps/indicated-vertical-speed", 0);
        addToMapSimToValue("/instrumentation/heading-indicator/indicated-heading-deg", 0);
        addToMapSimToValue("/instrumentation/magnetic-compass/indicated-heading-deg", 0);
        addToMapSimToValue("/instrumentation/slip-skid-ball/indicated-slip-skid", 0);
        addToMapSimToValue("/instrumentation/turn-indicator/indicated-turn-rate", 0);
        addToMapSimToValue("/instrumentation/vertical-speed-indicator/indicated-speed-fpm", 0);
        addToMapSimToValue("/controls/flight/aileron", 0);
        addToMapSimToValue("/controls/flight/elevator", 0);
        addToMapSimToValue("/controls/flight/rudder", 0);
        addToMapSimToValue("/controls/flight/flaps", 0);
        addToMapSimToValue("/controls/engines/engine/throttle", 0);
        addToMapSimToValue("/controls/switches/master-alt", 0);
        addToMapSimToValue("/controls/engines/current-engine/throttle", 0);
        addToMapSimToValue("/controls/switches/master-avionics", 0);
        addToMapSimToValue("/controls/switches/starter", 0);
        addToMapSimToValue("/engines/active-engine/auto-start", 0);
        addToMapSimToValue("/controls/flight/speedbrake", 0);
        addToMapSimToValue("/sim/model/c172p/brake-parking", 0);
        addToMapSimToValue("/controls/engines/engine/primer", 0);
        addToMapSimToValue("/controls/engines/current-engine/mixture", 0);
        addToMapSimToValue("/controls/switches/master-bat", 0);
        addToMapSimToValue("/engines/engine/rpm", 0);
    }

public:
    static VariablesSingelton *getInstanceOfVariablesSingelton() {
        if (!instance) {
            instance = new VariablesSingelton();
        }
        return instance;
    }

    string convertCharArrayToString(char* buffer);

    bool doIHaveThisVarInMapLeft(string nameOfVar);

    bool doIHaveThisVarInMapRight(string nameOfVar);

    void setVariableInInterpreterWhenReceiveFromServer(string simAddress, float value);

    /*void updateValueWithXmlFile(char *buffer, vector<string> vectorOfNameValueFromXml);*/

    void setMapLeftOfVarByValue(string nameOfVar, string value);

    void setMapRightOfVarByValue(string nameOfVar, string value);

    string convertDoubleValueToStringForSetVariables(double valueFromCalculateExpression);

    string deleteSpacesFromNewSetVariables(string text);

    void setNewVariableToInterpreterWithStringDouble(string nameOfVar, double valueFromString);

    void setNewVariableToInterpreterWithStringString(string nameOfVar, string valueString);

    void addVariableToInterpreter(string nameOfVar, string value);

    void addToMapRightWithSim(string nameOfVar, string simAddress);

    void addToMapLeftWithSim(string nameOfVar, string simAddress);

    void addToMapLeftWithVarPointer(string nameOfVar, Var *&var);

    void addToMapRightWithVarPointer(string nameOfVar, Var *&var);

    Var *getVarFromMapOfVarRight(string nameOfVar);

    Var *getVarFromMapOfVarLeft(string nameOfVar);

    unordered_map<string, Var *> getMapOfVarRight();

    unordered_map<string, Var *> getMapOfVarLeft();

    bool isThisStringIsInInterpreter(string nameOfVar);

    double getValueFromSetVariablesOfInterpreter(string nameOfVar);

    double calculateStringInInterpret(string valueString);

    void addToMapSimToValue(string sim, float value);

    void updateValueInMapSim(string sim, float value);
    void connectMe();
    void disconnectMe();

    bool isConnectSocket() const;

    void printValuesInSim();
    string getMsgFromQueue();
    bool queueWithMsg();
    int locationOfE(string number);
    string changeStringToBeWithoutE(string number, int indexE);


    virtual ~VariablesSingelton();


};


#endif //EX3_VARIABLESSINGELTON_H
