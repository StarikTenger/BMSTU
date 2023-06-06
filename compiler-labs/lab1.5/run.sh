flex -o lex.yy.cpp lexer.l
g++ -std=c++17 -o lexer *.cpp
./lexer < input.txt
rm *.cpp