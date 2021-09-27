#include "object.h"



Object::Object(MSVector pos, std::string texture_file, bool animated) {
	animation_ = new Animation(texture_file, animated);
	animation_->SetPosition(pos);

	hitbox_ = Hitbox(animation_->GetHitboxSize());
	hitbox_.SetPosition(pos);
}