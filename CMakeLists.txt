cmake_minimum_required(VERSION 3.9)
project(assembler)

set(CMAKE_CXX_STANDARD 11)

add_executable(assembler main.cpp FileReader.cpp FileReader.h PassOneManager.cpp PassOneManager.h CommandIdentifier.cpp CommandIdentifier.h OperationInfo.cpp OperationInfo.h CommandParser.cpp CommandParser.h Command.cpp Command.h PrimaryData.cpp PrimaryData.h labelInfo.h FileWriter.cpp FileWriter.h ErrorMsg.h ErrorMsg.cpp PassTwoManager.cpp PassTwoManager.h ExpressionEvaluator.cpp ExpressionEvaluator.h)