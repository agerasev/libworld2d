#pragma once

#include <la/vec.hpp>


class Entity {
public:
	bool active = false;
	bool interactive = true;
	
	double mass = 1.0;
	vec2 pos = nullvec2;
	vec2 vel = nullvec2;
	
	virtual double size() const = 0;
	
	Entity() = default;
	virtual ~Entity() = default;
	
	virtual void interact(Entity *e) {}
	
	virtual void move(double dt) {
		pos += vel*dt;
	}
};
