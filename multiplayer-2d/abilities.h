#pragma once

#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "slash.h"



class Abilities {
public:
	Abilities() = default;

	~Abilities() = default;

	void UseAbility(sf::Keyboard::Key key);

private:
	std::map<sf::Keyboard::Key, Action> current_abilities_;
};