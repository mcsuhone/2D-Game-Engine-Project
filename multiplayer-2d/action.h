#pragma once

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "animation.h"

enum ActionType {
	SLASH
};

struct ActionProperties {
	int duration;
	int animation;
};

const static ActionProperties slash = { 280, ATTACK };

const static std::map<int, ActionProperties> action_list = { {0, slash} };

class Action {
public:
	Action() = default;

	Action(int type);

	void Start() { timer_.restart(); }

	bool HasEnded() { if (timer_.getElapsedTime().asMilliseconds() > prop_.duration) return true; else return false; }

	bool Animated() { if (!animated_) { animated_ = true; return false; } else return true; }

	int GetType() { return type_; }

	int GetAnimation() { return prop_.animation; }

private:
	ActionProperties prop_;
	int type_;
	bool animated_;
	sf::Clock timer_;
};