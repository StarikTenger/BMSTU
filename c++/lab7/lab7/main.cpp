#include <iostream>
#include "StackRational.h"

int main() {
	StackRational stack;
	stack.push({1, 1});
	stack.push({2, 1});
	stack.push({3, 1});
	std::cout << stack.size() << "\n";
	std::cout << stack[0].nom << "\n";
	std::cout << stack[1].nom << "\n";
	std::cout << stack[2].nom << "\n";
	stack.pop();
	std::cout << stack.size() << "\n";
	std::cout << stack[0].nom << "\n";
}