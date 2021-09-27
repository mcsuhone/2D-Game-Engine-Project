#include "slime.h"



Slime::Slime(MSVector pos) : LivingObject(pos, "textures/slime", slime_stats) {
    SetMaxVelocity(MSVector(0.5f, 10.f));
}

void Slime::Behaviour() {
    MSVector velocity = GetVelocity();
    if (InAir()) {
        Fall();
        Decelerate(0.9);
    }
    else {
        if (abs(velocity.x) == 0.5f) {
            Accelerate(MSVector(0.f, -2.f));
        }
        else if (CanMove(Direction::RIGHT)) {
            Accelerate(MSVector(0.005, 0.f));
        }
        else if (CanMove(Direction::LEFT)) {
            Accelerate(MSVector(-0.005, 0.f));
        }
    }
}