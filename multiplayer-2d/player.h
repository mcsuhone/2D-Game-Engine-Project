#pragma once

#include <map>
#include "livingobject.h"
#include "stats.h"

/*
enum class Actions {
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};*/

class Player : public LivingObject {
public:
	/* Default constructor */
	Player() = default;

	/* Constructor */
	Player(MSVector pos, std::string texture_file, Stats stats = Stats());

	/* Destructor */
	~Player() = default;

	void Behaviour();

	//void Action(sf::Keyboard::Key key_pressed);

private:
	sf::Clock timer_;
	bool timer_start_;
};