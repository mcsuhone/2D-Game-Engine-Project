#pragma once

#include "livingobject.h"

static Stats slime_stats = Stats(10.f, 1.f);

class Slime : public LivingObject {
public:
	Slime() = default;

	Slime(MSVector pos);

	~Slime() = default;

	void Behaviour();
private:

};