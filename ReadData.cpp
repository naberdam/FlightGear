//
// Created by nerya on 12/12/2019.
//

#include <iostream>
#include "ReadData.h"

using namespace std;

vector<vector<string>> ReadData::lexer(string nameOfFile) {
  ifstream infile(nameOfFile);
  string line;
  string delimiterOpenParenthesis = "(", delimiterEqual = "=", delimiterOpenSpecialParenthesis = "{",
      delimiterCloseSpecialParenthesis = "}";
  string tokenOfTextLeft, tokenOfTextRight;
  size_t posOfThisLine;
  unsigned int i = 0;
  if (infile.is_open()) {
    this->vectorOfDataFromFile.resize(2);
    while (getline(infile, line)) {
      if (this->vectorOfDataFromFile.size() == i + 1) {
        this->vectorOfDataFromFile.resize(2 * (i + 1));
      }
      line = deleteTabsFromTheBeginningOfTheLine(line);
      line = deleteSpacesFromTheBeginningOfTheLine(line);
      //if there is 'var' in the beginning of the line
      if (isThereVarInStartOfLine(line)) {
        //'var' with '='
        if (isThereEqualSignInText(line)) {
          addEqualSignToTheBegin(line, i);
          //'var' with '<-' '->'
        } else {
          posOfThisLine = line.find(delimiterOpenParenthesis);
          tokenOfTextLeft = line.substr(0, posOfThisLine);
          line.erase(0, posOfThisLine + delimiterOpenParenthesis.length());
          tokenOfTextRight = line;
          addArrowSignToTheBegin(tokenOfTextLeft, i);
          orderTextWithParenthesis(tokenOfTextRight, i);
        }
        i++;
        continue;
      }
      //if there is '=' in the line that's mean we will need to handle with variable
      // which we will put in the Interpreter
      if (isThereEqualSignInText(line) && !isThereOpenSpecialParenthesisInText(line)
          && !isThereApostrophesInText(line)) {
        posOfThisLine = line.find(delimiterEqual);
        tokenOfTextLeft = line.substr(0, posOfThisLine);
        line.erase(0, posOfThisLine + delimiterEqual.length());
        tokenOfTextRight = deleteSpacesBetweenExpressions(line);
        orderWithSpaces(tokenOfTextLeft, i);
        this->vectorOfDataFromFile[i].push_back(delimiterEqual);
        this->vectorOfDataFromFile[i].push_back(tokenOfTextRight);
        i++;
        continue;
      }
      //if there is '{' that's mean there is function or loop that we will handle in the commands
      if (isThereOpenSpecialParenthesisInText(line)) {
        line = deleteOpenSpecialParenthesis(line);
        line = deleteWordWhileOrIf(line, this->vectorOfDataFromFile[i]);
        if (this->vectorOfDataFromFile[i][0].compare("while") == 0
            || this->vectorOfDataFromFile[i][0].compare("if") == 0) {
          line = deleteSpacesBetweenExpressions(line);
          orderByOperatorCondition(line, i);
          //it is function
        } else {
          line.erase(0, 1);
          line.erase(line.size() - 1, line.size());
          orderWithSpaces(line, i);
        }
        i++;
        //we put in other cell the '{' to know where to start the conditions of function or loop
        this->vectorOfDataFromFile[i].push_back("{");
        i++;
        continue;
      }
      //end of the loop or function if there is '}'
      if (isThereCloseSpecialParenthesisInText(line)) {
        //we put in other cell the '}' to know where to end the conditions of function or loop
        this->vectorOfDataFromFile[i].push_back("}");
        i++;
        continue;
      }
      //in case we have '(' ')'
      posOfThisLine = line.find(delimiterOpenParenthesis);
      tokenOfTextLeft = line.substr(0, posOfThisLine);
      line.erase(0, posOfThisLine + delimiterOpenParenthesis.length());
      tokenOfTextRight = line;
      orderWithSpaces(tokenOfTextLeft, i);
      orderTextWithParenthesis(tokenOfTextRight, i);
      i++;
    }
    this->vectorOfDataFromFile.resize(i);
    return this->vectorOfDataFromFile;
  } else {
    throw "cant open file";
  }

}

bool ReadData::isThereApostrophesInText(string text) {
  unsigned int i = 0;
  for (; i < text.size(); ++i) {
    if (text[i] == '"') {
      return true;
    }
  }
  return false;
}

void ReadData::addArrowSignToTheBegin(std::__cxx11::string text, unsigned int index) {
  unsigned int i = 0;
  string newSign = " ";
  string temp1, temp2, newText = "var";
  //erasing the word "var " with the space from text
  text.erase(0, 3);
  for (; i < text.size(); i++) {
    if (text[i] == '-' && text[i + 1] == '>') {
      newSign = "->";
      break;
    }
    if (text[i + 1] == '-' && text[i] == '<') {
      newSign = "<-";
      break;
    }
  }
  //take the string from the beginning till the "->" or "<-"
  temp1 = text.substr(0, i - 1);
  //take the string from the "->" or "<-" till the end
  temp2 = text.substr(i + 2, text.size() - i - 2);
  //add to "var" the newSign so we will know which command we will need
  newText += newSign;
  this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(newText));
  this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(temp1));
  this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(temp2));
}

void ReadData::addEqualSignToTheBegin(std::__cxx11::string text, unsigned int index) {
  unsigned int i = 0;
  string newSign = " ";
  string temp1, temp2, newText = "var";
  //erasing the word "var " with the space from text
  text.erase(0, 3);
  for (; i < text.size(); i++) {
    if (text[i] == '=') {
      newSign = "=";
      break;
    }
  }
  //take the string from the beginning till the "="
  temp1 = text.substr(0, i - 1);
  //take the string from the "=" till the end
  temp2 = text.substr(i + 1, text.size() - i - 1);
  //add to "var" the newSign so we will know which command we will need
  newText += newSign;
  this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(newText));
  this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(temp1));
  this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(temp2));
}

bool ReadData::isThereVarInStartOfLine(std::__cxx11::string text) {
  unsigned int i = 0;
  string checkTextVar;
  for (; i <= 3; ++i) {
    checkTextVar += text[i];
  }
  //check is it compares to "var "
  if (!checkTextVar.compare("var ")) {
    return 1;
  }
  return 0;
}

void ReadData::orderByOperatorCondition(string text, unsigned int index) {
  unsigned int i = 0;
  for (; i < text.size(); i++) {
    //handle text with the operator <=
    if (text[i] == '<' && text[i + 1] == '=') {
      //left condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text.substr(0, i)));
      this->vectorOfDataFromFile[index].push_back("<=");
      //erase all chars till operator
      text.erase(0, i + 2);
      //right condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text));
      break;
    }
    //handle text with the operator >=
    if (text[i] == '>' && text[i + 1] == '=') {
      //left condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text.substr(0, i)));
      this->vectorOfDataFromFile[index].push_back(">=");
      //erase all chars till operator
      text.erase(0, i + 2);
      //right condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text));
      break;
    }
    //handle text with the operator !=
    if (text[i] == '!' && text[i + 1] == '=') {
      //left condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text.substr(0, i)));
      this->vectorOfDataFromFile[index].push_back("!=");
      //erase all chars till operator
      text.erase(0, i + 2);
      //right condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text));
      break;
    }
    //handle text with the operator ==
    if (text[i] == '=' && text[i + 1] == '=') {
      //left condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text.substr(0, i)));
      this->vectorOfDataFromFile[index].push_back("==");
      //erase all chars till operator
      text.erase(0, i + 2);
      //right condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text));
      break;
    }
    //handle text with the operator <
    if (text[i] == '<') {
      //left condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text.substr(0, i)));
      this->vectorOfDataFromFile[index].push_back("<");
      //erase all chars till operator
      text.erase(0, i + 1);
      //right condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text));
      break;
    }
    //handle text with the operator >
    if (text[i] == '>') {
      //left condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text.substr(0, i)));
      this->vectorOfDataFromFile[index].push_back(">");
      //erase all chars till operator
      text.erase(0, i + 1);
      //right condition
      this->vectorOfDataFromFile[index].push_back(deleteSpacesBetweenExpressions(text));
      break;
    }
  }
}

string ReadData::deleteWordWhileOrIf(string text, vector<string> &vectOfSpecialParenthesis) {
  unsigned int i = 0;
  while (text[i] != ' ' && text[i] != '(') {
    ++i;
  }
  vectOfSpecialParenthesis.push_back(text.substr(0, i));
  return text.erase(0, i);
}

string ReadData::deleteTabsFromTheBeginningOfTheLine(string text) {
  unsigned int i = 0;
  bool doneHandleWithSpacesFromBeginning = false;
  string newText;
  for (; i < text.size(); ++i) {
    //erase tabs from the beginning
    if (!doneHandleWithSpacesFromBeginning && text[i] == '\t') {
      continue;
    }
    //first char which is not tab
    if (!doneHandleWithSpacesFromBeginning && text[i] != '\t') {
      doneHandleWithSpacesFromBeginning = true;
    }
    newText += text[i];
  }
  //new text without tabs from beginning
  return newText;
}

string ReadData::deleteSpacesFromTheBeginningOfTheLine(string text) {
  unsigned int i = 0;
  bool doneHandleWithSpacesFromBeginning = false;
  string newText;
  for (; i < text.size(); ++i) {
    //erase spaces from the beginning
    if (!doneHandleWithSpacesFromBeginning && text[i] == ' ') {
      continue;
    }
    //first char which is not space
    if (!doneHandleWithSpacesFromBeginning && text[i] != ' ') {
      doneHandleWithSpacesFromBeginning = true;
    }
    newText += text[i];
  }
  //new text without spaces from beginning
  return newText;
}

string ReadData::deleteOpenSpecialParenthesis(string text) {
  unsigned int i = 0;
  string newText;
  for (; i < text.size(); ++i) {
    if (text[i] == '{') {
      break;
    }
    //if there is space before '{'
    if (text[i] == ' ' && text[i + 1] == '{') {
      break;
    }
    newText += text[i];
  }
  return newText;
}

bool ReadData::isThereCloseSpecialParenthesisInText(std::__cxx11::string text) {
  unsigned int i = 0;
  for (; i < text.size(); ++i) {
    if (text[i] == '}') {
      return true;
    }
  }
  return false;
}

bool ReadData::isThereOpenSpecialParenthesisInText(std::__cxx11::string text) {
  unsigned int i = 0;
  for (; i < text.size(); ++i) {
    if (text[i] == '{') {
      return true;
    }
  }
  return false;
}

bool ReadData::isThereEqualSignInText(std::__cxx11::string text) {
  unsigned int i = 0;
  for (; i < text.size(); ++i) {
    if (text[i] == '=') {
      return true;
    }
  }
  return false;
}

string ReadData::deleteSpacesBetweenExpressions(string text) {
  unsigned int i = 0;
  string newText;
  for (; i < text.size(); ++i) {
    if (text[i] != ' ') {
      newText += text[i];
    }
  }
  return newText;
}

void ReadData::orderWithSpaces(string leftTextInLine, unsigned int index) {
  string delimiterSpace = " ";
  string token;
  size_t posOfThisLine;
  //how much spaces do we have in this text
  unsigned int howManySpacesInText = countSpacesInText(leftTextInLine);
  unsigned int i;
  for (i = 0; i < howManySpacesInText; ++i) {
    posOfThisLine = leftTextInLine.find(delimiterSpace);
    token = leftTextInLine.substr(0, posOfThisLine);
    //erasing chars till we have ' '
    leftTextInLine.erase(0, posOfThisLine + delimiterSpace.length());
    this->vectorOfDataFromFile[index].push_back(token);
  }
  //check if we need to do it one more time
  if (!leftTextInLine.empty()) {
    this->vectorOfDataFromFile[index].push_back(leftTextInLine);

  }
}

void ReadData::orderTextWithParenthesis(string rightTextInLine, unsigned int index) {
  string textToVector;
  bool openApostrophes = false;
  unsigned int i = 0;
  while (i != rightTextInLine.size()) {
    if (i + 1 == rightTextInLine.size()) {
      this->vectorOfDataFromFile[index].push_back(textToVector);
      break;
    }
    //delete apostrophes from text
    if (rightTextInLine[i] == '"' && !openApostrophes) {
      openApostrophes = true;
      i++;
      continue;
    }
    //delete spaces from text if it is not string with '"'
    if (rightTextInLine[i] == ' ' && !openApostrophes) {
      i++;
      continue;
    }
    //end of string
    if (rightTextInLine[i] == '"' && openApostrophes) {
      openApostrophes = false;
      this->vectorOfDataFromFile[index].push_back(textToVector);
      textToVector.clear();
      i += 2;
      continue;
    }
    //end of what we need to insert to our vector
    if (rightTextInLine[i + 1] == ',' && !openApostrophes) {
      this->vectorOfDataFromFile[index].push_back(textToVector);
      textToVector.clear();
      i += 2;
      continue;
    }
    textToVector += rightTextInLine[i];
    i++;
  }
}

unsigned int ReadData::countSpacesInText(string text) {
  unsigned int i = 0;
  unsigned int countSpaces = 0;
  for (; i < text.size(); ++i) {
    if (text[i] == ' ') {
      countSpaces++;
    }
  }
  return countSpaces;
}

ReadData::ReadData() {}

ReadData::~ReadData() {

}



