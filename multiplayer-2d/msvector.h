#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

class MSVector : public sf::Vector2f {
public:
	MSVector() : sf::Vector2f(0.f, 0.f) { }

	MSVector(float x, float y) : sf::Vector2f(x, y) { }

	MSVector(sf::Vector2f vec) : sf::Vector2f(vec) { }

	float length();

	float distance(MSVector vec);

	MSVector plus(MSVector other);

	MSVector minus(MSVector other);

	MSVector scale(float amount);
};

