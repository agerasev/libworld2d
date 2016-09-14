#pragma once

#include <la/vec.hpp>


class Entity {
public:
	bool active = false;
	bool interactive = true;
	
	double mass = 1.0;
	
	vec2 pos = vec2(0,0);
	vec2 vel = vec2(0,0);
	vec2 acc = vec2(0,0);
	
	virtual double size() const {
		return sqrt(mass);
	}
	
	Entity() = default;
	virtual ~Entity() = default;
	
	virtual void move(double dt) {
		vel += acc*dt;
		pos += vel*dt;
		acc = vec2(0,0);
	}
};
