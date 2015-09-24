/*
 * Items.cpp
 *
 *  Created on: 2015. 1. 2.
 *      Author: EzKorry
 */

#include "Items.h"
#include "SmoothMoveNode.h"
#include "apTouchManager.h"
#include "asPointManager.h"
#include "asPoint.h"
#include "apField.h"
#include "apPoint.h"
#include "GLES-Render.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <sstream>

USING_NS_CC;
using namespace arphomod;

using namespace cocos2d::network;
/*
 *
 *!!!!! template !!!!!

bool [className]::itemInit() {

	return true;
}

std::string [className]::getTitle() const {
	return std::string("TITLE STRING");
}

[className]::~[className]() {

}

 */


TouchManagerTestItem::~TouchManagerTestItem() {

}

bool TouchManagerTestItem::itemInit() {

	auto label = Label::createWithTTF("yeah~", "fonts/Marker Felt.ttf", 30);
	label->setPosition(Vec2(300, 300));
	this->addChild(label);

	auto size = Director::getInstance()->getVisibleSize();
	auto sprite = Sprite::create("ch_front.png");
	sprite->setPosition(size.width / 6, size.height / 2);
	this->addChild(sprite);
	auto tm = getAPTouchManager();
	auto checker = APTouchManager::createCheckerWithRect(sprite, Rect(-50.0f, -50.0f, 100.0f, 100.0f));
	tm->registerNode(sprite,checker);
	tm->addBehavior(sprite, APTouchType::Began, [label]()->void {
		label->setString("Fuck yeah sprite touched!!");
		label->setColor(Color3B(cocos2d::random<int>(128, 255),cocos2d::random<int>(128, 255),cocos2d::random<int>(128, 255)));
	}, "TouchManagerTest");

	auto moveRight = MoveBy::create(2.0f, Vec2(size.width / 4, 0));
	auto moveLeft = MoveBy::create(2.0f, Vec2(-size.width / 4, 0));
	auto seq = Sequence::create(moveRight, moveLeft, NULL);
	auto inf = RepeatForever::create(seq);
	sprite->runAction(inf);

	return true;

}

std::string TouchManagerTestItem::getTitle() const {
	return std::string("This is Touch Manager Test Item");
}

bool SmoothTouchMoveTestItem::itemInit() {

	auto manager = this->getAPTouchManager();
	_bigNode = SmoothMoveNode::create(manager);
	_bigNode->setAnchorPoint(Vec2(0, 0));
	_bigNode->setPosition(Vec2(0, 0));
	this->addChild(_bigNode, 10, "bigNode");

	auto ttfConf = TTFConfig("fonts/NanumBarunGothicLight.ttf", 30);
	_labelForce = Label::createWithTTF(ttfConf, "");
	_labelForce->setPosition(Vec2(300, 30));
	this->addChild(_labelForce);

	_labelAngle = Label::createWithTTF(ttfConf, "");
	_labelAngle->setPosition(Vec2(300, 60));
	this->addChild(_labelAngle);

	schedule(schedule_selector(SmoothTouchMoveTestItem::updateValues));

	return true;
}

void SmoothTouchMoveTestItem::updateValues(float delta) {

	_labelAngle->setString(NumberToString(_bigNode->getAngle()));
	_labelForce->setString(NumberToString(_bigNode->getForceX()));

}
std::string SmoothTouchMoveTestItem::getTitle() const {
	return std::string("Smooth Touch Move Test");
}

SmoothTouchMoveTestItem::~SmoothTouchMoveTestItem() {

}

bool SmoothTouchSelectTestItem::itemInit() {

	return true;
}

std::string SmoothTouchSelectTestItem::getTitle() const {
	return std::string("Smooth Item Select");
}

SmoothTouchSelectTestItem::~SmoothTouchSelectTestItem() {

}

void AWSRankingTestItem::onConnect(cocos2d::network::SIOClient* client) {
	log("testItem Connected!");

}
void AWSRankingTestItem::onMessage(cocos2d::network::SIOClient* client,
		const std::string& data) {
	log("test item onMessage");
}
void AWSRankingTestItem::onClose(cocos2d::network::SIOClient* client) {
	log("test item onclose");

}
void AWSRankingTestItem::onError(cocos2d::network::SIOClient* client,
		const std::string& data) {
	log("testItem onError: %s", data.c_str());

}

bool AWSRankingTestItem::itemInit() {

	auto ttfConf = TTFConfig("fonts/NanumBarunGothicLight.ttf", 50);
	auto labelClick = Label::createWithTTF(ttfConf, "CLICK");
	labelClick->setPosition(Vec2(300, 300));
	this->addChild(labelClick, 3, "labelClick");
	auto ch = APTouchManager::createDefaultChecker(labelClick);
	auto touchManager = getAPTouchManager();
	touchManager->registerNode(labelClick, ch);
	touchManager->addBehavior(labelClick,APTouchType::Began, [this]() {
		_client->emit("chat message","yeah yeah~~");
		log("client emitted!");
	}, "awsRankingButtonPress");

	_client = network::SocketIO::connect(
			"http://ec2-54-65-12-4.ap-northeast-1.compute.amazonaws.com:3000",
			*this);
	//you may set a tag for the client for reference in callbacks
	_client->setTag("Test Client");

	//register event callbacks using the CC_CALLBACK_2() macro and passing the instance of the target class
	/*_client->on("custom-power",[](network::SIOClient* client, const std::string& data) {
	 log("yeah! %s", data.c_str());

	 });*/

	return true;
}

std::string AWSRankingTestItem::getTitle() const {
	return std::string("aws ranking test");

}

AWSRankingTestItem::~AWSRankingTestItem() {

}

bool ZOrderTest::itemInit() {

	auto ttfConf = TTFConfig("fonts/Marker Felt.ttf", 70);
	auto label1 = Label::createWithTTF(ttfConf, "Label1");
	auto label2 = Label::createWithTTF(ttfConf, "Label2");
	auto label3 = Label::createWithTTF(ttfConf, "Label3");
	auto label4 = Label::createWithTTF(ttfConf, "Label4");

	label1->setColor(Color3B(255, 255, 255));
	label2->setColor(Color3B(255, 0, 255));
	label3->setColor(Color3B(255, 255, 0));
	label4->setColor(Color3B(0, 255, 255));

	auto node1 = Node::create();
	auto node2 = Node::create();
	auto node3 = Node::create();
	auto node4 = Node::create();

	node1->addChild(label1);
	node2->addChild(label2);
	node3->addChild(label3);
	node4->addChild(label4);

	node1->setPosition(Vec2(300, 300));
	node2->setPosition(Vec2(0, 30));
	node3->setPosition(Vec2(330, 300));
	node4->setPosition(Vec2(0, 30));

	this->addChild(node1, 20, "label1");
	node1->addChild(node2, 10, "label2");
	this->addChild(node3, 10, "label3");
	node3->addChild(node4, 40, "label4");

	//consquence: when it renders, if a.localZOrder>b.localZOrder which have same parent, a and a's children is in front of b and b's children

	auto cons =
			Label::createWithTTF(
					"when it renders, if a.localZOrder>b.localZOrder which have same parent, a and a's children is in front of b and b's children",
					"fonts/NanumBarunGothicLight.ttf", 30);
	cons->setAlignment(TextHAlignment::LEFT);
	cons->setAnchorPoint(Vec2(0, 0));
	cons->setPosition(Vec2(0, 200));
	this->addChild(cons);

	return true;
}

std::string ZOrderTest::getTitle() const {
	return std::string("ZOrderTest");
}

ZOrderTest::~ZOrderTest() {

}

bool CharacterAndMapTest::itemInit() {

	auto drawNode1 = DrawNode::create();

	this->addChild(drawNode1);

	drawNode1->drawCircle(Vec2(100, 100), 50, CC_DEGREES_TO_RADIANS(90), 30,
			false, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	drawNode1->drawSolidCircle(Vec2(300, 100), 50, CC_DEGREES_TO_RADIANS(90),
			30, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	drawNode1->drawSolidCircle(Vec2(400, 100), 50, CC_DEGREES_TO_RADIANS(80),
			20, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	drawNode1->drawSolidCircle(Vec2(500, 100), 50, CC_DEGREES_TO_RADIANS(70),
			10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	drawNode1->drawSolidCircle(Vec2(600, 100), 50, CC_DEGREES_TO_RADIANS(30),
			25, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	drawNode1->drawSegment(Vec2(300, 200), Vec2(300, 300), 100,
			Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

	return true;
}

std::string CharacterAndMapTest::getTitle() const {
	return std::string("Character And Map Test");
}

CharacterAndMapTest::~CharacterAndMapTest() {

}

bool Box2DTest::itemInit() {

	//////////////////////////////

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//SET MOUSE LISTENER
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	listener->onTouchBegan = CC_CALLBACK_2(Box2DTest::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Box2DTest::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Box2DTest::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//END MOUSE LISTENER

	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	world = std::make_shared<b2World>(gravity);

	debugDraw = new GLESDebugDraw(SCALE_RATIO);
	world->SetDebugDraw(debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	debugDraw->SetFlags(flags);


	//CREATE A BALL
	dragOffsetStartX = 0;
	dragOffsetEndX = 0;
	dragOffsetStartY = 0;
	dragOffsetEndY = 0;
	existBall = false;
	ballX = 500;
	ballY = 200;
	powerMultiplier = 10;
	ball = Sprite::create("ball.png");
	ball->setPosition(CCPoint(ballX, ballY));
	this->addChild(ball);


	//HelloWorld::defineBall();

	addWall(visibleSize.width, 10, (visibleSize.width / 2), 0); //CEIL
	addWall(10, visibleSize.height, 0, (visibleSize.height / 2)); //LEFT
	addWall(10, visibleSize.height, visibleSize.width,
			(visibleSize.height / 2)); //RIGHT

	for (int i = 1; i <= 31; i++) {
		points[i] = CCSprite::create("dot.png");
		this->addChild(points[i]);
	}
	scheduleUpdate();
	return true;

	//b2DebugDraw b;

}

std::string Box2DTest::getTitle() const {
	return std::string("Box2D Testing!!");
}

Box2DTest::~Box2DTest() {

}

void Box2DTest::defineBall() {
	ballShape.m_radius = 45 / SCALE_RATIO;
	b2FixtureDef ballFixture;
	ballFixture.density = 10;
	ballFixture.friction = 0.8;
	ballFixture.restitution = 0.6;
	ballFixture.shape = &ballShape;

	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.userData = ball;

	ballBodyDef.position.Set(ball->getPosition().x / SCALE_RATIO,
			ball->getPosition().y / SCALE_RATIO);

	ballBody = world->CreateBody(&ballBodyDef);
	ballBody->CreateFixture(&ballFixture);
	ballBody->SetGravityScale(10);
}

void Box2DTest::addWall(float w, float h, float px, float py) {
	b2PolygonShape floorShape;

	floorShape.SetAsBox(w / SCALE_RATIO, h / SCALE_RATIO);
	b2FixtureDef floorFixture;
	floorFixture.density = 0;
	floorFixture.friction = 10;
	floorFixture.restitution = 0.5;
	floorFixture.shape = &floorShape;
	b2BodyDef floorBodyDef;

	floorBodyDef.position.Set(px / SCALE_RATIO, py / SCALE_RATIO);
	b2Body *floorBody = world->CreateBody(&floorBodyDef);
	floorBody->CreateFixture(&floorFixture);
}
void Box2DTest::simulateTrajectory(b2Vec2 coord) {
	//define ball physicis
	Box2DTest::defineBall();

	ballBody->SetLinearVelocity(b2Vec2(coord.x, coord.y));
	for (int i = 1; i <= 31; i++) {
		world->Step(deltaTime, 10, 10);
		points[i]->setPosition(
				CCPoint(ballBody->GetPosition().x * SCALE_RATIO,
						ballBody->GetPosition().y * SCALE_RATIO));
		world->ClearForces();

	}

	world->DestroyBody(ballBody);
}

bool Box2DTest::onTouchBegan(Touch* touch, Event* event) {
	dragOffsetStartX = touch->getLocation().x;
	dragOffsetStartY = touch->getLocation().y;

	auto touchLocation = touch->getLocation();

	ballX = touchLocation.x;
	ballY = touchLocation.y;

	if (existBall) {
		world->DestroyBody(ballBody);
	}

	ball->setPosition(Vec2(ballX, ballY));
	return true;
}
void Box2DTest::onTouchMoved(Touch* touch, Event* event) {
	auto touchLocation = touch->getLocation();

	dragOffsetEndX = touchLocation.x;
	dragOffsetEndY = touchLocation.y;

	float dragDistanceX = dragOffsetStartX - dragOffsetEndX;
	float dragDistanceY = dragOffsetStartY - dragOffsetEndY;

	//HelloWorld::simulateTrajectory(b2Vec2((dragDistanceX )/SCALE_RATIO,(dragDistanceY )/SCALE_RATIO));
	Box2DTest::simulateTrajectory(
			b2Vec2((dragDistanceX * powerMultiplier) / SCALE_RATIO,
					(dragDistanceY * powerMultiplier) / SCALE_RATIO));

}
void Box2DTest::onTouchEnded(Touch* touch, Event* event) {
	existBall = true;

	Box2DTest::defineBall();

	CCPoint touchLocation = touch->getLocation();

	dragOffsetEndX = touchLocation.x;
	dragOffsetEndY = touchLocation.y;

	float dragDistanceX = dragOffsetStartX - dragOffsetEndX;
	float dragDistanceY = dragOffsetStartY - dragOffsetEndY;

	//ballBody->SetLinearVelocity(b2Vec2((dragDistanceX)/SCALE_RATIO,(dragDistanceY)/SCALE_RATIO));
	ballBody->SetLinearVelocity(
			b2Vec2((dragDistanceX * powerMultiplier) / SCALE_RATIO,
					(dragDistanceY * powerMultiplier) / SCALE_RATIO));
}

void Box2DTest::update(float dt) {

	int positionIterations = 10;
	int velocityIterations = 10;

	deltaTime = dt;
	world->Step(dt, velocityIterations, positionIterations);

	for (b2Body *body = world->GetBodyList(); body != NULL; body =
			body->GetNext())
		if (body->GetUserData()) {
			Sprite *sprite = (Sprite *) body->GetUserData();
			sprite->setPosition(
					Vec2(body->GetPosition().x * SCALE_RATIO,
							body->GetPosition().y * SCALE_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));

		}
	world->ClearForces();
	//world->DrawDebugData();

}

void Box2DTest::draw(Renderer *renderer, const Mat4& transform, uint32_t transformUpdated)
{
    glEnableVertexAttribArray(0);
    world->DrawDebugData();
}

std::string PathFindingTest::getTitle() const {
	return std::string("path finding!");
	cocos2d::log("getTitle");

}

// initializing from derived class
bool PathFindingTest::itemInit() {

	cocos2d::log("PathFindingTest item init start");

	_stage = Node::create();
	this->addChild(_stage, 0, "stage");

	_pointManager = asPointManager::create();


	int i,j;
	auto touchManager = getAPTouchManager();
	for(i=0;i<5;i++) {
		for(j=0;j<5;j++) {
			auto p = createPoint(_stage, Vec2(300 + 120*i, 100+120*j));
			_points[i*5+j] = p;
			touchManager->registerNode(p, [p](cocos2d::Touch* touch)->bool {
				cocos2d::Rect r(p->getPositionX() - 40, p->getPositionY() - 40, 80, 80);
				return r.containsPoint(touch->getLocation());
			});
			std::stringstream ss;
			ss<<p->_ID;
			ss<<"behavior";
			cocos2d::log("%s",ss.str().c_str());


			touchManager->addBehavior(p, APTouchType::Began, [this, p]()->void {
				if(this->_isStart == true) {
					_startingPoint = p;
					_isStart = false;

				} else {
					_destinationPoint = p;
					_isStart = true;
				}
				resetDrawing();
			},ss.str());

		}
	}

	_pointManager->connectPoint(_points[0], _points[6]);
	_pointManager->connectPoint(_points[2], _points[6]);
	_pointManager->connectPoint(_points[2], _points[8]);
	_pointManager->connectPoint(_points[8], _points[12]);
	_pointManager->connectPoint(_points[11], _points[12]);
	_pointManager->connectPoint(_points[10], _points[11]);
	_pointManager->connectPoint(_points[6], _points[10]);
	_pointManager->connectPoint(_points[11], _points[16]);
	_pointManager->connectPoint(_points[16], _points[21]);
	_pointManager->connectPoint(_points[18], _points[21]);
	_pointManager->connectPoint(_points[18], _points[23]);
	_pointManager->connectPoint(_points[19], _points[23]);
	_pointManager->connectPoint(_points[19], _points[24]);
	_pointManager->connectPoint(_points[13], _points[19]);
	_pointManager->connectPoint(_points[9], _points[13]);
	_pointManager->connectPoint(_points[8], _points[9]);

	_startingPoint = _points[0];
	_destinationPoint = _points[24];
	_drawing = DrawNode::create();
	this->addChild(_drawing, 30, "drawing");

	auto list = _pointManager->findPath(_points[0], _points[18]);
	if(!list.empty()) {
		for(auto& item : list) {
			cocos2d::log("item:%d, (%f, %f)", item, item->getPositionX(), item->getPositionY());
		}
	}



	//createPoint(_stage, Vec2(100,100));
	cocos2d::log("PathFindingTest item init end");
	return true;

}

void PathFindingTest::resetDrawing() {
	_drawing->clear();
	_drawing->drawDot(_startingPoint->getPosition(), 10, Color4F(0, 1, 0, 1));
	_drawing->drawDot(_destinationPoint->getPosition(), 10, Color4F(0,0,1,1));
	auto list = _pointManager->findPath(_startingPoint, _destinationPoint);
	if(list.empty()) return;



	auto it = list.begin();
	for(auto& item : list) {
		_drawing->drawDot(item->getPosition(), 6.0, Color4F(1,0,0,1));
		auto pos = (*it)->getPosition();
		it++;
		if(it != list.end()) {
			_drawing->drawSegment(pos, (*it)->getPosition(), 3, Color4F(1, 0.2, 0.2, 1));
		}



	}

}


asPoint* PathFindingTest::createPoint(cocos2d::Node* parent, cocos2d::Vec2 coordinate) {


	if(parent == nullptr || _pointManager == nullptr) return nullptr;

	auto point = asPoint::create();
	point->setPosition(coordinate);
	parent->addChild(point);
	_pointManager->addPoint(point);
	return point;


}

PathFindingTest::~PathFindingTest() {

}
bool PathFindingTest2::itemInit() {


	//;
	_field = apField::create();
	this->addChild(_field, 0, "field");

	_draw = cocos2d::DrawNode::create();
	this->addChild(_draw, 2, "draw");


	std::vector<apPointPtr> ps {
		apPoint::create(100,100),//starting
		apPoint::create(200,100),//1
		apPoint::create(300,100),
		apPoint::create(200,200),
		apPoint::create(300,200),
		apPoint::create(300,500),//5
		apPoint::create(400,500),
		apPoint::create(400,300),
		apPoint::create(500,500),
		apPoint::create(500,300),
		apPoint::create(600,500),//10
		apPoint::create(500,600),
		apPoint::create(600,400),
		apPoint::create(700,300),
		apPoint::create(600,200),
		apPoint::create(500,200),//15
		apPoint::create(800,600)//end
	};


	std::vector<apPointPtr> t1 {
		ps[1],
		ps[2],
		ps[4],
		ps[3]
	},
	 t2 {
		ps[5],
		ps[7],
		ps[6]
	},
	t3 {
		ps[6],
		ps[7],
		ps[9],
		ps[8]
	},
	t4 {
		ps[8],
		ps[9],
		ps[10]
	},
	t5 {
		ps[11],
		ps[8],
		ps[10]
	},

	t6 {
		ps[12],
		ps[13],
		ps[14],
		ps[15]
	};

	_field->makePolygon(t1);
	_field->makePolygon(t2);
	_field->makePolygon(t3);
	_field->makePolygon(t4);
	_field->makePolygon(t5);
	_field->makePolygon(t6);
	_field->setStart(ps[0]);
	_field->setDestination(ps[16]);
	_field->generatePath();
	_field->drawThings();

	auto path = _field->findPath();


	cocos2d::log("findpath finished");
	for(auto it = path.begin(); it != path.end(); it++) {
		auto dest = std::next(it);
		if (dest != path.end()) {
			_draw->drawSegment(Vec2((*it)->x, (*it)->y),
					Vec2((*dest)->x, (*dest)->y), 2,
					Color4F(153.0 / 255.0, 173.0 / 255.0, 170.0 / 255.0, 1));
		}
	}



	return true;
}

std::string PathFindingTest2::getTitle() const {
	return std::string("path finding depending polygon");
}

PathFindingTest2::~PathFindingTest2() {

}




std::string ComponentTest::getTitle() const {
	return std::string("component Test");
}

bool ComponentTest::itemInit(){

	auto com1 = Component::create();

	return true;

}
ComponentTest::~ComponentTest() {

}

bool MoveControllerTest::itemInit() {

	return true;
}

std::string MoveControllerTest::getTitle() const {
	return std::string("ControllerTest");
}

MoveControllerTest::~MoveControllerTest() {

}


