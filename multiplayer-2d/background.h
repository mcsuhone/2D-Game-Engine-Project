#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "functions.h"
#include "msvector.h"


class Background {
public:
	Background() = default;

	Background(std::string file);

	~Background();

	void DrawBackground(sf::RenderWindow* window, MSVector view_position, MSVector view_size);

	void DrawForeground(sf::RenderWindow* window, MSVector view_position, MSVector view_size);

private:
	void LoadFiles(std::string file);

	std::vector<sf::Texture*> background_textures_;
	std::vector<std::pair<sf::Sprite*, float>> background_sprites_;

	std::vector<sf::Texture*> foreground_textures_;
	std::vector<std::pair<sf::Sprite*, float>> foreground_sprites_;

	MSVector size_;
	MSVector previous_view_position_;
};