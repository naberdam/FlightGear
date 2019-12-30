//
// Created by nerya on 12/12/2019.
//

#ifndef EX3_READDATA_H
#define EX3_READDATA_H

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <stack>

using namespace std;
class ReadData {
private:
    vector<vector<string>> vectorOfDataFromFile;

public:
    ReadData();

    vector<vector<string>> lexer(string nameOfFile);

    void orderWithSpaces(string leftTextInLine, unsigned int index);
    void orderTextWithParenthesis(string rightTextInLine, unsigned int index);
    void orderByOperatorCondition(string text, unsigned int index);
    bool isThereEqualSignInText(string text);
    bool isThereOpenSpecialParenthesisInText(string text);
    bool isThereCloseSpecialParenthesisInText(string text);
    bool isThereApostrophesInText(string text);
    string deleteSpacesBetweenExpressions(string text);
    string deleteSpacesFromTheBeginningOfTheLine(string text);
    string deleteTabsFromTheBeginningOfTheLine(string text);
    string deleteOpenSpecialParenthesis(string text);
    void addEqualSignToTheBegin(string text, unsigned int index);
    void addArrowSignToTheBegin(string text, unsigned int index);
    string switchFirstOpenParenthesisToSpace(string text);
    /*string switchLastCloseParenthesisToSpace(string text);*/

    bool isThereVarInStartOfLine(string text);
    unsigned int countSpacesInText(string text);
    string deleteWordWhileOrIf(string text, vector<string>& vectOfSpecialParenthesis);

    virtual ~ReadData();


};


#endif //EX3_READDATA_H
