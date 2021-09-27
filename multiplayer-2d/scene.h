#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "client.h"
#include "options.h"

enum class ProgramState {
	GAME,
	MENU
};

class Scene {
public:
	Scene(bool server_state);

	~Scene();

	void MainLoop();

	/* Called only from main!! */
	void EventPoller();
	void SetState(ProgramState state) { state_ = state; }

private:
	void GameLoop();
	void MenuLoop();
	
	sf::RenderWindow* window_;
	sf::View* view_;
	Options options_;
	ProgramState state_;
	bool server_state_;

	Game* game_;
	Client* client_;
};