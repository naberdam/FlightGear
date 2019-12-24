//
// Created by nerya on 05/11/2019.
//

#ifndef UNTITLED1_EX1_H
#define UNTITLED1_EX1_H

#include <string>
#include "Expression.h"
#include <stack>
#include <queue>
#include <regex>
#include <map>

using namespace std;

class Value : public Expression {
private:
    double val;

public:

    Value(double val1) : val(val1) {}

    double calculate() override {
        return val;
    }

    Value(Value &val1) : val(val1.calculate()) {}
};

class Variable : public Expression {
private:
    string name;
    double value;
public:
    Variable(string name1, double value1);

    const string &getName() const;

    double getValue();

    void setName(string &name);

    void setValue(double value);

    Variable &operator++();

    Variable &operator++(int);

    Variable &operator--();

    Variable operator--(int);

    Variable &operator+=(double val);

    Variable &operator-=(double val);

    double calculate() override;

};

class UnaryOperator : public Expression {
protected:
    Expression *expression;

public:
    UnaryOperator(Expression *expression1) {
        this->expression = expression1;
    }

    virtual ~UnaryOperator() {
        delete expression;
    }

    Expression *getExpression() {
        return expression;
    }

    void setExpression(Expression *expression1) {
        UnaryOperator::expression = expression1;
    }
};

class BinaryOperator : public Expression {
protected:
    Expression *left;
    Expression *right;
public:
    BinaryOperator(Expression *left, Expression *right);

    virtual ~BinaryOperator();

    Expression *getLeft();

    void setLeft(Expression *left);

    Expression *getRight();

    void setRight(Expression *right);
};

class Plus : public BinaryOperator {
public:
    Plus(Expression *left, Expression *right);

    double calculate() override;
};

class Minus : public BinaryOperator {
public:
    Minus(Expression *left, Expression *right);

    double calculate() override;
};

class Mul : public BinaryOperator {
public:
    Mul(Expression *left, Expression *right);

    double calculate() override;
};

class Div : public BinaryOperator {
public:
    Div(Expression *left, Expression *right);

    double calculate() override;
};

class UPlus : public UnaryOperator {
public:
    UPlus(Expression *expression1);

    double calculate() override;

};

class UMinus : public UnaryOperator {
public:
    UMinus(Expression *expression1);

    double calculate() override;
};

class Interpreter {
protected:
    map <string, string> valuesOfVariables;


public:

    Expression *interpret(string exp);

    bool isNumberChar(char character);
    bool isNumberString(string number);
    bool isThisStringIsVariableOrJustString(string nameOfVar);
    string getValueOfVariable(string nameOfVar);
    string replaceAll(string* str, const string& from, const string& to);

    bool isOperator(char character);
    string updateString(string exp, int i, int j, double loadNumber);
    bool checkProperString(string stringCheck);
    bool checkProperParenthesis(string stringCheck);
    bool checkProperStack(stack<Expression*> stackCheck);
    void orderMapByValuesAndVariables(string tokenOfAllText);

    bool priorityOfOperators(char outStack, string inStack);

    bool isOperator(string queueFront);
    void setVariables(string variablesValue);
    bool isLetter(string token);
    bool checkProperEquation(string charOfExpCheck, unsigned int i);
    bool checkProperReplaceVariables(string stringCheck);
    bool isLetterChar(char stringCheck);
    Expression* binaryCalculate(Expression *firstExpression, Expression *secondExpression,
                                string operatorCalculation);

};


#endif //UNTITLED1_EX1_H
