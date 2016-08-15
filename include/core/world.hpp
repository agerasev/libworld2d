#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <cmath>

#include <la/vec.hpp>
#include <la/mat.hpp>

//#include "random.hpp"
#include <core/entity.hpp>

class World {
public:
	long id_counter = 1;
	
	std::map<long, Entity*> entities;
	std::mutex access;
	std::function<void(void)> sync;
	
	bool done = false;
	bool paused = false;
	
	vec2 size;
	
	int delay = 40000; // us
	double step_duration = 0.0; // ms
	long steps_elapsed = 0;
	
	void setDelay(int ms) {
		delay = ms;
	}
	
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
	
	virtual void sense(Entity *e) = 0;
	
	void operator()() {
		auto last_draw_time = std::chrono::steady_clock::now();
		while(!done) {
			if(paused) {
				std::this_thread::sleep_for(std::chrono::milliseconds(40));
				continue;
			}
			
			auto begin_time = std::chrono::steady_clock::now();
			access.lock();
			{
				// interact
				for(auto &p : entities) {
					Entity *e = p.second;
					if(e->active) {
						for(auto &op : entities) {
							Entity *oe = op.second;
							if(length(e->pos - oe->pos) < e->size() + oe->size()) {
								e->interact(oe);
							}
						}
					}
				}
				
				// sense
				for(auto &p : entities) {
					sense(p.second);
				}
				
				// process
				for(auto &p : entities) {
					Entity *entity = p.second;
					entity->process();
				}
				
				// remove dead
				for(auto ii = entities.begin(); ii != entities.end();) {
					Entity *e = ii->second;
					if(e->alive) {
						++ii;
					} else {
						entities.erase(ii++);
						delete e;
					}
				}
				
				// reproduce
				for(auto &p : entities) {
					Entity *e = p.second;
					std::list<Entity*> prod = e->produce();
					for(Entity *ne : prod) {
						add(ne);
					}
				}
				
				// move entities
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
			access.unlock();
			
			auto current_time = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_draw_time);
			if(duration >= std::chrono::milliseconds(20)) {
				sync();
				last_draw_time = current_time;
			}
			step_duration = 1e-3*std::chrono::duration_cast<std::chrono::microseconds>(current_time - begin_time).count();
			
			steps_elapsed += 1;
			
			std::this_thread::sleep_for(std::chrono::microseconds(delay));
		}
	}
};
