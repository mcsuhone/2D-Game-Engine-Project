#include "world.h"

World::World() {
    for (int i = -25; i < 25; i++) {
        obstacles_.push_back(new Block(MSVector(i * 64, 500), "textures/GrassBrick2.png"));
    }
    obstacles_.push_back(new Block(MSVector(3 * 64, 500 - 64), "textures/GrassBrick2.png"));
    obstacles_.push_back(new Block(MSVector(1 * 64, 500 - 64 * 2), "textures/GrassBrick2.png"));
    obstacles_.push_back(new Block(MSVector(10 * 64, 500 - 64), "textures/GrassBrick2.png"));

    background_ = new Background("textures/background_forest");

    Slime* slime = new Slime(MSVector(600.f, 250.f));
    enemies_.push_back(slime);
}

World::~World() {
    for (auto obj : obstacles_) {
        delete obj;
    }
    for (auto i : players_) {
        delete i.second;
    }

    delete background_;
}

bool World::AddPlayer(int network_id, MSVector position) {
    if (players_.find(network_id) == players_.end()) {
        players_[network_id] = new Player(position, "textures/player");
        return true;
    }
    return false;
}