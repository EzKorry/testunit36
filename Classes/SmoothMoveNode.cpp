/*
 * SmoothMoveNode.cpp
 *
 *  Created on: 2015. 1. 18.
 *      Author: EzKorry
 */

#include "SmoothMoveNode.h"
#include "apTouchManager.h"

USING_NS_CC;
using namespace arphomod;
SmoothMoveNode::SmoothMoveNode() {
	// TODO Auto-generated constructor stub

}

SmoothMoveNode::~SmoothMoveNode() {
	// TODO Auto-generated destructor stub
}

bool SmoothMoveNode::init(std::shared_ptr<APTouchManager> manager) {

	if(!Node::init()) return false;
	_manager = manager;

	auto size = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	_manager->registerNode(this,
			[size, origin](Touch* touch)->bool {
				auto location = touch->getLocation();
				auto rect = cocos2d::Rect(origin.x, origin.y, origin.x + size.width, origin.y + size.height);
				return rect.containsPoint(location);
			});

	_manager->setMaxTouch(this, 1);

	auto& angle = _angle;
	//auto& force = _force;
	auto& touchedCoordinates = _touchedCoordinates;

	_manager->addBehavior(this, APTouchType::Began, [this,&touchedCoordinates](){

		auto touch = _manager->getTouch(this);

		if(isScheduled(schedule_selector(SmoothMoveNode::slide))) {
			unschedule(schedule_selector(SmoothMoveNode::slide));
		}
		auto location = touch->getLocation();
		touchedCoordinates.clear();
		for(int i=0;i<4;i++){
			touchedCoordinates.emplace_back(location);
		}

		if(!isScheduled(schedule_selector(SmoothMoveNode::moving))) {
			schedule(schedule_selector(SmoothMoveNode::moving));
		}
		/*if(isScheduled(schedule_selector(SmoothMoveNode::stopSlide))) {
			unschedule(schedule_selector(SmoothMoveNode::stopSlide));
			scheduleOnce(schedule_selector(SmoothMoveNode::stopSlide), 0);
		}*/

		_startingPoint = getPosition();
		_startingTouchPoint = touch->getLocation();



	}, "smoothBegan");

	_manager->setBehavior(this,APTouchType::MovedInside,
			[this/*&angle, &force,*/, &touchedCoordinates]() {

				auto touch = _manager->getTouch(this);
				_touchedCoordinates.emplace_back(touch->getLocation());
				_touchedCoordinates.pop_front();

				setPosition(_startingPoint + touch->getLocation() - _startingTouchPoint );

			}, "smoothMoveInside");

	_manager->setBehavior(this, APTouchType::EndedIn,
			[this]() {


				auto end = _touchedCoordinates.end();
				auto angleX = (*(std::prev(end, 1))).x - (*(std::prev(end, 4))).x;
				auto angleY = (*(std::prev(end, 1))).y - (*(std::prev(end, 4))).y;


				// moved pixel per seconds.
				_forceX = ((*(std::prev(end, 1))).x - (*(std::prev(end, 3))).x)/2/**30*/;
				_forceY = ((*(std::prev(end, 1))).y - (*(std::prev(end, 3))).y)/2/**30*/;

				_angle = std::atan2(angleY, angleX);

				_force = std::sqrt(
						(_forceX * _forceX) +
						(_forceY * _forceY)
				);


				_slideOrigin = getPosition();
				_slidingTime = 0.0;

				if(isScheduled(schedule_selector(SmoothMoveNode::moving))) {
					unschedule(schedule_selector(SmoothMoveNode::moving));
				}
				if(!isScheduled(schedule_selector(SmoothMoveNode::slide))) {
					schedule(schedule_selector(SmoothMoveNode::slide));
				}



				//scheduleOnce(schedule_selector(SmoothMoveNode::stopSlide), std::abs((_force * 60) / _resistance));



			}, "smoothEndIn");


	auto label = Label::createWithTTF("power!!", "fonts/NanumBarunGothicLight.ttf", 70);
	label->setPosition(Vec2(500,500));
	this->addChild(label, 0, "label");
	return true;


}

float SmoothMoveNode::getForceX() const{
	return _forceX;
}

float SmoothMoveNode::getForceY() const {
	return _forceY;
}

float SmoothMoveNode::getAngle() const{
	return _angle;
}

void SmoothMoveNode::moving(float delta) {



}

void SmoothMoveNode::slide(float delta) {
	/*
	_slidingTime += delta;
	 auto distanceX = _slidingTime * (_slidingTime * -_resistance +_forceX);
	 setPosition(_slideOrigin.x + distanceX,_slideOrigin.y);
*/
	/*
	setPosition(getPositionX() + _forceX, getPositionY() + _forceY);
	_forceX -= _resistance * delta;
	_forceY -= _resistance * delta;
	*/

	auto x = getPositionX() + std::cos(_angle) * _force;
	auto y = getPositionY() + std::sin(_angle) * _force;
	setPosition(x, y);
	_force -= _resistance * delta;
	if (_force < 0 && isScheduled(schedule_selector(SmoothMoveNode::slide))) {

		unschedule(schedule_selector(SmoothMoveNode::slide));

	}


}

void SmoothMoveNode::stopSlide(float delta) {
	if(isScheduled(schedule_selector(SmoothMoveNode::slide))) {
		unschedule(schedule_selector(SmoothMoveNode::slide));
	}
}
