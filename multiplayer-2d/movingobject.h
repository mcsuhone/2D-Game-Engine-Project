#pragma once

#include <vector>
#include "block.h"
#include "action.h"


struct ObjectState {
	MSVector position;
	MSVector velocity;
	bool in_air;
	bool can_move_left;
	bool can_move_right;
	bool can_move_up;
	bool is_facing_left;
};

struct Manifold {
	float penetration_depth;
	MSVector normal;
};

enum class Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

static float GRAVITY = 0.07f;

class MovingObject : public Object {
public:
	/* Default constructor */
	MovingObject() = default;
	/* Constructor */
	MovingObject(MSVector pos, std::string texture_file);

	~MovingObject() = default;
protected:
	ObjectState GetCurrentState() { return current_state_; }
	ObjectState GetPreviousState() { return previous_state_; }
	MSVector GetVelocity() { return current_state_.velocity; }

	void SetCurrentState(ObjectState state) { current_state_ = state; }
	void SetPreviousState(ObjectState state) { previous_state_ = state; }
	/* Sets max velocity and makes sure it is positive vector. */
	void SetMaxVelocity(MSVector velocity);

	/* Accelerate by a vector. Doesn't accelerate over max speed, if set. */
	void Accelerate(MSVector acceleration);

	/* Decelerate by an amount (0.0-1.0). 0 is instant stop, 1 does nothing. */
	void Decelerate(float amount);

	/* Applies gravitational acceleration. */
	void Fall(float float_amount = 0.f);

	/* Returns true if object is in air. */
	bool InAir() { return current_state_.in_air; }

	/* Returns true if object can move in the spesified direction. */
	bool CanMove(Direction direction);

	/* Returns true if object faces left. */
	bool IsFacingLeft() { return current_state_.is_facing_left; }

	/* Makes object move by velocity. */
	void DoMove();

	bool Turned() { if (current_state_.is_facing_left != previous_state_.is_facing_left) return true; else return false; }
	bool IsIdle() { if (current_state_.velocity.x == 0) return true; else return false; }
	bool Jumped() { if (!previous_state_.in_air && current_state_.in_air && current_state_.velocity.y < 0) return true; else return false; }

	/* Handles input, collisions and movement of the object */
	virtual void Update(std::vector<Block*> obstacles) = 0;
	
	/* Updates all states. Mainly face direction. */
	void UpdateState();

	MSVector CreateManifold(Block* other);
private:
	bool HasMaxVelocity();

	ObjectState current_state_ = { MSVector(), MSVector(), true, true, true, true, true };
	ObjectState previous_state_ = { MSVector(), MSVector(), true, true, true, true, true };
	MSVector max_velocity_;
};