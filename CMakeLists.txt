cmake_minimum_required(VERSION 3.9)
project(assembler)

set(CMAKE_CXX_STANDARD 11)

add_executable(assembler main.cpp FileReader.cpp FileReader.h LoopManager.cpp LoopManager.h CommandIdentifier.cpp CommandIdentifier.h OperationInfo.cpp OperationInfo.h CommandParser.cpp CommandParser.h Command.cpp Command.h PrimaryData.cpp PrimaryData.h cmake-build-debug/main.cpp cmake-build-debug/main.h main2.cpp main2.h)