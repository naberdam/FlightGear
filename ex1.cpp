

//
// Created by nerya on 05/11/2019.
//

#include <iostream>
#include "ex1.h"

using namespace std;

Variable::Variable(string name1, double value1) {
    this->name = name1;
    this->value = value1;
}

Variable &Variable::operator++() {
    setValue(getValue() + 1);
    return *this;
}

// Define postfix increment operator.
Variable &Variable::operator++(int) {
    setValue(getValue() + 1);
    return *this;
}

// Define prefix decrement operator.
Variable &Variable::operator--() {
    setValue(getValue() - 1);
    return *this;
}

// Define postfix decrement operator.
Variable Variable::operator--(int) {
    setValue(getValue() - 1);
    return *this;
}

Variable &Variable::operator+=(double val) {
    setValue(getValue() + val);
    return *this;
}

Variable &Variable::operator-=(double val) {
    setValue(getValue() - val);
    return *this;
}

double Variable::calculate() {
    return getValue();
}

double Variable::getValue() {
    return value;
}

void Variable::setName(string &name1) {
    Variable::name = name1;
}

void Variable::setValue(double value1) {
    Variable::value = value1;
}

const string &Variable::getName() const {
    return name;
}

Expression *BinaryOperator::getLeft() {
    return left;
}

void BinaryOperator::setLeft(Expression *left1) {
    BinaryOperator::left = left1;
}

Expression *BinaryOperator::getRight() {
    return right;
}

void BinaryOperator::setRight(Expression *right1) {
    BinaryOperator::right = right1;
}

BinaryOperator::BinaryOperator(Expression *left1, Expression *right1) : left(left1), right(right1) {}

BinaryOperator::~BinaryOperator() {
    delete left;
    delete right;

}

double Plus::calculate() {
    return (this->getLeft()->calculate() + (this->getRight()->calculate()));
}

Plus::Plus(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Minus::calculate() {
    return (this->getLeft()->calculate() - (this->getRight()->calculate()));
}

Minus::Minus(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Mul::calculate() {
    return (this->getLeft()->calculate() * (this->getRight()->calculate()));
}

Mul::Mul(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Div::calculate() {
    if (this->getRight()->calculate() == 0.0) {
        throw "error in calculate div";
    }
    return (this->getLeft()->calculate() / (this->getRight()->calculate()));

}

Div::Div(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

UPlus::UPlus(Expression *expression1) : UnaryOperator(expression1) {}

double UPlus::calculate() {
    return getExpression()->calculate();
}

UMinus::UMinus(Expression *expression1) : UnaryOperator(expression1) {}

double UMinus::calculate() {
    return getExpression()->calculate() * (-1);
}

bool Interpreter::isLetter(string token) {
    //check if there is illegal letter in first char
    if ((token[0] < 'a' || token[0] > 'z') && (token[0] < 'A' || token[0] > 'Z') && token[0] != '_') {
        return false;
    }
    //check other chars in token if they are legal
    for (int i = 1; i < (int) token.size(); i++) {
        if ((token[i] < 'a' || token[i] > 'z') && (token[i] < 'A' || token[i] > 'Z') && token[i] != '_' &&
                (token[i] < '0' || token[i] > '9')) {
            return false;
        }
    }
    return true;
}

bool Interpreter::isThisStringIsVariableOrJustString(std::__cxx11::string nameOfVar) {
    //first, check if the map is empty
    if (this->valuesOfVariables.empty()) {
        return false;
    }
    //check if we have this name of var in our map
    if (this->valuesOfVariables.count(nameOfVar)) {
        return true;
    }
    return false;
}

string Interpreter::getValueOfVariable(std::__cxx11::string nameOfVar) {
    string valueWithoutParanthesis;
    if (this->valuesOfVariables.count(nameOfVar)) {
        //check if the value of nameOfVar is negative and it has parenthesis
        if (this->valuesOfVariables[nameOfVar][0] == '(') {
            //delete parenthesis
            valueWithoutParanthesis = this->valuesOfVariables[nameOfVar].substr(1);
            valueWithoutParanthesis = valueWithoutParanthesis.substr(0, valueWithoutParanthesis.size() - 1);
            return valueWithoutParanthesis;
        }
        //else - positive value
        return this->valuesOfVariables[nameOfVar];
    }
    throw "error in getValueOfVariable";
}

bool Interpreter::isLetterChar(char stringCheck) {
    //letter between a to z
    if (stringCheck >= 'a' && stringCheck <= 'z') {
        return 1;
    }
    //letter between A to Z
    if (stringCheck >= 'A' && stringCheck <= 'Z') {
        return 1;
    }
    if (stringCheck == '_') {
        return 1;
    }
    //else = it is illegal
    return 0;
}

bool Interpreter::isNumberString(string number) {
    //this boolean tells us if we already have number and we can use '.'
    bool isTheNumberCanBeGood = false;
    //this boolean tells us if we already have '.'
    bool alreadyHadPoint = false;
    for (int i = 0; (unsigned) i < number.size(); ++i) {
        //negative number
        if (i == 0 && number[i] == '-') {
            continue;
        }
        //the letter is not a number
        if (!isNumberChar(number[i])) {
            return 0;
        }
        //the first '.' and the string we have till now is good
        if (number[i] == '.' && !alreadyHadPoint && isTheNumberCanBeGood) {
            alreadyHadPoint = true;
            continue;
        }
        //it is not a correct number
        if (!isTheNumberCanBeGood && number[i] == '.') {
            return 0;
        }
        //it is not a correct number because we already have one '.'
        if (alreadyHadPoint && number[i] == '.') {
            return 0;
        }
        isTheNumberCanBeGood = true;
    }
    if (!isTheNumberCanBeGood) {
        return 0;
    }
    return 1;
}

bool Interpreter::checkProperString(string stringCheck) {
    //check if the string is not empty
    if (stringCheck.empty() || !strcmp(stringCheck.c_str(), "")) {
        throw "the string is empty";
    }
    return 1;
}

void Interpreter::orderMapByValuesAndVariables(string tokenOfAllText) {
    string tokenOfSmallTextLeft, tokenOfSmallTextRight, delimiterOfSmallText = "=";
    size_t posOfSmallText;
    posOfSmallText = tokenOfAllText.find(delimiterOfSmallText);
    //tokenOfSmallTextLeft is the variable
    tokenOfSmallTextLeft = tokenOfAllText.substr(0, posOfSmallText);
    string valueVariableIfValueIsMinus;
    //check is the variable and the value are not equal
    if (strcmp(tokenOfAllText.c_str(), tokenOfSmallTextLeft.c_str())) {
        tokenOfAllText.erase(0, posOfSmallText + delimiterOfSmallText.length());
        posOfSmallText = tokenOfAllText.find(delimiterOfSmallText);
        //tokenOfSmallTextRight is the value
        tokenOfSmallTextRight = tokenOfAllText.substr(0, posOfSmallText);
        //check the variable and value are not empty
        if (tokenOfSmallTextRight != "" && tokenOfSmallTextLeft != "") {
            //check proper variable and value
            if (isLetter(tokenOfSmallTextLeft) && isNumberString(tokenOfSmallTextRight)) {
                //if it is a negative number, we want to put parenthesis
                if (tokenOfSmallTextRight[0] == '-') {
                    valueVariableIfValueIsMinus = "(";
                    valueVariableIfValueIsMinus += tokenOfSmallTextRight;
                    valueVariableIfValueIsMinus += ")";
                    tokenOfSmallTextRight = valueVariableIfValueIsMinus;
                }
                //checking if we have this variable already in our map
                if (valuesOfVariables.find(tokenOfSmallTextLeft) == valuesOfVariables.end()) {
                    valuesOfVariables.insert(pair<string, string>(tokenOfSmallTextLeft, tokenOfSmallTextRight));
                } else {
                    valuesOfVariables[tokenOfSmallTextLeft] = tokenOfSmallTextRight;
                }
                //one of them is empty
            } else {
                throw "the string is not correct - 1";
            }
        }
        //if the variable and value are equal so the string is not good and we need to throw an exception
    } else {
        throw "the string is not correct - 2";
    }
}

void Interpreter::setVariables(string variablesValue) {
    //the basic of this function is from stackoverflow
    checkProperString(variablesValue);
    string delimiterOfAllText = ";", delimiterOfSmallText = "=";
    size_t posOfAllText = 0;
    string tokenOfAllText;
    //iterations on variablesValue so can update our map of variables
    while ((posOfAllText = variablesValue.find(delimiterOfAllText)) != string::npos) {
        tokenOfAllText = variablesValue.substr(0, posOfAllText);
        variablesValue.erase(0, posOfAllText + delimiterOfAllText.length());
        //send to the function expression that contains variable '=' value
        orderMapByValuesAndVariables(tokenOfAllText);
    }
    //for the last text
    orderMapByValuesAndVariables(variablesValue);
}

//this function has been taken from stackoverflow
string Interpreter::replaceAll(string *str, const string &from, const string &to) {
    size_t start_pos = 0;
    while ((start_pos = str->find(from, start_pos)) != string::npos) {
        if (isOperator((*str)[start_pos + from.length()])
                || (*str)[start_pos + from.length()] == ')'
                || (*str)[start_pos + from.length()] == '\0') {
            str->replace(start_pos, from.length(), to);
        }
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return *str;
}

bool Interpreter::checkProperReplaceVariables(string stringCheck) {
    for (int i = 0; (unsigned) i < stringCheck.size(); i++) {
        checkProperEquation(stringCheck, i);
    }
    return 1;
}

Expression *Interpreter::interpret(string exp) {
    int i = 0;
    stack<string> stackOperators;
    stack<Expression *> stackExpressions;
    queue<string> queuePolish;
    queue<Expression *> queueUMinus;
    Expression *firstTemp{nullptr};
    bool isNumberOrNot = 0;
    char checkUminusOrMinus = 32, checkUplusOrPlus = 32;
    string number, operatorToStack;
    //replace all variables in equation with their values
    if (!valuesOfVariables.empty()) {
        for (map<string, string>::iterator it = valuesOfVariables.begin(); it != valuesOfVariables.end(); it++) {
            replaceAll((&exp), string(it->first), string(it->second));
        }
    }
    //if there is letter so it is not a good equation
    checkProperReplaceVariables(exp);
    //check if every open parenthesis there is close parenthesis
    checkProperParenthesis(exp);
    while (exp.size() > 0) {
        checkProperEquation(exp, i);
        //while we have number so we want to save those chars in 'number'
        while (isNumberChar(exp[i])) {
            number += exp[i];
            checkUminusOrMinus = exp[i];
            checkUplusOrPlus = exp[i];
            exp = exp.substr(i + 1, exp.size());
            isNumberOrNot = 1;
        }
        //is it is a number so we want to insert 'number' to queuePolish
        if (isNumberOrNot) {
            //check proper number
            if (isNumberString(number)) {
                queuePolish.push(number);
                number.clear();
                isNumberOrNot = 0;
            } else {
                //it is not a proper number
                throw "the equation is not good";
            }
        }
        if (exp[i] == '(') {
            operatorToStack += exp[i];
            stackOperators.push(operatorToStack);
            operatorToStack.clear();
            checkUminusOrMinus = exp[i];
            checkUplusOrPlus = exp[i];
            exp = exp.substr(i + 1, exp.size());
            //check if after the '(' we have '-', so we save him in 'number' because it is a negative number or Uminus
            if (exp[i] == '-' && exp[i + 1] != '(') {
                number += exp[i];
                //erase '-' from exp
                exp = exp.substr(i + 1, exp.size());
            }
            continue;
        }
        if (isOperator(exp[i])) {
            //Uminus
            if (exp[i] == '-' && exp[i + 1] == '(' && !isNumberChar(checkUminusOrMinus) && checkUplusOrPlus != ')') {
                Interpreter *i1 = new Interpreter();
                exp = exp.substr(i + 1, exp.size());
                string temp;
                int j = i;
                int countParenthesisOfUminus = 1;
                //create temp with the expression between the open parenthesis to the close parenthesis
                while (countParenthesisOfUminus) {
                    temp += exp[j + 1];
                    j++;
                    if (exp[j] == '(') {
                        countParenthesisOfUminus++;
                    } else if (exp[j + 1] == ')') {
                        countParenthesisOfUminus--;
                    }
                }
                //recursive on the expression between the open parenthesis to the close parenthesis
                Expression *e4 = i1->interpret(temp);
                firstTemp = new UMinus(e4);
                queueUMinus.push(firstTemp);
                //a sign for me that there is expression i need to handle
                queuePolish.push("#");
                //erase temp from exp because we handle it already and we have it in queueUMinus
                exp = exp.substr(i + j + 2, exp.size());
                checkUplusOrPlus = ')';
                delete i1;
                continue;
            }
            //UPlus
            if (exp[i] == '+' && exp[i + 1] == '(' && !isNumberChar(checkUplusOrPlus) && checkUplusOrPlus != ')') {
                exp = exp.substr(i + 1, exp.size());
                continue;
            }
            //negative number without open parenthesis before
            if (exp[i] == '-' && isNumberChar(exp[i + 1]) && checkUminusOrMinus == 32) {
                number += exp[i];
                exp = exp.substr(i + 1, exp.size());
                continue;
            }
            if (!stackOperators.empty()) {
                string topStackOperator = stackOperators.top();
                //pop operators by calculation priority
                while (priorityOfOperators(exp[i], topStackOperator) && !stackOperators.empty() && exp[i] != ')') {
                    queuePolish.push(stackOperators.top());
                    stackOperators.pop();
                    if (stackOperators.empty()) {
                        break;
                    }
                    topStackOperator = stackOperators.top();
                }
            }
            operatorToStack += exp[i];
            stackOperators.push(operatorToStack);
            operatorToStack.clear();
            checkUminusOrMinus = exp[i];
            checkUplusOrPlus = exp[i];
            exp = exp.substr(i + 1, exp.size());
            continue;
        }
        if (exp[i] == ')') {
            //pop all operators till we have ')'
            while (!stackOperators.empty() && stackOperators.top() != "(") {
                queuePolish.push(stackOperators.top());
                stackOperators.pop();
            }
            //we have in the top of the stack ')'
            stackOperators.pop();
            checkUminusOrMinus = exp[i];
            checkUplusOrPlus = exp[i];
            exp = exp.substr(i + 1, exp.size());

        }

    }
    while (!stackOperators.empty()) {
        queuePolish.push(stackOperators.top());
        stackOperators.pop();
    }
    Expression *firstNumber, *secondNumber;
    string operatorCalculation;
    Value *valueQueue;
    while (!queuePolish.empty()) {
        while (!queuePolish.empty() && !isOperator(queuePolish.front())) {
            //if we need to use queueUMinus
            if (!strcmp(queuePolish.front().c_str(), "#")) {
                stackExpressions.push(queueUMinus.front());
                queueUMinus.pop();
            } else {
                valueQueue = new Value(stod(queuePolish.front()));
                stackExpressions.push(valueQueue);
            }
            queuePolish.pop();
        }
        //start calculation of expressions
        if (!queuePolish.empty()) {
            //right number in equation
            secondNumber = stackExpressions.top();
            stackExpressions.pop();
            if (stackExpressions.empty()) {
                break;
            }
            //left number in equation
            firstNumber = stackExpressions.top();
            stackExpressions.pop();
            //push new binary operator with two numbers and operator we got from queuePolish
            stackExpressions.push(binaryCalculate(firstNumber, secondNumber, queuePolish.front()));
            queuePolish.pop();
        }
    }
    checkProperStack(stackExpressions);
    return stackExpressions.top();
}

bool Interpreter::checkProperStack(stack<Expression *> stackCheck) {
    if (stackCheck.empty()) {
        throw "the expression is not good";
    }
    stackCheck.pop();
    if (!stackCheck.empty()) {
        throw "the expression is not good";
    }
    return 1;

}

bool Interpreter::checkProperParenthesis(string stringCheck) {
    int countOpenParenthesis = 0, countCloseParenthesis = 0;
    for (int i = 0; (unsigned) i < stringCheck.size(); ++i) {
        if (stringCheck[i] == '(') {
            countOpenParenthesis++;
        }
        if (stringCheck[i] == ')') {
            countCloseParenthesis++;
        }
    }
    if (countCloseParenthesis != countOpenParenthesis) {
        throw "Parenthesis is not correct";
    }
    return 1;

}

bool Interpreter::checkProperEquation(string charOfExpCheck, unsigned int i) {
    //not one of the correct possibilities to char in the expression
    if (!isNumberChar(charOfExpCheck[i]) && !isOperator(charOfExpCheck[i]) && charOfExpCheck[i] != '(' &&
            charOfExpCheck[i] != ')') {
        throw "the equation is not correct";
    }
    if (charOfExpCheck.size() > 1) {
        //two operators one by one
        if (isOperator(charOfExpCheck[i]) && isOperator(charOfExpCheck[i + 1])) {
            throw "the equation is not correct";
        }
        if (charOfExpCheck[i] == '-' && isLetterChar(charOfExpCheck[i + 1])) {
            throw "the equation is not correct";
        }
    }
    return 1;

}

string Interpreter::updateString(string exp, int i, int j, double loadNumber) {
    string newExp = "(" + to_string(loadNumber) + ")";
    i += j + 2;
    exp = exp.substr(i, exp.size());
    while (exp.size() > 0) {
        newExp += exp[i];
    }
    return newExp;
}

Expression *Interpreter::binaryCalculate(Expression *firstExpression, Expression *secondExpression,
                                         string operatorCalculation) {
    Expression *calculation{nullptr};
    //create binary expression by operatorCalculation
    if (operatorCalculation == "+") {
        calculation = new Plus(firstExpression, secondExpression);
    } else if (operatorCalculation == "-") {
        calculation = new Minus(firstExpression, secondExpression);
    } else if (operatorCalculation == "*") {
        calculation = new Mul(firstExpression, secondExpression);
    } else {
        calculation = new Div(firstExpression, secondExpression);
    }
    return calculation;
}

bool Interpreter::priorityOfOperators(char outStack, string inStack) {
    return ((outStack == '+' || outStack == '-') && (inStack == "*" || inStack == "/"))
            || (outStack == '+' && inStack == "-")
            || (outStack == '-' && inStack == "+");
}

bool Interpreter::isOperator(char character) {
    switch (character) {
        case '-':return 1;
        case '/':return 1;
        case '+':return 1;
        case '*':return 1;
        default:break;
    }
    return 0;
}

bool Interpreter::isOperator(string queueFront) {
    return (queueFront == "-" || queueFront == "+" || queueFront == "/" || queueFront == "*");
}

bool Interpreter::isNumberChar(char character) {
    switch (character) {
        case '0':return 1;
        case '1':return 1;
        case '2':return 1;
        case '3':return 1;
        case '4':return 1;
        case '5':return 1;
        case '6':return 1;
        case '7':return 1;
        case '8':return 1;
        case '9':return 1;
        case '.':return 1;
        default:break;
    }
    return 0;
}





/*
//
// Created by nerya on 05/11/2019.
//

#include <iostream>
#include "ex1.h"

using namespace std;

Variable::Variable(string name1, double value1) {
    this->name = name1;
    this->value = value1;
}

Variable &Variable::operator++() {
    setValue(getValue() + 1);
    return *this;
}

// Define postfix increment operator.
Variable &Variable::operator++(int) {
    setValue(getValue() + 1);
    return *this;
}

// Define prefix decrement operator.
Variable &Variable::operator--() {
    setValue(getValue() - 1);
    return *this;
}

// Define postfix decrement operator.
Variable Variable::operator--(int) {
    setValue(getValue() - 1);
    return *this;
}

Variable &Variable::operator+=(double val) {
    setValue(getValue() + val);
    return *this;
}

Variable &Variable::operator-=(double val) {
    setValue(getValue() - val);
    return *this;
}

double Variable::calculate() {
    return getValue();
}

double Variable::getValue() {
    return value;
}

void Variable::setName(string &name1) {
    Variable::name = name1;
}

void Variable::setValue(double value1) {
    Variable::value = value1;
}

const string &Variable::getName() const {
    return name;
}

Expression *BinaryOperator::getLeft() {
    return left;
}

void BinaryOperator::setLeft(Expression *left1) {
    BinaryOperator::left = left1;
}

Expression *BinaryOperator::getRight() {
    return right;
}

void BinaryOperator::setRight(Expression *right1) {
    BinaryOperator::right = right1;
}

BinaryOperator::BinaryOperator(Expression *left1, Expression *right1) : left(left1), right(right1) {}

BinaryOperator::~BinaryOperator() {
    delete left;
    delete right;

}

double Plus::calculate() {
    return (this->getLeft()->calculate() + (this->getRight()->calculate()));
}

Plus::Plus(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Minus::calculate() {
    return (this->getLeft()->calculate() - (this->getRight()->calculate()));
}

Minus::Minus(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Mul::calculate() {
    return (this->getLeft()->calculate() * (this->getRight()->calculate()));
}

Mul::Mul(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Div::calculate() {
    if (this->getRight()->calculate() == 0.0) {
        throw "error in calculate div";
    }
    return (this->getLeft()->calculate() / (this->getRight()->calculate()));

}

Div::Div(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

UPlus::UPlus(Expression *expression1) : UnaryOperator(expression1) {}

double UPlus::calculate() {
    return getExpression()->calculate();
}

UMinus::UMinus(Expression *expression1) : UnaryOperator(expression1) {}

double UMinus::calculate() {
    return getExpression()->calculate() * (-1);
}

bool Interpreter::isLetter(string token) {
    if ((token[0] < 'a' || token[0] > 'z') && (token[0] < 'A' || token[0] > 'Z') && token[0] != '_') {
        return false;
    }
    for (int i = 1; i < (int) token.size(); i++) {
        if ((token[i] < 'a' || token[i] > 'z') && (token[i] < 'A' || token[i] > 'Z') && token[i] != '_' &&
            (token[i] < '0' || token[i] > '9')) {
            return false;
        }
    }
    return true;
}

bool Interpreter::isThisStringIsVariableOrJustString(std::__cxx11::string nameOfVar) {
    if (this->valuesOfVariables.empty()) {
        return false;
    }
    if (this->valuesOfVariables.count(nameOfVar)) {
        return true;
    }
}

string Interpreter::getValueOfVariable(std::__cxx11::string nameOfVar) {
    if (this->valuesOfVariables.count(nameOfVar)) {
        return this->valuesOfVariables[nameOfVar];
    }
    throw "error in getValueOfVariable";
}

bool Interpreter::isLetterChar(char stringCheck) {
    if (stringCheck >= 'a' && stringCheck <= 'z') {
        return 1;
    }
    if (stringCheck >= 'A' && stringCheck <= 'Z') {
        return 1;
    }
    if (stringCheck == '_') {
        return 1;
    }
    return 0;
}

bool Interpreter::isNumberString(string number) {
    bool isTheNumberCanBeGood = false;
    for (int i = 0; (unsigned) i < number.size(); ++i) {
        if (i == 0 && number[i] == '-') {
            continue;
        }
        if (!isNumberChar(number[i])) {
            return 0;
        }
        if (!isTheNumberCanBeGood && number[i] == '.') {
            return 0;
        }
        isTheNumberCanBeGood = true;
    }
    if (!isTheNumberCanBeGood) {
        return 0;
    }
    return 1;
}

bool Interpreter::checkProperString(string stringCheck) {
    if (stringCheck.empty() || !strcmp(stringCheck.c_str(), "")) {
        throw "the string is empty";
    }
    return 1;
}

void Interpreter::orderMapByValuesAndVariables(string tokenOfAllText) {
    string tokenOfSmallTextLeft, tokenOfSmallTextRight, delimiterOfSmallText = "=";
    size_t posOfSmallText;
    posOfSmallText = tokenOfAllText.find(delimiterOfSmallText);
    //tokenOfSmallTextLeft is the variable
    tokenOfSmallTextLeft = tokenOfAllText.substr(0, posOfSmallText);
    string valueVariableIfValueIsMinus;
    //check is the variable and the value are not equal
    if (strcmp(tokenOfAllText.c_str(), tokenOfSmallTextLeft.c_str())) {
        tokenOfAllText.erase(0, posOfSmallText + delimiterOfSmallText.length());
        posOfSmallText = tokenOfAllText.find(delimiterOfSmallText);
        //tokenOfSmallTextRight is the value
        tokenOfSmallTextRight = tokenOfAllText.substr(0, posOfSmallText);
        //check the variable and value are not empty
        if (tokenOfSmallTextRight != "" && tokenOfSmallTextLeft != "") {
            //check proper variable and value
            if (isLetter(tokenOfSmallTextLeft) && isNumberString(tokenOfSmallTextRight)) {
                //if it is a negative number, we want to put parenthesis
                if (tokenOfSmallTextRight[0] == '-') {
                    valueVariableIfValueIsMinus = "(";
                    valueVariableIfValueIsMinus += tokenOfSmallTextRight;
                    valueVariableIfValueIsMinus += ")";
                    tokenOfSmallTextRight = valueVariableIfValueIsMinus;
                }
                //checking if we have this variable already in our map
                if (valuesOfVariables.find(tokenOfSmallTextLeft) == valuesOfVariables.end()) {
                    valuesOfVariables.insert(pair<string, string>(tokenOfSmallTextLeft, tokenOfSmallTextRight));
                } else {
                    valuesOfVariables[tokenOfSmallTextLeft] = tokenOfSmallTextRight;
                }
                //one of them is empty
            } else {
                throw "the string is not correct - 1";
            }
        }
        //if the variable and value are equal so the string is not good and we need to throw an exception
    } else {
        throw "the string is not correct - 2";
    }
}

void Interpreter::setVariables(string variablesValue) {
    //the basic of this function is from stackoverflow
    checkProperString(variablesValue);
    */
/*try {
    }
    catch (exception excp1) {
        cout << "the string is empty" << endl;
    }*//*

    string delimiterOfAllText = ";", delimiterOfSmallText = "=";
    size_t posOfAllText = 0;
    string tokenOfAllText;
    while ((posOfAllText = variablesValue.find(delimiterOfAllText)) != string::npos) {
        tokenOfAllText = variablesValue.substr(0, posOfAllText);
        variablesValue.erase(0, posOfAllText + delimiterOfAllText.length());
        orderMapByValuesAndVariables(tokenOfAllText);
        */
/*try {
        } catch (exception excp2) {
            cout << "error: variables are not correct" << endl;
        }*//*

    }
    orderMapByValuesAndVariables(variablesValue);
    */
/*try {
    } catch (exception excp3) {
        cout << "error: variables are not correct" << endl;
    }*//*


}

//this function has been taken from stackoverflow
string Interpreter::replaceAll(string *str, const string &from, const string &to) {
    size_t start_pos = 0;
    while ((start_pos = str->find(from, start_pos)) != string::npos) {
        if (isOperator((*str)[start_pos + from.length()])
            || (*str)[start_pos + from.length()] == ')'
            || (*str)[start_pos + from.length()] == '\0') {
            str->replace(start_pos, from.length(), to);
        }
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return *str;
}

bool Interpreter::checkProperReplaceVariables(string stringCheck) {
    for (int i = 0; (unsigned) i < stringCheck.size(); i++) {
        checkProperEquation(stringCheck, i);
    }
    return 1;
}

Expression *Interpreter::interpret(string exp) {
    int i = 0;
    stack<string> stackOperators;
    stack<Expression *> stackExpressions;
    queue<string> queuePolish;
    queue<Expression *> queueUMinus;
    Expression *firstTemp{nullptr};
    bool isNumberOrNot = 0;
    char checkUminusOrMinus = 32, checkUplusOrPlus = 32;
    string number, operatorToStack;
    //replace all variables in equation with their values
    if (!valuesOfVariables.empty()) {
        for (map<string, string>::iterator it = valuesOfVariables.begin(); it != valuesOfVariables.end(); it++) {
            replaceAll((&exp), string(it->first), string(it->second));
        }
    }
    //if there is letter so it is not a good equation
    checkProperReplaceVariables(exp);
    //check if every open parenthesis there is close parenthesis
    checkProperParenthesis(exp);
    while (exp.size() > 0) {
        checkProperEquation(exp, i);
        //while we have number so we want to save those chars in 'number'
        while (isNumberChar(exp[i])) {
            number += exp[i];
            checkUminusOrMinus = exp[i];
            checkUplusOrPlus = exp[i];
            exp = exp.substr(i + 1, exp.size());
            isNumberOrNot = 1;
        }
        //is it is a number so we want to insert 'number' to queuePolish
        if (isNumberOrNot) {
            //check proper number
            if (isNumberString(number)) {
                queuePolish.push(number);
                number.clear();
                isNumberOrNot = 0;
            } else {
                throw "the equation is not good";
            }
        }
        if (exp[i] == '(') {
            operatorToStack += exp[i];
            stackOperators.push(operatorToStack);
            operatorToStack.clear();
            checkUminusOrMinus = exp[i];
            checkUplusOrPlus = exp[i];
            exp = exp.substr(i + 1, exp.size());
            //check if after the '(' we have '-', so we save him in 'number' because it is a negative number or Uminus
            if (exp[i] == '-' && exp[i + 1] != '(') {
                number += exp[i];
                //erase '-' from exp
                exp = exp.substr(i + 1, exp.size());
            }
            continue;
        }
        if (isOperator(exp[i])) {
            //Uminus
            if (exp[i] == '-' && exp[i + 1] == '(' && !isNumberChar(checkUminusOrMinus) && checkUplusOrPlus != ')') {
                Interpreter *i1 = new Interpreter();
                exp = exp.substr(i + 1, exp.size());
                string temp;
                int j = i;
                int countParenthesisOfUminus = 1;
                //create temp with the expression between the open parenthesis to the close parenthesis
                while (countParenthesisOfUminus) {
                    temp += exp[j + 1];
                    j++;
                    if (exp[j] == '(') {
                        countParenthesisOfUminus++;
                    } else if (exp[j + 1] == ')') {
                        countParenthesisOfUminus--;
                    }
                }
                //recursive on the expression between the open parenthesis to the close parenthesis
                Expression *e4 = i1->interpret(temp);
                firstTemp = new UMinus(e4);
                queueUMinus.push(firstTemp);
                //a sign for me that there is expression i need to handle
                queuePolish.push("#");
                //erase temp from exp because we handle it already and we have it in queueUMinus
                exp = exp.substr(i + j + 2, exp.size());
                checkUplusOrPlus = ')';
                delete i1;
                continue;
            }
            //UPlus
            if (exp[i] == '+' && exp[i + 1] == '(' && !isNumberChar(checkUplusOrPlus) && checkUplusOrPlus != ')') {
                exp = exp.substr(i + 1, exp.size());
                continue;
            }
            //negative number without open parenthesis before
            if (exp[i] == '-' && isNumberChar(exp[i + 1]) && checkUminusOrMinus == 32) {
                number += exp[i];
                exp = exp.substr(i + 1, exp.size());
                continue;
            }
            if (!stackOperators.empty()) {
                string topStackOperator = stackOperators.top();
                //pop operators by calculation priority
                while (priorityOfOperators(exp[i], topStackOperator) && !stackOperators.empty() && exp[i] != ')') {
                    queuePolish.push(stackOperators.top());
                    stackOperators.pop();
                    if  (stackOperators.empty()) {
                        break;
                    }
                    topStackOperator = stackOperators.top();
                }
            }
            operatorToStack += exp[i];
            stackOperators.push(operatorToStack);
            operatorToStack.clear();
            checkUminusOrMinus = exp[i];
            checkUplusOrPlus = exp[i];
            exp = exp.substr(i + 1, exp.size());
            continue;
        }
        if (exp[i] == ')') {
            //pop all operators till we have ')'
            while (!stackOperators.empty() && stackOperators.top() != "(") {
                queuePolish.push(stackOperators.top());
                stackOperators.pop();
            }
            stackOperators.pop();
            checkUminusOrMinus = exp[i];
            checkUplusOrPlus = exp[i];
            exp = exp.substr(i + 1, exp.size());

        }

    }
    while (!stackOperators.empty()) {
        queuePolish.push(stackOperators.top());
        stackOperators.pop();
    }
    Expression *firstNumber, *secondNumber;
    string operatorCalculation;
    Value *valueQueue;
    while (!queuePolish.empty()) {
        while (!queuePolish.empty() && !isOperator(queuePolish.front())) {
            //if we need to use firstTemp/queueUMinus
            if (!strcmp(queuePolish.front().c_str(), "#")) {
                stackExpressions.push(queueUMinus.front());
                queueUMinus.pop();
            } else {
                valueQueue = new Value(stod(queuePolish.front()));
                stackExpressions.push(valueQueue);
            }
            queuePolish.pop();
        }
        if (!queuePolish.empty()) {
            secondNumber = stackExpressions.top();
            stackExpressions.pop();
            if (stackExpressions.empty()) {
                break;
            }
            firstNumber = stackExpressions.top();
            stackExpressions.pop();
            //push new binary operator with two numbers and operator we got from queuePolish
            stackExpressions.push(binaryCalculate(firstNumber, secondNumber, queuePolish.front()));
            queuePolish.pop();
        }
    }
    checkProperStack(stackExpressions);
    return stackExpressions.top();
}
bool Interpreter::checkProperStack(stack<Expression *> stackCheck) {
    if (stackCheck.empty()) {
        throw "the expression is not good";
    }
    stackCheck.pop();
    if (!stackCheck.empty()) {
        throw "the expression is not good";
    }
    return 1;

}

bool Interpreter::checkProperParenthesis(string stringCheck) {
    int countOpenParenthesis = 0, countCloseParenthesis = 0;
    for (int i = 0; (unsigned) i < stringCheck.size(); ++i) {
        if (stringCheck[i] == '(') {
            countOpenParenthesis++;
        }
        if (stringCheck[i] == ')') {
            countCloseParenthesis++;
        }
    }
    if (countCloseParenthesis != countOpenParenthesis) {
        throw "Parenthesis is not correct";
    }
    return 1;

}


bool Interpreter::checkProperEquation(string charOfExpCheck, unsigned int i) {
    //not one of the correct possibilities to char in the expression
    if (!isNumberChar(charOfExpCheck[i]) && !isOperator(charOfExpCheck[i]) && charOfExpCheck[i] != '(' &&
        charOfExpCheck[i] != ')') {
        throw "the equation is not correct";
    }
    if (charOfExpCheck.size() > 1) {
        //two operators one by one
        if (isOperator(charOfExpCheck[i]) && isOperator(charOfExpCheck[i + 1])) {
            throw "the equation is not correct";
        }
        if (charOfExpCheck[i] == '-' && isLetterChar(charOfExpCheck[i + 1])) {
            throw "the equation is not correct";
        }
    }
    return 1;

}

string Interpreter::updateString(string exp, int i, int j, double loadNumber) {
    string newExp = "(" + to_string(loadNumber) + ")";
    i += j + 2;
    exp = exp.substr(i, exp.size());
    while (exp.size() > 0) {
        newExp += exp[i];
    }
    return newExp;
}

Expression *Interpreter::binaryCalculate(Expression *firstExpression, Expression *secondExpression,
                                         string operatorCalculation) {
    Expression *calculation{nullptr};
    if (operatorCalculation == "+") {
        calculation = new Plus(firstExpression, secondExpression);
    } else if (operatorCalculation == "-") {
        calculation = new Minus(firstExpression, secondExpression);
    } else if (operatorCalculation == "*") {
        calculation = new Mul(firstExpression, secondExpression);
    } else {
        calculation = new Div(firstExpression, secondExpression);
    }
    return calculation;
}

*/
/*Expression* Interpreter::binaryCalculate(double firstNumber, double secondNumber, const string operatorCalculation) {
    Value firstValue(firstNumber);
    Value secondValue(secondNumber);
    Expression *calculation{nullptr};
    if (operatorCalculation == "+") {
        calculation = new Plus(&firstValue, &secondValue);
    } else if (operatorCalculation == "-") {
        calculation = new Minus(&firstValue, &secondValue);
    } else if (operatorCalculation == "*") {
        calculation = new Mul(&firstValue, &secondValue);
    } else {
        calculation = new Div(&firstValue, &secondValue);
    }
    if (calculation != nullptr) {
        return calculation;
    }
}*//*


bool Interpreter::priorityOfOperators(char outStack, string inStack) {
    return ((outStack == '+' || outStack == '-') && (inStack == "*" || inStack == "/"))
           || (outStack == '+' && inStack == "-")
           || (outStack == '-' && inStack == "+");
}

bool Interpreter::isOperator(char character) {
    switch (character) {
        case '-':
            return 1;
        case '/':
            return 1;
        case '+':
            return 1;
        case '*':
            return 1;
        default:
            break;
    }
    return 0;
}

bool Interpreter::isOperator(string queueFront) {
    return (queueFront == "-" || queueFront == "+" || queueFront == "/" || queueFront == "*");
}

bool Interpreter::isNumberChar(char character) {
    switch (character) {
        case '0':
            return 1;
        case '1':
            return 1;
        case '2':
            return 1;
        case '3':
            return 1;
        case '4':
            return 1;
        case '5':
            return 1;
        case '6':
            return 1;
        case '7':
            return 1;
        case '8':
            return 1;
        case '9':
            return 1;
        case '.':
            return 1;
        default:
            break;
    }
    return 0;
}
*/

