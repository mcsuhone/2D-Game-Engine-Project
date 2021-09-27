#include "action.h"



Action::Action(int type) {
	prop_ = action_list.at(SLASH);
	type_ = type;
	timer_.restart();
	animated_ = false;
}