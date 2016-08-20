#pragma once

#include <QGraphicsItem>

#include <la/vec.hpp>

#include <core/entity.hpp>


QPointF v2q(const vec2 &v) {
	return QPointF(v.x(), v.y());
}

QColor qmix(const QColor &a, const QColor &b, double r = 0.5) {
	return QColor(
		a.red()*r + b.red()*(1 - r),
		a.green()*r + b.green()*(1 - r),
		a.blue()*r + b.blue()*(1 - r),
		255
	);
}


class Item : public QGraphicsItem {
public:
	constexpr static const char *COLOR = "#888888";
	
	bool exists = true;
	
	double size = 20.0;
	
	QColor color;
	
	Item(Entity *e) : QGraphicsItem() {
		color = QColor(COLOR);
	}
	virtual ~Item() = default;
	
	virtual QRectF boundingRect() const override {
		return QRectF(-size, -size, 2*size, 2*size);
	}
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override {
		if(size > 0.0) {
			QColor dark_color = qmix(color, QColor("#000000"), 0.75);
			QPen pen;
			pen.setWidth(2);
			pen.setCosmetic(true);
			pen.setColor(dark_color);
			painter->setPen(pen);
			
			painter->setBrush(color);
			painter->drawEllipse(boundingRect());
		}
	}
	
	virtual void sync(Entity *e) {
		setPos(v2q(e->pos));
		size = e->size();
		// update();
	}
};
