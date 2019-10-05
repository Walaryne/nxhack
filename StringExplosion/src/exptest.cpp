#include <string>

#include <vector>
#include <iostream>
#include "stringexplosion.hpp"

int main() {
	std::string h = "hello how are you";
	std::vector<std::string> exploded;

	se::exploder exp(h ,' ', true);

	try {
		exploded = exp.explode();
	} catch (se::exploderex &e) {
		std::cerr << "Error: " << e << std::endl;
	}

	for(std::string s : exploded) {
		std::cout << s << std::endl;
	}

	return 0;
}
