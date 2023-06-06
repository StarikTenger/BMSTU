flex -o lex.yy.cpp lexer.l
g++ -o lexer *.cpp
./lexer < input.txt
rm *.cpp