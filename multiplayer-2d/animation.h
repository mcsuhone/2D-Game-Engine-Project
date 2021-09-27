#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "msvector.h"
#include "functions.h"

enum AnimationType {
	IDLE,
	MOVING,
	IN_AIR,
	JUMP,
	ATTACK
};


class Animation {
public:
	Animation() = default;

	Animation(std::string file, bool animated);

	~Animation() {
		delete(texture_);
	}

	void UpdateAnimationState(int state);

	void Flip();

	void PlayAnimation(int state);

	void SetPosition(MSVector pos);

	sf::Sprite GetSprite();
	sf::Sprite* GetSpritePointer() { return &sprite_; }
	MSVector GetSize() { return size_; }
	MSVector GetHitboxSize() { return hitbox_; }
	MSVector GetOffset() { return offset_; }

private:
	void ReadFiles(std::string file);
	void NextFrame();
	void SetRect();

	sf::Texture* texture_;
	sf::Sprite sprite_;

	bool animated_;
	bool flipped_;
	bool instant_update_;
	std::pair<bool, int> playing_animation_;

	int animation_state_;
	int current_frame_;

	std::vector<int> last_frames_;
	MSVector size_;
	MSVector hitbox_;
	MSVector offset_;
	std::map<int, int> delays_;

	sf::Clock timer_;
};