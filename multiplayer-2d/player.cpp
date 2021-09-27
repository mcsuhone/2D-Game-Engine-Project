#include "player.h"

Player::Player(MSVector pos, std::string texture_file, Stats stats) : LivingObject(pos, texture_file, stats) {
    timer_start_ = false;
    SetMaxVelocity(MSVector(1.5f, 10.f));
}

void Player::Behaviour() {
    MSVector velocity = GetVelocity();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && CanMove(Direction::LEFT)) {
        if (velocity.x > 0) { // These could use IsFacing??
            Decelerate(0);
        }
        Accelerate(MSVector(-0.07f, 0.f));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && CanMove(Direction::RIGHT)) {
        if (velocity.x < 0) {
            Decelerate(0);
        }
        Accelerate(MSVector(0.07f, 0.f));
    }
    else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))) {
        Decelerate(0.91);
    }

    if (InAir() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && velocity.y < 0 && CanMove(Direction::UP)) {
        Fall(0.02);
    }
    else if (InAir()) {
        Fall();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && CanMove(Direction::UP) && CanAct()) {
        Accelerate(MSVector(0.f, -3.f)); // Is jump function needed here?
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        StartAction(ActionType::SLASH);
    }
}

/*
void Player::Action(sf::Keyboard::Key key_pressed) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && CanMove(Direction::LEFT)) {
        actions_[Actions::MOVE_LEFT] = true;
    }
    else {
        actions_[Actions::MOVE_LEFT] = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && CanMove(Direction::RIGHT)) {
        actions_[Actions::MOVE_RIGHT] = true;
    }
    else {
        actions_[Actions::MOVE_RIGHT] = false;
    }
}*/