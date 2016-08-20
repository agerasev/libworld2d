#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <map>
#include <cmath>
#include <string>

#include <core/world.hpp>
#include <event.hpp>

#include "item.hpp"

class Scene : public QGraphicsScene {
public:
	std::map<int, Item*> items;
	World *world = nullptr;

	Scene(World *w) : QGraphicsScene() {
		world = w;
		
		setBackgroundBrush(QColor("#FFFFFF"));
		setSceneRect(-world->size.x(), -world->size.y(), 2*world->size.x(), 2*world->size.y());
		addRect(sceneRect(), QPen(), QBrush(QColor("#FFFFCC")));
	}

	virtual ~Scene() {
		for(auto &p : items) {
			delete p.second;
		}
	}
	
	virtual Item *instance(Entity *e) const = 0;
	
	void sync() {
		for(auto &p : items) {
			p.second->exists = false;
		}
		
		world->access.lock();
		{
			for(auto &ep : world->entities) {
				int id = ep.first;
				Entity *e = ep.second;
				Item *it = nullptr;
				auto ii = items.find(id);
				if(ii == items.end()) {
					it = instance(e);
					it->setZValue(id);
					ii = items.insert(std::pair<int, Item*>(id, it)).first;
					addItem(it);
				} else {
					it = ii->second;
				}
				it->sync(e);
				it->exists = true;
			}
		}
		world->access.unlock();
		
		for(auto ii = items.begin(); ii != items.end();) {
			if(ii->second->exists) {
				++ii;
			} else {
				auto iv = ii->second;
				removeItem(iv);
				items.erase(ii++);
				delete iv;
			}
		}
		
		update();
	}
};

class View : public QGraphicsView {
public:
	float zf = 1.25;

	View() : QGraphicsView() {
		setStyleSheet( "border-style: none;");

		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		setDragMode(ScrollHandDrag);
		
		setRenderHint(QPainter::Antialiasing);
	}

	virtual void enterEvent(QEvent *event) override {
		QGraphicsView::enterEvent(event);
		viewport()->setCursor(Qt::ArrowCursor);
	}
	
	virtual void mousePressEvent(QMouseEvent *event) override {
		QGraphicsView::mousePressEvent(event);
		viewport()->setCursor(Qt::ArrowCursor);
	}
	
	virtual void mouseReleaseEvent(QMouseEvent *event) override {
		QGraphicsView::mouseReleaseEvent(event);
		viewport()->setCursor(Qt::ArrowCursor);
	}
	
	virtual void mouseDoubleClickEvent(QMouseEvent *event) override {
		QGraphicsView::mouseDoubleClickEvent(event);
	}
	
	virtual void wheelEvent(QWheelEvent *event) override {
		float z = pow(zf, event->delta()/120.0);
		scale(z,z);
	}
};
