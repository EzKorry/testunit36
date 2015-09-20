/*
 * SmoothMoveNode.h
 *
 *  Created on: 2015. 1. 18.
 *      Author: EzKorry
 */

#ifndef SMOOTHMOVENODE_H_
#define SMOOTHMOVENODE_H_

#include <cocos2d.h>
class APTouchManager;
class SmoothMoveNode: public cocos2d::Node {
public:
	SmoothMoveNode();
	virtual ~SmoothMoveNode();

	static SmoothMoveNode* create(std::shared_ptr<APTouchManager> manager)
	{
		SmoothMoveNode *pRet = new SmoothMoveNode();
	    if (pRet && pRet->init(manager))
	    {
	        pRet->autorelease();
	        return pRet;
	    }
	    else
	    {
	        delete pRet;
	        pRet = NULL;
	        return NULL;
	    }
	}

	virtual bool init(std::shared_ptr<APTouchManager> manager);

	float getForceX() const;

	float getForceY() const;

	float getAngle() const;

private:

	void slide(float delta);
	void moving(float delta);
	void stopSlide(float delta);

	std::list<cocos2d::Vec2> _touchedCoordinates;
	std::list<float> _movingSec;
	float _forceX{0.0};
	float _forceY{0.0};
	float _force{0.0};
	float _angle{0.0};
	bool _deeebug{true};
	std::shared_ptr<APTouchManager> _manager{nullptr};
	float _slidingTime{0.0};
	float _resistance{20};
	cocos2d::Vec2 _slideOrigin{cocos2d::Vec2::ZERO};

	cocos2d::Vec2 _startingPoint{cocos2d::Vec2::ZERO};
	cocos2d::Vec2 _startingTouchPoint{cocos2d::Vec2::ZERO};
};





#endif /* SMOOTHMOVENODE_H_ */
