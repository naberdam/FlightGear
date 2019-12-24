//
// Created by nerya on 18/12/2019.
//

#ifndef EX3_STRINGTOCOMMANDS_H
#include "string"
#include "Command.h"
#include "unordered_map"
#include "Command.h"
#include "PrintCommand.h"
#include "SleepCommand.h"
#include "VarDefinitionEqual.h"
#include "WhileCommand.h"
#include "IfCommand.h"
#include "VarDefinitionLeft.h"
#include "VarDefinitionRight.h"
#include "EqualCommandWithoutVar.h"
#include "OpenDataServerCommand.h"
#include "UnknownFuncCommand.h"

#define EX3_STRINGTOCOMMANDS_H

using namespace std;

class StringToCommands {
private:
    unordered_map<string, Command*> mapOfCommands;
    static StringToCommands *instanceOfStringToCommands;

    StringToCommands() {
        this->mapOfCommands["openDataServer"] = new OpenDataServerCommand();
        this->mapOfCommands["Print"] = new PrintCommand();
        this->mapOfCommands["Sleep"] = new SleepCommand();
        this->mapOfCommands["var->"] = new VarDefinitionRight();
        this->mapOfCommands["var<-"] = new VarDefinitionLeft();
        this->mapOfCommands["var="] = new VarDefinitionEqual();
        this->mapOfCommands["while"] = new WhileCommand();
        this->mapOfCommands["if"] = new IfCommand();
        this->mapOfCommands[""] = new UnknownFuncCommand();
    }

public:
    static StringToCommands *getInstanceOfStringToCommands() {
        if (!instanceOfStringToCommands) {
            instanceOfStringToCommands = new StringToCommands();
        }
        return instanceOfStringToCommands;
    }
    Command* getCommandFromMapOfCommandsByString(string nameOfCommand);
    void addCommandEqualWithoutVarToMap(string nameOfVar);
    void addKnownFuncCommandToMap(string nameOfVar, vector<vector<string>> vecExecutes);

};


#endif //EX3_STRINGTOCOMMANDS_H
