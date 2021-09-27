#include "game.h"

Game::Game(sf::RenderWindow* window, sf::View* view) {
	window_ = window;
	view_ = view;

    network_id_ = 0;
    world_ = NULL;
}

Game::~Game() {
    delete world_;
}

bool Game::LoadWorld() {
    if (world_ == NULL) {
        world_ = new World();
        world_->AddPlayer(network_id_, MSVector(400, 250));
        return true;
    }
    return false;
}

void Game::Action(sf::Event::KeyEvent key_event) {
    if (key_event.code == sf::Keyboard::A) {
		std::cout << "A is pushed down!";
	}


}

void Game::Update(float dt) {
    Player* p = world_->GetPlayer(network_id_);
    p->Update(world_->GetObstacles());

    for (auto e : world_->GetEnemies()) {
        e->Update(world_->GetObstacles());
    }
}

void Game::Render() {
    Player* p = world_->GetPlayer(network_id_);

    view_->setCenter(p->GetPosition().x, 400);
    window_->setView(*view_);

    MSVector view_center = view_->getCenter();
    MSVector view_size = view_->getSize();
    MSVector view_position = view_center.minus(view_size.scale(0.5));

    window_->clear(sf::Color(0, 100, 200));

    Background* b = world_->GetBackground();
    b->DrawBackground(window_, view_position, view_->getSize());

    for (auto obj : world_->GetObstacles()) {
        window_->draw(obj->GetSprite());
    }
    for (auto e : world_->GetEnemies()) {
        window_->draw(e->GetSprite());
    }
    
    window_->draw(p->GetSprite());
    
    b->DrawForeground(window_, view_position, view_->getSize());

    window_->display();
}