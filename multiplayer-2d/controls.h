#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <map>

class Controls {
public:
	Controls() = default;
	~Controls() = default;

	void CheckKeysPressed();

private:
	std::map<int, bool> keys_pressed_;
};