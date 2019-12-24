//
// Created by nerya on 15/12/2019.
//

#include "Var.h"


float Var::getValue() const {
    return value;
}

const string &Var::getSim() const {
    return sim;
}

void Var::setFloatValue(float value) {
    this->value = value;
}


void Var::setValue(double value) {
    char buf[42];
    sprintf(buf, "%.7g", value); // round to 7 decimal digits
    Var::value = atof(buf);
}


Var::Var(float value, const string &sim) : value(value), sim(sim) {}
