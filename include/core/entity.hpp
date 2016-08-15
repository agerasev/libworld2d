#pragma once

#include <cmath>
#include <list>

#include <la/vec.hpp>


class Entity {
public:
	double score = 0.0;
	bool alive = true;
	bool active = false;
	
	double mass = 1.0;
	vec2 pos = nullvec2;
	vec2 vel = nullvec2;
	
	long total_age = 0;
	int age = 0, anc = 0;
	
	virtual double size() const {
		return 0.5*sqrt(score);
	}
	
	Entity() = default;
	virtual ~Entity() = default;
	
	virtual void interact(Entity *e) = 0;
	
	virtual void process() {
		age += 1;
		total_age += 1;
	}
	
	virtual std::list<Entity*> produce() = 0;
	
	virtual void move(double dt) {
		pos += vel*dt;
	}
};
