#!/bin/bash

mkdir build
cp lexer.l build/lexer.l 
cp parser.y build/parser.y
cp driver.hpp build/driver.hpp
cp driver.cpp build/driver.cpp
cp main.cpp build/main.cpp
cp CMakeLists.txt build/CMakeLists.txt
cd build
lex -o lexer.cpp lexer.l
bison -o parser.cpp -d parser.y
cmake .
make

rm lexer.l
rm parser.y
rm driver.hpp 
rm driver.cpp
rm main.cpp
rm CMakeLists.txt

./formater ../input.txt > ../out.txt

#g++ -o calc *.cpp
#rm -f lexer.&pp parser.?pp
#./calc  input.txt
