cmake_minimum_required(VERSION 3.15)
project(ex3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

add_executable(ex3 server.cpp client.cpp ex1.cpp ex1.h Expression.h ReadData.cpp ReadData.h Command.h PrintCommand.cpp PrintCommand.h VariablesSingelton.cpp VariablesSingelton.h Var.cpp Var.h SleepCommand.cpp SleepCommand.h VarDefinitionLeft.cpp VarDefinitionLeft.h VarDefinitionRight.cpp VarDefinitionRight.h ConditionParser.cpp ConditionParser.h WhileCommand.cpp WhileCommand.h IfCommand.cpp IfCommand.h VarDefinitionEqual.cpp VarDefinitionEqual.h ParserCommands.cpp ParserCommands.h EqualCommandWithoutVar.cpp EqualCommandWithoutVar.h main.cpp StringToCommands.cpp StringToCommands.h OpenDataServerCommand.cpp OpenDataServerCommand.h ConnectClientCommand.cpp ConnectClientCommand.h UnknownFuncCommand.cpp UnknownFuncCommand.h KnownFuncCommand.cpp KnownFuncCommand.h GlobalConnectFlightGear.h)