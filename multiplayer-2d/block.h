#pragma once

#include "object.h"

class Block : public Object {
public:
	/* Default constructor*/
	Block() = default;
	/* Constructor */
	Block(MSVector pos, std::string texture_file);
};