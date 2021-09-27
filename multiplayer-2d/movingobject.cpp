#include "movingobject.h"

MovingObject::MovingObject(MSVector pos, std::string texture_file) : Object(pos, texture_file, true) {
	current_state_ = { pos, MSVector(0.f, 0.f), true, true, true, true, true };
	previous_state_ = current_state_;
	max_velocity_ = MSVector();
}

void MovingObject::SetMaxVelocity(MSVector velocity) {
	max_velocity_ = MSVector(abs(velocity.x), abs(velocity.y));
}

void MovingObject::Accelerate(MSVector acceleration) {
	if (HasMaxVelocity()) {
		if (abs(current_state_.velocity.x + acceleration.x) < max_velocity_.x) {
			current_state_.velocity.x += acceleration.x;
		}
		else {
			if (current_state_.velocity.x < 0) {
				current_state_.velocity.x = -(max_velocity_.x);
			}
			else if (current_state_.velocity.x > 0) {
				current_state_.velocity.x = max_velocity_.x;
			}
		}
		if (abs(current_state_.velocity.y + acceleration.y) < max_velocity_.y) {
			current_state_.velocity.y += acceleration.y;
		}
		else {
			if (current_state_.velocity.y < 0) {
				current_state_.velocity.y = -(max_velocity_.y);
			}
			else if (current_state_.velocity.y > 0) {
				current_state_.velocity.y = max_velocity_.y;
			}
		}
	}
	else {
		current_state_.velocity += acceleration;
	}
}

void MovingObject::Decelerate(float amount) {
	if (amount >= 0.f && amount <= 1.f) {
		if (abs(current_state_.velocity.x) < 0.01) {
			current_state_.velocity.x = 0;
		}
		current_state_.velocity.x = current_state_.velocity.x * amount;
	}
}

void MovingObject::Fall(float float_amount) {
	if (current_state_.in_air) {
		Accelerate(MSVector(0.f, GRAVITY - float_amount));
	}
}

bool MovingObject::CanMove(Direction direction) {
	if (direction == Direction::LEFT) { return current_state_.can_move_left; }
	else if (direction == Direction::RIGHT) { return current_state_.can_move_right; }
	else if (direction == Direction::UP) { return current_state_.can_move_up; }
	else { return false; }
}

void MovingObject::DoMove() {
	Move(current_state_.velocity);
	current_state_.position = GetPosition();
}

void MovingObject::UpdateState() {
	if (current_state_.velocity.x < 0) {
		current_state_.is_facing_left = false;
	}
	else if (current_state_.velocity.x > 0) {
		current_state_.is_facing_left = true;
	}
}

MSVector MovingObject::CreateManifold(Block* other) {
	
	MSVector self_position = GetPosition();
	MSVector other_position = other->GetPosition();

	MSVector self_size = GetSize();
	MSVector self_half_size = self_size.scale(0.5);
	MSVector other_size = other->GetSize();
	MSVector other_half_size = other_size.scale(0.5);

	MSVector self_center_position = self_position.plus(self_half_size);
	MSVector other_center_position = other_position.plus(other_half_size);

	float x_penetration = 0.f;
	float y_penetration = 0.f;
	float x_safe = self_position.x;
	float y_safe = self_position.y;

	
	if (other_center_position.x > self_center_position.x) {
		x_penetration = (self_center_position.x + self_half_size.x) - (other_center_position.x - other_half_size.x);
		x_safe = other_center_position.x - other_half_size.x - self_half_size.x * 2;
	}
	
	else {
		x_penetration = (other_center_position.x + other_half_size.x) - (self_center_position.x - self_half_size.x);
		x_safe = other_center_position.x + other_half_size.x;
	}
	/* Lands on ground */
	if (other_center_position.y > self_center_position.y) {
		y_penetration = (self_center_position.y + self_half_size.y) - (other_center_position.y - other_half_size.y);
		y_safe = other_center_position.y - other_half_size.y - self_half_size.y * 2;
	}
	/* Hits head */
	else {
		y_penetration = (other_center_position.y + other_half_size.y) - (self_center_position.y - self_half_size.y);
		y_safe = other_center_position.y + other_half_size.y;
	}

	MSVector prev_center_position = GetPreviousState().position.plus(self_half_size);
	MSVector prev_min_position = prev_center_position.minus(self_half_size);
	MSVector prev_max_position = prev_center_position.plus(self_half_size);

	bool above = false;
	bool left_side = false;
	bool right_side = false;
	bool below = false;

	if (prev_max_position.y < other_center_position.y - other_half_size.y) {
		above = true;
	}
	if (prev_min_position.y > other_center_position.y + other_half_size.y) {
		below = true;
	}
	if (prev_max_position.x < other_center_position.x - other_half_size.x) {
		left_side = true;
	}
	if (prev_min_position.x > other_center_position.x + other_half_size.x) {
		right_side = true;
	}

	if (above && left_side) {
		if (x_penetration >= y_penetration) {
			SetPosition(MSVector(self_position.x, y_safe));
			return MSVector(0.f, -1.f);
		}
		else {
			SetPosition(MSVector(x_safe, self_position.y));
			return MSVector(-1.f, 0.f);
		}
	}
	if (above && right_side) {
		if (x_penetration >= y_penetration) {
			SetPosition(MSVector(self_position.x, y_safe));
			return MSVector(0.f, -1.f);
		}
		else {
			SetPosition(MSVector(x_safe, self_position.y));
			return MSVector(1.f, 0.f);
		}
	}
	if (below && left_side) {
		if (x_penetration >= y_penetration) {
			SetPosition(MSVector(self_position.x, y_safe));
			return MSVector(0.f, 1.f);
		}
		else {
			SetPosition(MSVector(x_safe, self_position.y));
			return MSVector(-1.f, 0.f);
		}
	}
	if (below && right_side) {
		if (x_penetration >= y_penetration) {
			SetPosition(MSVector(self_position.x, y_safe));
			return MSVector(0.f, 1.f);
		}
		else {
			SetPosition(MSVector(x_safe, self_position.y));
			return MSVector(1.f, 0.f);
		}
	}

	if (above) {
		SetPosition(MSVector(self_position.x, y_safe));
		return MSVector(0.f, -1.f);
	}
	if (below) {
		SetPosition(MSVector(self_position.x, y_safe));
		return MSVector(0.f, 1.f);
	}
	if (left_side) {
		SetPosition(MSVector(x_safe, self_position.y));
		return MSVector(-1.f, 0.f);
	}
	if (right_side) {
		SetPosition(MSVector(x_safe, self_position.y));
		return MSVector(1.f, 0.f);
	}
	return MSVector();
}

bool MovingObject::HasMaxVelocity() {
	return (max_velocity_.x != 0 && max_velocity_.y != 0);
}
