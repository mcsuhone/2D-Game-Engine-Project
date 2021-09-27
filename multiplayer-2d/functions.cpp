#include "functions.h"


std::vector<std::string> Functions::SplitString(std::string str, char character) {
	std::vector<std::string> vec;
	std::stringstream ss(str);
	std::string temp;

	while (std::getline(ss, temp, character)) {
		vec.push_back(temp);
	}
	return vec;
}
