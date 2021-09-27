#pragma once



class Stats {
public:
	Stats() = default;
	Stats(float hp, float attack);
	~Stats() = default;

	float GetHitpoints() { return max_hitpoints_; }
	float GetAttackDamage() { return attack_damage_; }

private:
	float max_hitpoints_;
	float attack_damage_;
};