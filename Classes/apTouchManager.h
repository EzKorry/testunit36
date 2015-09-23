/*
 * TouchManager.h
 *
 *  Created on: 2014. 11. 19.
 *      Author: EzKorry
 */

#ifndef APTOUCHMANAGER_H_
#define APTOUCHMANAGER_H_

#include <sstream>
#include <string>
#include "cocos2d.h"


/* how to use:
 * 1. #include "apTouchManager.h"
 * 2. auto manager = APTouchManager::create();
 * 3. manager->registerNode(~~~);
 * 4. manager->setBehavior(~~~);
 */

namespace arphomod {

/**
 * - Began: TouchBegan when each Checker true.
 * - MovedInner: TouchMoved out to in. required Began.
 * - MovedInside: TouchMoved in to in. required Began.
 * - MovedOuter: TouchMoved in to out. required Began.
 * - MovedOutside: TouchMoved out to out. required Began.
 * - MovedInnerIgnoreBegan: TouchMoved out to in. not required Began.
 * - MovedInsideIgnoreBegan: TouchMoved in to in. not required Began.
 * - MovedOuterIgnoreBegan: TouchMoved in to out. not required Began.
 * - EndedIn: TouchEnded inside. required Began.
 * - EndedOut: TouchEnded outside. required Began.
 * - Cancelled: TouchCancelled. required Began.
 */


enum class APTouchType{
	Began,
	MovedInner,
	MovedInside,
	MovedOuter,
	MovedOutside,
	MovedInnerIgnoreBegan,
	MovedInsideIgnoreBegan,
	MovedOuterIgnoreBegan,
	EndedIn,
	EndedOut,
	Cancelled,

};

enum class APAttribute{
	Checker,

};


using APTouchChecker = std::function<bool(cocos2d::Touch*)>;
using APTouchBehavior = std::function<void()>;
using APTouchBehaviorMap = std::unordered_map<cocos2d::Node*, APTouchBehavior>;

struct APTouchData {
	APTouchData(APTouchChecker checker) : checker(checker) {

	}
	APTouchChecker checker;
	bool enabled = true;
	int maxTouch = 6;
	int order = 0;
};

class APTouchManager{
public:



	// initialize Nodes
	// default order: 0
	// default max Touch: 6
	void registerNode(cocos2d::Node* node, APTouchChecker checker, const std::string& hook);
	void registerNode(cocos2d::Node* node, APTouchChecker checker, std::string&& hook);

	void initWithNodeToAttatch();

	// factory Method
	static std::shared_ptr<APTouchManager> create(cocos2d::Node* node);
	static std::shared_ptr<APTouchManager> getInstance();

	// set behavior with specified timing
	void addBehavior(cocos2d::Node* node,APTouchType timing, APTouchBehavior behavior, const std::string& behaviorTag);
	void addBehavior(cocos2d::Node* node,APTouchType timing, APTouchBehavior behavior, std::string&& behaviorTag);

	// delete behavior
	void delBehavior(const std::string& hook, const std::string& behaviorTag);

	// set checker that says when it is true if touch point is in custom area.
	void setChecker(cocos2d::Node* node, APTouchChecker checker);

	// set that this manager is enabled.
	void setEnabledManager(bool enabled);

	// get now touch.
	cocos2d::Touch* getTouch(cocos2d::Node* node);

	// set that something node is enabled.
	void setEnabledNode(cocos2d::Node* node, bool enabled);

	// set that all node is enabled.
	void setEnabledAll(bool enabled);

	// set something node's max touch. default: 6 (touch id:0~5)
	void setMaxTouch(cocos2d::Node* node, int max);

	// set something node's order. higher number indicates higher priority.
	void setOrder(cocos2d::Node* node, int order);

	void setBeganFromOutsideEnabled(bool enabled); 	// coming soon!

	// reset touch manager.
	void clear();

	// unregister node.
	void removeNode(cocos2d::Node* node);

	// if this execute when something touch is going, touch immediately end.
	// no callback(ended, cancelled) executes.
	void cancelAllTouch();

	// create node's default checker which uses getSize(), containsPoint()
	// node must have anchor point (0.5,0.5)
	static APTouchChecker createDefaultChecker(cocos2d::Node* node);

	static cocos2d::Rect createDefaultRect(cocos2d::Node* node);

	// create checker with rect. it is absolute.
	static APTouchChecker createCheckerWithRect(const cocos2d::Rect& rect);

	// create checke with rect following node. it is relative rect.
	// if with Rect(-20, -30, 100, 100), node positioning (200,200), then you can touch
	// (180, 170, 100, 100) rect at that time.
	static APTouchChecker createCheckerWithRect(cocos2d::Node* node, const cocos2d::Rect& rect);


	virtual ~APTouchManager();
	APTouchManager();
	APTouchManager(APTouchManager&& rhs);
	APTouchManager& operator=(APTouchManager&& rhs);

private:
	// initialize Callbacks
	void attachCallback();

	// callbacks
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
	inline bool isBehaviorNullptr(cocos2d::Node* node, APTouchType type);
	inline cocos2d::Vec2 getAbsolutePosition(cocos2d::Node* node);

	void touchStarted(cocos2d::Touch* touch);
	void touchEnded(cocos2d::Touch* touch, bool isCancelled = false);

	cocos2d::Node* getTouchedNode(cocos2d::Touch* touch);

	//void onEvent(cocos2d::Touch* touch, cocos2d::Event* event, APTouchType timing);



	std::map<APTouchType, APTouchBehaviorMap> _behaviorMap;
	std::unordered_map<cocos2d::Node*, APTouchData> _d;
	/*std::unordered_map<cocos2d::Node*, APTouchChecker> _checkerMap;
	std::unordered_map<cocos2d::Node*, bool> _enabledMap;
	std::unordered_map<cocos2d::Node*, int> _maxTouchMap;
	std::unordered_map<cocos2d::Node*, int> _nodeToOrder;*/
	std::unordered_map<int, cocos2d::Node*> _touchToNowNode;
	std::unordered_map<int, cocos2d::Node*> _touchToBeganNode;
	std::unordered_map<int, cocos2d::Touch*> _touchIdToTouch;

	static std::shared_ptr<APTouchManager> _sp;

	bool _enabled{true};



};

template<typename T>
std::string NumberToString(T Number) {
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}


}

#endif /* APTOUCHMANAGER_H_ */
