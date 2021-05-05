#include <iostream>
#include "StackRational.h"

int main() {
	StackRational stack;
	stack.push({1, 1});
	stack.push({2, 1});
	stack.push({3, 1});
	StackRational stack_new;
	
	std::cout << stack.size() << " - size\n";
	std::cout << stack[0].nom << " [0]\n";
	std::cout << stack[1].nom << " [1]\n";
	std::cout << stack[2].nom << " [2]\n";
	stack.pop();
	std::cout << stack.size() << " - size\n";
	std::cout << stack[0].nom << " [0]\n";

}