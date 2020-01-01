//
// Created by nerya on 17/12/2019.
//

#ifndef EX3_PARSERCOMMANDS_H
#define EX3_PARSERCOMMANDS_H

#include <unordered_map>
#include <string>
#include "Command.h"

using namespace std;

class ParserCommands {

 public:
  ParserCommands();
  void parser(vector<vector<string>> lexerVector);
};

#endif //EX3_PARSERCOMMANDS_H
