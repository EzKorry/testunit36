/*
 * asPoint.cpp
 *
 *  Created on: 2015. 4. 12.
 *      Author: EzKorry
 */

#include "asPoint.h"

USING_NS_CC;

asPoint::asPoint() {
	// TODO Auto-generated constructor stub


}

asPoint::~asPoint() {
	// TODO Auto-generated destructor stub
}

bool asPoint::init() {
	auto dot = DrawNode::create();
	dot->drawDot(Vec2(0,0), 10, Color4F(1.0,1.0,1.0,1.0));
	this->addChild(dot);


	return true;
}

void asPoint::drawRoute(asPoint* p) {
	auto drawnode = DrawNode::create();
	auto end = this->convertToNodeSpace(p->getPosition());

	drawnode->drawLine(Vec2::ZERO, end, Color4F(1.0,1.0,1.0,1.0));
	this->addChild(drawnode);
	_drawing.emplace(p, drawnode);

}
