#include "livingobject.h"

LivingObject::LivingObject(MSVector pos, std::string texture_file, Stats stats) : MovingObject(pos, texture_file) {
	stats_ = stats;
    action_queue_ = {};
}

void LivingObject::Update(std::vector<Block*> obstacles) {
    // Set previous state to current state, current state will be then modified.
    SetPreviousState(GetCurrentState());
    
    /* These are defined in subclasses. */
    Behaviour();
    UpdateState();
    DoMove();

    /* Get current state AFTER the behaviour updates */
    ObjectState current_state = GetCurrentState();

    float first_collision_distance = 1000.f;
    int index = -1;
    int i = 0;

    sf::FloatRect hitbox = GetHitbox().getGlobalBounds();

    sf::RectangleShape left = GetHitbox(); left.move(-1.f, 0.f); sf::FloatRect left_hitbox = left.getGlobalBounds();
    sf::RectangleShape right = GetHitbox(); right.move(1.f, 0.f); sf::FloatRect right_hitbox = right.getGlobalBounds();
    sf::RectangleShape under = GetHitbox(); under.move(0.f, 1.f); sf::FloatRect under_hitbox = under.getGlobalBounds();
    sf::RectangleShape top = GetHitbox(); top.move(0.f, -3.f); sf::FloatRect top_hitbox = top.getGlobalBounds();
    bool left_intersect = false;
    bool right_intersect = false;
    bool under_intersect = false;
    bool top_intersect = false;

    /* Find first collision */
    for (auto obstacle : obstacles) {
        sf::FloatRect obstacle_hitbox = obstacle->GetHitbox().getGlobalBounds();

        if (hitbox.intersects(obstacle_hitbox)) {
            if (GetPosition().distance(obstacle->GetPosition()) < first_collision_distance) {
                index = i;
            }
        }
        if (left_hitbox.intersects(obstacle_hitbox)) {
            left_intersect = true;
        }
        if (right_hitbox.intersects(obstacle_hitbox)) {
            right_intersect = true;
        }
        if (under_hitbox.intersects(obstacle_hitbox)) {
            under_intersect = true;
        }
        if (top_hitbox.intersects(obstacle_hitbox)) {
            top_intersect = true;
        }
        i++;
    }

    // These could be done with functions also ??
    if (left_intersect) {
        current_state.can_move_left = false;
    }
    else {
        current_state.can_move_left = true;
    }
    if (right_intersect) {
        current_state.can_move_right = false;
    }
    else {
        current_state.can_move_right = true;
    }
    if (under_intersect) {
        current_state.in_air = false;
    }
    else {
        current_state.in_air = true;
    }
    if (top_intersect) {
        current_state.can_move_up = false;
    }
    else {
        current_state.can_move_up = true;
    }

    /* If collision was found, create manifold. */
    if (index != -1) {
        MSVector m = CreateManifold(obstacles.at(index));

        if (m.y < 0) {
            current_state.velocity.y = 0;
        }
        else if (m.y > 0) {
            current_state.velocity.y = 0;
            current_state.can_move_up = false;
        }
        else if (m.x > 0) {
            current_state.velocity.x = 0;
        }
        else if (m.x < 0) {
            current_state.velocity.x = 0;
        }
    }

    // Make function out of this?
    ObjectState new_state = { current_state.position, current_state.velocity , current_state.in_air, current_state.can_move_left, current_state.can_move_right, current_state.can_move_up, current_state.is_facing_left };
    SetCurrentState(new_state);

    /* Clear old actions */

    std::vector<Action*> action_queue = GetActionQueue();

    if (!action_queue.empty()) {
        if (action_queue.at(0)->HasEnded()) {
            UpdateActionQueue();
        }
    }

    /* Animation updates */

    std::vector<Action*> action_queue2 = GetActionQueue();

    if (!action_queue2.empty()) {
        Action* front = action_queue2.front();
        if (!front->Animated()) {
            Animator()->PlayAnimation(front->GetAnimation());
        }
    }

    if (Turned()) {
        Animator()->Flip();
    }
    if (Jumped()) {
        Animator()->PlayAnimation(JUMP);
    }
    else if (current_state.in_air) {
        Animator()->UpdateAnimationState(IN_AIR);
    }
    else {
        if (IsIdle()) {
            Animator()->UpdateAnimationState(IDLE);
        }
        else {
            Animator()->UpdateAnimationState(MOVING);
        }
    }
}

void LivingObject::StartAction(int type) {
    if (CanAct()) {
        action_queue_.push_back(new Action(type));
    }
}

void LivingObject::UpdateActionQueue() {
    if (action_queue_.size() > 1) {
        Action* act = action_queue_.at(1);
        action_queue_.clear();
        action_queue_.push_back(act);
        action_queue_.at(0)->Start();
    }
    else {
        action_queue_.clear();
    }
}