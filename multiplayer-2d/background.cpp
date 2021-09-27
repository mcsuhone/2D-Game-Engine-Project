#include "background.h"


Background::Background(std::string file) {
	LoadFiles(file);
	previous_view_position_ = MSVector();
}

Background::~Background() {
	for (auto t : background_textures_) {
		delete t;
	}

	for (auto t : foreground_textures_) {
		delete t;
	}

	for (auto pair : background_sprites_) {
		delete pair.first;
	}

	for (auto pair : foreground_sprites_) {
		delete pair.first;
	}
}

void Background::LoadFiles(std::string file) {
	/* Read file */
	std::string info_file_name = file + "/info.txt";
	std::string texture_file_name = file + "/layer_";

	std::string current_line;
	int index = 0;
	std::fstream info_file(info_file_name);

	Functions f;

	while (std::getline(info_file, current_line)) {
		if (current_line != "") {
			std::vector<std::string> vec = f.SplitString(current_line, '=');

			if (vec.size() >= 3) {
				std::string texture_name = texture_file_name + std::to_string(index) + ".png";

				sf::Texture* texture = new sf::Texture();
				sf::Sprite* sprite = new sf::Sprite();

				if (!texture->loadFromFile(texture_name)) {
					std::cout << "Error while loading texture from " << file << "\n";
				}
				sprite->setTexture(*texture);

				sprite->setScale(MSVector(0.685f, 0.685f));

				sprite->setPosition(MSVector(-(size_.x * 0.685), 0));

				if (vec.at(1) == "b") {
					background_textures_.push_back(texture);
					std::pair<sf::Sprite*, float> pair(sprite, std::stof(vec.at(2)));
					background_sprites_.push_back(pair);
				}
				else if (vec.at(1) == "f") {
					sprite->move(MSVector(0.f, 10.f));
					foreground_textures_.push_back(texture);
					std::pair<sf::Sprite*, float> pair(sprite, std::stof(vec.at(2)));
					foreground_sprites_.push_back(pair);
				}

				index++;
			}
		}
	}
	if (!background_textures_.empty()) {
		sf::Vector2u vec = background_textures_.at(0)->getSize();
		size_ = MSVector(vec.x, vec.y);
	}
	else {
		size_ = MSVector(32, 32);
	}
}

int IsOutsideOfView(sf::Sprite* sprite, MSVector view_position, MSVector view_size) {
	MSVector sprite_pos = sprite->getPosition();
	sf::Vector2u sprite_size = sprite->getTexture()->getSize();
	if (sprite_pos.x + sprite_size.x < view_position.x) {
		return -1;
	}
	else if (sprite_pos.x > view_position.x + view_size.x) {
		return 1;
	}
	return 0;
}

void Background::DrawBackground(sf::RenderWindow* window, MSVector view_position, MSVector view_size) {

	float x_travel = view_position.x - previous_view_position_.x;

	for (auto pair : background_sprites_) {
		sf::Sprite* sprite = pair.first;

		int sprite_state = IsOutsideOfView(sprite, view_position, view_size);
		if (sprite_state == -1) {
			sprite->move(MSVector(size_.x * 0.685, 0));
		}
		if (sprite_state == 1) {
			sprite->move(MSVector(-size_.x * 0.685, 0));
		}

		sprite->move(x_travel * pair.second, 0);
		window->draw(*sprite);

		sprite->move(MSVector(-size_.x * 0.685, 0));
		window->draw(*sprite);
		sprite->move(MSVector(-size_.x * 0.685, 0));
		window->draw(*sprite);
		sprite->move(MSVector(3 * size_.x * 0.685, 0));
		window->draw(*sprite);
		sprite->move(MSVector(size_.x * 0.685, 0));
		window->draw(*sprite);

		sprite->move(MSVector(-2 * size_.x * 0.685, 0));
	}
}

void Background::DrawForeground(sf::RenderWindow* window, MSVector view_position, MSVector view_size) {
	float x_travel = view_position.x - previous_view_position_.x;

	for (auto pair : foreground_sprites_) {
		sf::Sprite* sprite = pair.first;

		int sprite_state = IsOutsideOfView(sprite, view_position, view_size);
		if (sprite_state == -1) {
			sprite->move(MSVector(size_.x * 0.685, 0));
		}
		if (sprite_state == 1) {
			sprite->move(MSVector(-size_.x * 0.685, 0));
		}

		sprite->move(x_travel * pair.second, 0);
		window->draw(*sprite);

		sprite->move(MSVector(-size_.x * 0.685, 0));
		window->draw(*sprite);
		sprite->move(MSVector(-size_.x * 0.685, 0));
		window->draw(*sprite);
		sprite->move(MSVector(3 * size_.x * 0.685, 0));
		window->draw(*sprite);
		sprite->move(MSVector(size_.x * 0.685, 0));
		window->draw(*sprite);

		sprite->move(MSVector(-2 * size_.x * 0.685, 0));
	}

	previous_view_position_ = view_position;
}