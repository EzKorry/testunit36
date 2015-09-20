/*
 * Items.h
 *
 *  Created on: 2015. 1. 2.
 *      Author: EzKorry
 */

#ifndef ITEMS_H_
#define ITEMS_H_
#define SCALE_RATIO 32.0

#include "BaseItem.h"
#include "network/SocketIO.h"
#include "Box2D/Box2D.h"
#include "apField.h"

USING_NS_CC;


class SmoothMoveNode;
class asPointManager;
class asPoint;
class GLESDebugDraw;
using arphomod::apField;
class TouchManagerTestItem: public BaseItem {
public:

	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	virtual ~TouchManagerTestItem();
};

class SmoothTouchMoveTestItem: public BaseItem {
public:

	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	void updateValues(float delta);

	virtual ~SmoothTouchMoveTestItem();

private:

	SmoothMoveNode* _bigNode;
	cocos2d::Label* _labelForce;
	cocos2d::Label* _labelAngle;
};

class SmoothTouchSelectTestItem: public BaseItem {
public:

	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	virtual ~SmoothTouchSelectTestItem();
};

class AWSRankingTestItem: public BaseItem, public cocos2d::network::SocketIO::SIODelegate {
public:

	virtual void onConnect(cocos2d::network::SIOClient* client) override;
	virtual void onMessage(cocos2d::network::SIOClient* client, const std::string& data) override;
	virtual void onClose(cocos2d::network::SIOClient* client) override;
	virtual void onError(cocos2d::network::SIOClient* client, const std::string& data) override;

	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	virtual ~AWSRankingTestItem();
private:

	cocos2d::network::SIOClient* _client{ nullptr};
};
class ZOrderTest: public BaseItem {
public:

	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	virtual ~ZOrderTest();
};


class CharacterAndMapTest : public BaseItem {
public:
	virtual std::string getTitle() const override;
	virtual bool itemInit() override;
	virtual ~CharacterAndMapTest();
};

class Box2DTest : public BaseItem, public b2ContactListener {
public:
	virtual std::string getTitle() const override;
	virtual bool itemInit() override;
	virtual ~Box2DTest();

	std::shared_ptr<b2World> world;
	float deltaTime;

	Sprite *ball;
	bool existBall;
	float ballX;
	float ballY;
	int dragOffsetStartX;
	int dragOffsetEndX;
	int dragOffsetStartY;
	int dragOffsetEndY;
	float powerMultiplier;
	b2Body *ballBody;
	b2CircleShape ballShape;
	b2BodyDef ballBodyDef;
	GLESDebugDraw *debugDraw;
	void defineBall();
	void draw(Renderer *renderer, const Mat4& transform, uint32_t transformUpdated) override;

	Sprite *points[32];

	void addWall(float w, float h, float px, float py);
	void simulateTrajectory(b2Vec2 coord);

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	void update(float dt);

};


class PathFindingTest: public BaseItem {
public:

	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	virtual ~PathFindingTest();

	asPoint* createPoint(cocos2d::Node* parent, cocos2d::Vec2 coordinate);

	void resetDrawing();

private:
	cocos2d::Node* _stage{nullptr};
	std::shared_ptr<asPointManager> _pointManager{nullptr};
	std::array<asPoint*, 25> _points;
	cocos2d::DrawNode* _drawing{nullptr};
	bool _isStart{false};
	asPoint* _startingPoint{nullptr};
	asPoint* _destinationPoint{nullptr};
};

class PathFindingTest2: public BaseItem {
public:
	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	virtual ~PathFindingTest2();

private:
	apField* _field { nullptr };
	cocos2d::DrawNode* _draw { nullptr };
};


class ComponentTest: public BaseItem {
public:
	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	virtual ~ComponentTest();

private:
};

class MoveControllerTest: public BaseItem {
public:
	virtual std::string getTitle() const override;

	// initializing from derived class
	virtual bool itemInit() override;

	virtual ~MoveControllerTest();

private:
};




#endif /* ITEMS_H_ */
