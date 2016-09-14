#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <cmath>

#include <la/vec.hpp>
#include <la/mat.hpp>

#include "entity.hpp"


class World {
public:
	int id_counter = 1;
	
	std::map<int, Entity*> entities;
	std::mutex access;
	
	bool done = false;
	bool paused = false;
	
	vec2 size;
	
	double delay = 40.0; // ms
	double step_duration = 0.0; // ms
	int steps_elapsed = 0;
	
	World(const vec2 &s) {
		size = s;
	}
	virtual ~World() {
		for(auto &p : entities) {
			delete p.second;
		}
	}
	
	void add(Entity *e) {
		entities.insert(std::pair<int, Entity*>(id_counter++, e));
	}
	
	/*
	void interact() {
		for(auto &p : entities) {
			Entity *e = p.second;
			if(e->active) {
				for(auto &op : entities) {
					Entity *oe = op.second;
					if(oe->interactive && length(e->pos - oe->pos) < e->size() + oe->size()) {
						e->interact(oe);
					}
				}
			}
		}
	}
	
	void move() {
		double dt = 1e-1;
		for(auto &p : entities) {
			Entity *e = p.second;
			e->move(dt);
			
			vec2 msize = size - vec2(e->size(), e->size());
			if(e->pos.x() < -msize.x()) {
				e->pos.x() = -msize.x();
			} else if(e->pos.x() > msize.x()) {
				e->pos.x() = msize.x();
			}
			if(e->pos.y() < -msize.y()) {
				e->pos.y() = -msize.y();
			} else if(e->pos.y() > msize.y()) {
				e->pos.y() = msize.y();
			}
		}
	}
	
	virtual void step() = 0;
	*/
	
	void operator()() {
		while(!done) {
			if(paused) {
				std::this_thread::sleep_for(std::chrono::milliseconds(40));
				continue;
			}
			
			auto begin_time = std::chrono::steady_clock::now();
			access.lock();
			{
				// step();
			}
			access.unlock();
			auto end_time = std::chrono::steady_clock::now();
			
			step_duration = 1e-3*std::chrono::duration_cast<std::chrono::microseconds>(end_time - begin_time).count();
			
			steps_elapsed += 1;
			
			std::this_thread::sleep_for(std::chrono::microseconds(int(1e3*delay)));
		}
	}
};
