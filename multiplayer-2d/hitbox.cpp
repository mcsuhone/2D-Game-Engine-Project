#include "hitbox.h"



Hitbox::Hitbox(MSVector size) : sf::RectangleShape(size) {}

void Hitbox::SetPosition(MSVector pos) {
	setPosition(pos);
}

void Hitbox::Move(MSVector vec) {
	move(vec);
}