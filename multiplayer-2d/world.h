#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "block.h"
#include "slime.h"
#include "background.h"

class World {
public:
	World();

	~World();

	std::vector<Block*> GetObstacles() { return obstacles_; }

	std::vector<LivingObject*> GetEnemies() { return enemies_; }

	std::map<int, Player*> GetPlayers() { return players_; }

	Player* GetPlayer(int network_id) { return players_[network_id]; }

	Background* GetBackground() { return background_; }

	bool AddPlayer(int network_id, MSVector position);

private:
	std::vector<Block*> obstacles_;
	std::vector<LivingObject*> enemies_;
	std::map<int, Player*> players_;

	Background* background_;
};