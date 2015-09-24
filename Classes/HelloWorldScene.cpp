#include "HelloWorldScene.h"
#include "apTouchManager.h"
#include "BaseItem.h"
#include "Items.h"
#include <sstream>

USING_NS_CC;
using namespace arphomod;
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    //this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    
    _apTouchManager = APTouchManager::getInstance();

	_main = Node::create();
	this->addChild(_main, 0, "main");


    auto square = Sprite::create("square.png");
    square->setPosition(Vec2(400,400));
    this->addChild(square, 2, "square");

    auto stateLabel = Label::createWithTTF("","fonts/Marker Felt.ttf", 24);
    stateLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2 + stateLabel->getContentSize().height));
    this->addChild(stateLabel, 2, "stateLabel");


    auto defaultChecker = APTouchManager::createDefaultChecker(square);
	_apTouchManager->registerNode(square, defaultChecker);
	_apTouchManager->addBehavior(square,APTouchType::Began,
			[stateLabel]()->void {
				cocos2d::log("begand!!!");
				stateLabel->setString("began");
			}, "squareBegan","o");

	_apTouchManager->addBehavior(square,APTouchType::MovedOuter,
			[stateLabel]()->void {
				cocos2d::log("movedinside yeah");
				stateLabel->setString("moved");
			}, "squareMoveInside","o");
	_apTouchManager->addBehavior(square,APTouchType::MovedInner,
			[stateLabel]()->void {
				cocos2d::log("movedinner13848");
				stateLabel->setString("moved");
			}, "squareMoveInner", "o");
	_apTouchManager->addBehavior(square,APTouchType::MovedInnerIgnoreBegan,
			[stateLabel]()->void {
				cocos2d::log("movedoutside1992");
				stateLabel->setString("moved");
			}, "squareMoveInnerNoBegan", "o");
	_apTouchManager->addBehavior(square, APTouchType::MovedOuterIgnoreBegan,
			[stateLabel]()->void {
				cocos2d::log("movedOuterIgnoreBegan");
				stateLabel->setString("moved");
			},"wquareMoveOuter", "o");

	_apTouchManager->addBehavior(square, APTouchType::EndedIn,
			[stateLabel]()->void {
				cocos2d::log("Endedin656");
				stateLabel->setString("ended");
			}, "squareEndIn");

	_apTouchManager->addBehavior(square, APTouchType::EndedOut,
			[stateLabel]()->void {
				cocos2d::log("EndedOut###!!");
				stateLabel->setString("cancelled");
			}, "squareEndOut");
	_apTouchManager->addBehavior(square, APTouchType::EndedInIgnoreBegan,
			[stateLabel]()->void {
				cocos2d::log("EndedOut###!!");
				stateLabel->setString("cancelled");
			}, "squareEndInNoBegan");



	/* Up Down slide
	 *
	 */
	auto emptyNode = Node::create();
	this->addChild(emptyNode);
	_apTouchManager->registerNode(emptyNode, [](cocos2d::Touch* touch)->bool {
		return true;
	});
	_apTouchManager->setOrder(emptyNode, -100);
	_apTouchManager->addBehavior(emptyNode, APTouchType::Began, [this, emptyNode]()->void {
		_startingPoint = _main->getPosition();
		auto touch = _apTouchManager->getTouch(emptyNode);
		_startingTouchPoint = touch->getLocation();
	} ,"emptyNodeBegan");
	_apTouchManager->addBehavior(emptyNode, APTouchType::MovedInside, [this, emptyNode]()->void {
		auto touch = _apTouchManager->getTouch(emptyNode);
		_main->setPosition(_startingPoint + touch->getLocation() - _startingTouchPoint );
	}, "emptyNodeMoved");



	registerScene<TouchManagerTestItem>("Touch Manager Test");
	registerScene<SmoothTouchMoveTestItem>("Smooth touch Move Test");
	registerScene<ZOrderTest>("Z Order testing");
	registerScene<AWSRankingTestItem>("AWS Socket.io TEST");
	registerScene<CharacterAndMapTest>("draw Test");
	registerScene<Box2DTest>("box2d test");
	registerScene<PathFindingTest>("pathFinding test");
	registerScene<PathFindingTest2>("pathfinding polygon");
	registerScene<MoveControllerTest>("moveControllerTest");

	//log("after RegisterScene");

    return true;
}

template <typename T, typename>
void HelloWorld::registerScene(std::string title) {


    	//_itemOrder.insert(std::pair< BaseItem*,int>(item, _order));


    	auto label = Label::createWithTTF(title, "fonts/NanumBarunGothicLight.ttf", 40);

    	auto visibleSize = Director::getInstance()->getVisibleSize();
    	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50 - (_order * 100)));
    	_main->addChild(label, 30, title);

    	auto defaultChecker = APTouchManager::createDefaultChecker(label);

    	T* item{nullptr};
    	auto tempAp = _apTouchManager;
    	stringstream ss;
    	ss << label->getString() << "Behavior";

    	_apTouchManager->registerNode(label, defaultChecker);
    	_apTouchManager->addBehavior(label, APTouchType::Began,
    			[item, tempAp]()->void  {
    		auto scene = BaseItem::create<typename std::remove_pointer<typename std::decay<decltype(item)>::type>::type>(tempAp);
    		cocos2d::Director::getInstance()->pushScene(scene);
    		//cocos2d::log("%d touched",scene);
    		tempAp->cancelAllTouch();
    	}, ss.str());


    	_order++;


    }


std::shared_ptr<APTouchManager> HelloWorld::getAPTouchManager() {
	return _apTouchManager;
}





void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
