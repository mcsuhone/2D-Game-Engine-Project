#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "animation.h"
#include "hitbox.h"
#include "msvector.h"

class Object {
public:
	/* Default constructor*/
	Object() = default;
	/* Constructor */
	Object(MSVector pos, std::string texture_file, bool animated);

	~Object() {
		delete animation_;
	}

	MSVector GetPosition() { return hitbox_.GetPosition(); }

	MSVector GetSize() { return hitbox_.GetSize(); }

	sf::RectangleShape GetHitbox() { return hitbox_; }
	
	void SetPosition(MSVector pos) { animation_->SetPosition(pos); hitbox_.SetPosition(pos); }

	void Move(MSVector vec) { animation_->GetSpritePointer()->move(vec); hitbox_.Move(vec); }

	Animation* Animator() { return animation_; }

	sf::Sprite GetSprite() { return animation_->GetSprite(); }

private:
	Animation* animation_;
	Hitbox hitbox_;
};