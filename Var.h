//
// Created by nerya on 15/12/2019.
//

#ifndef EX3_VAR_H
#define EX3_VAR_H

#include "string"

using namespace std;
class Var {
private:
    float value;
    string sim;
public:

    Var(float value, const string &sim);

    float getValue() const;

    const string &getSim() const;

    void setValue(double value);

    void setFloatValue(float value);
};


#endif //EX3_VAR_H
