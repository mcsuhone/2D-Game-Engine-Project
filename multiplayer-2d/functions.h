#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <string>


class Functions {
public:
	Functions() = default;

	std::vector<std::string> SplitString(std::string str, char character);
};