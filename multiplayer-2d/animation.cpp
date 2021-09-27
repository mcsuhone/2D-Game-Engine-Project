#include "animation.h"

Animation::Animation(std::string file, bool animated) {
	animated_ = animated;
	flipped_ = false;
	instant_update_ = false;
	playing_animation_ = std::pair<bool, int>(false, 0);

	if (animated) {
		ReadFiles(file);

		current_frame_ = 0;
		animation_state_ = 0;

		sprite_.setTextureRect(sf::IntRect(current_frame_ * size_.x, animation_state_ * size_.y, size_.x, size_.y));
	}
	else {
		texture_ = new sf::Texture();
		if (!texture_->loadFromFile(file)) {
			std::cout << "Error while loading texture from " << file << "\n";
		}
		sprite_.setTexture(*texture_);

		animation_state_ = 0;
		current_frame_ = 0;

		last_frames_ = { 0 };
		size_ = MSVector(texture_->getSize().x, texture_->getSize().y);
		hitbox_ = size_;
		offset_ = MSVector();
		delays_[0] = 1000;
	}
}


void Animation::ReadFiles(std::string file) {
	/* Set defaults */
	size_ = MSVector(32, 32);
	hitbox_ = MSVector(32, 32);
	offset_ = MSVector();
	

	/* Read file */
	std::string info_file_name = file + "/info.txt";
	std::string sprite_file_name = file + "/sprite_sheet.png";

	std::string current_line;
	std::fstream info_file(info_file_name);

	Functions f;

	int animation_count = -1;

	while (std::getline(info_file, current_line)) {
		if (current_line != "") {
			if (current_line.at(0) == '#') {
				animation_count++;
			}
			else {
				std::vector<std::string> vec = f.SplitString(current_line, '=');

				if (vec.size() >= 2) {
					std::string type = vec.at(0);

					if (type == "size") {
						std::vector<std::string> eq = f.SplitString(vec.at(1), ',');
						int x = std::stoi(eq.at(0));
						int y = std::stoi(eq.at(1));
						size_ = MSVector(x, y);
					}
					else if (type == "hitbox") {
						std::vector<std::string> eq = f.SplitString(vec.at(1), ',');
						int x = std::stoi(eq.at(0));
						int y = std::stoi(eq.at(1));
						hitbox_ = MSVector(x, y);
					}
					else if (type == "offset") {
						std::vector<std::string> eq = f.SplitString(vec.at(1), ',');
						int x = std::stoi(eq.at(0));
						int y = std::stoi(eq.at(1));
						offset_ = MSVector(x, y);
					}
					else if (type == "frame_count") {
						int frames = std::stoi(vec.at(1));
						last_frames_.push_back(frames);
					}
					else if (type == "delay") {
						int delay = std::stoi(vec.at(1));
						delays_[animation_count] = delay;
					}
				}
			}
		}
	}

	if (delays_.empty()) {
		std::cout << "Loading of delays failed.\n";
		delays_[0] = 2000;
	}
	if (last_frames_.empty()) {
		std::cout << "Loading of last_frames.\n";
		last_frames_.push_back(1);
	}

	texture_ = new sf::Texture();
	if (!texture_->loadFromFile(sprite_file_name)) {
		std::cout << "Error while loading texture from " << file << "\n";
	}
	sprite_.setTexture(*texture_);
}

void Animation::UpdateAnimationState(int state) {
	if (last_frames_.size() > state) {
		if (animation_state_ != state && !playing_animation_.first) {
			animation_state_ = state;
			current_frame_ = 0;
			instant_update_ = true;
			SetRect();
		}
		else if (animation_state_ != state) {
			animation_state_ = state;
		}
	}
}

void Animation::Flip() {
	instant_update_ = true;
	if (flipped_) {
		flipped_ = false;
	}
	else {
		flipped_ = true;
	}
}

void Animation::PlayAnimation(int state) {
	playing_animation_ = std::pair<bool, int>(true, state);
	current_frame_ = 0;
	instant_update_ = true;
}

void Animation::SetPosition(MSVector pos) {
	sprite_.setPosition(pos.x - offset_.x, pos.y - offset_.y);
}

sf::Sprite Animation::GetSprite() {
	if (animated_) {
		int delay = 100;
		if (playing_animation_.first) {
			delay = delays_[playing_animation_.second];
		}
		else {
			delay = delays_[animation_state_];
		}
		if (timer_.getElapsedTime().asMilliseconds() > delay || instant_update_) {
			if (instant_update_) {
				instant_update_ = false;
			}
			NextFrame();
			timer_.restart();
		}

		return sprite_;
	}
	return sprite_;
}

void Animation::NextFrame() {
	int last_frame = 0;
	if (playing_animation_.first) {
		if (last_frames_.size() > playing_animation_.second) {
			last_frame = last_frames_.at(playing_animation_.second);
		}
		else {
			last_frame = last_frames_.at(last_frames_.size() - 1);
		}
	}
	else {
		if (last_frames_.size() > animation_state_) {
			last_frame = last_frames_.at(animation_state_);
		}
		else {
			last_frame = last_frames_.at(last_frames_.size() - 1);
		}
	}
	if (current_frame_ == last_frame) {
		/* Last frame */
		current_frame_ = 0;
		if (playing_animation_.first) {
			playing_animation_ = std::pair<bool, int>(false, 0);
		}
		SetRect();
	}
	else {
		SetRect();
		current_frame_++;
	}
}

void Animation::SetRect() {
	int animation_state = 0;
	
	if (playing_animation_.first) {
		animation_state = playing_animation_.second;
	}
	else {
		animation_state = animation_state_;
	}

	if (flipped_) {
		sprite_.setTextureRect(sf::IntRect((current_frame_ * size_.x) + size_.x, animation_state * size_.y, -size_.x, size_.y));
	}
	else {
		sprite_.setTextureRect(sf::IntRect(current_frame_ * size_.x, animation_state * size_.y, size_.x, size_.y));
	}
}

