#pragma once

#include <vector>
#include "world.h"


class Game {
public:
	Game() = default;

	Game(sf::RenderWindow* window, sf::View* view);

	~Game();

	World* GetWorld() { return world_; }
	void SetNetworkID(int id) { network_id_ = id; }

	bool LoadWorld();

	void Action(sf::Event::KeyEvent key_event);

	void Update(float dt);

	void Render();

private:
	sf::RenderWindow* window_;
	sf::View* view_;

	World* world_;
	int network_id_;
};