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

void Var::setFloatValue(float value1) {
  this->value = value1;
}

//took it from stackoverflow
void Var::setValue(double value1) {
  char buf[42];
  sprintf(buf, "%.7g", value1); // round to 7 decimal digits
  Var::value = atof(buf);
}

Var::Var(float value1, const string &sim1) : value(value1), sim(sim1) {}
