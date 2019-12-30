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

    Var(float value1, const string &sim1);

    float getValue() const;

    const string &getSim() const;

    void setValue(double value1);

    void setFloatValue(float value1);
};


#endif //EX3_VAR_H
