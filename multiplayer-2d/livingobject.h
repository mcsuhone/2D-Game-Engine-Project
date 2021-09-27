#pragma once

#include "movingobject.h"
#include "stats.h"


class LivingObject : public MovingObject {
public:
	LivingObject() = default;

	LivingObject(MSVector pos, std::string texture_file, Stats stats);

	~LivingObject() {
		for (auto a : action_queue_) {
			delete a;
		}
	}

	void Update(std::vector<Block*> obstacles);

protected:
	virtual void Behaviour() = 0;

	void StartAction(int type);
	
	bool CanAct() { if (action_queue_.size() <= 0) return true; else return false; }

	std::vector<Action*> GetActionQueue() { return action_queue_; }
	
	void UpdateActionQueue();

private:
	Stats stats_;
	std::vector<Action*> action_queue_;
};


