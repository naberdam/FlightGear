//
// Created by nerya on 17/12/2019.
//

#include <iostream>
#include "ReadData.h"
#include "ParserCommands.h"

#include <sstream>

bool isThisTxtFile(const string txtFile) {
  unsigned int i = txtFile.size() - 1;
  string endText;
  while (txtFile[i] != '.') {
    endText += txtFile[i];
    --i;
  }
  return !endText.compare("txt");
}

int main(int argc, char *argv[]) {
  ReadData temp;
  //check if we have enough argv
  try {
    if (argc < 1) {
      throw "error with input main";
    }
  } catch (...) {
    cout << "error with input main" << endl;
    return 0;
  }
  //it does not end with '.txt'
  try {
    if (!isThisTxtFile(argv[1])) {
      throw "error with name of fileText";
    }
  } catch (...) {
    cout << "error with name of fileText" << endl;
    return 0;
  }
  //start parsing the commands in file text
  try {
    vector<vector<string>> vec = temp.lexer(argv[1]);
    ParserCommands parserCommands = ParserCommands();
    parserCommands.parser(vec);
  } catch (char const *exc1) {
    cout << exc1 << endl;
  }

  return 0;
}

