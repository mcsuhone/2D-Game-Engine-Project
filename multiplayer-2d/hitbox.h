#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "msvector.h"

class Hitbox : public sf::RectangleShape {
public:
	Hitbox() = default;

	Hitbox(MSVector size);

	void SetPosition(MSVector pos);

	void Move(MSVector pos);

	MSVector GetSize() { return MSVector(getSize().x, getSize().y); }

	MSVector GetPosition() { return MSVector(getPosition().x, getPosition().y); }

private:
	MSVector offset_;
};