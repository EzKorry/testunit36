/*
 * TouchManager.cpp
 *
 *  Created on: 2014. 11. 19.
 *      Author: EzKorry
 */

#include <memory>
#include <functional>
#include <algorithm>
#include <future>
#include "apTouchManager.h"
#include "cocos2d.h"


namespace arphomod {



// static member initialize
std::shared_ptr<APTouchManager> APTouchManager::create(cocos2d::Node* node) {
	auto ptr = std::make_shared<APTouchManager>();
	ptr->initWithNodeToAttatch(node);
	return ptr;

}

void APTouchManager::initWithNodeToAttatch(cocos2d::Node* node) {
	_behaviorMap.emplace(APTouchType::Began, APTouchBehaviorMap{});
	_behaviorMap.emplace(APTouchType::MovedInside, APTouchBehaviorMap{});
	_behaviorMap.emplace(APTouchType::MovedOutside, APTouchBehaviorMap{});
	_behaviorMap.emplace(APTouchType::MovedInner, APTouchBehaviorMap{});
	_behaviorMap.emplace(APTouchType::MovedOuter, APTouchBehaviorMap{});
	_behaviorMap.emplace(APTouchType::EndedIn, APTouchBehaviorMap{});
	_behaviorMap.emplace(APTouchType::EndedOut, APTouchBehaviorMap{});
	_behaviorMap.emplace(APTouchType::Cancelled, APTouchBehaviorMap{});

	for(int i=0;i<6;i++) {
		_beganNodeMap.emplace(i, nullptr);
		_nowNodeMap.emplace(i, nullptr);
	}
	attachCallback(node);
}

void APTouchManager::attachCallback(cocos2d::Node* node){
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&APTouchManager::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&APTouchManager::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&APTouchManager::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&APTouchManager::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 100);
}


void APTouchManager::registerNode(cocos2d::Node* node,	APTouchChecker checker) {
	if (_checkerMap.find(node) != _checkerMap.end()) {
		_checkerMap.at(node) = checker;

	} else {
		// node retain.
		node->retain();
		//node->setonExitTransitionDidStartCallback([](){cocos2d::log("OnExitTransitionDidStart!!");});
		//node->setOnExitCallback([](){cocos2d::log("onExit!!");});
		_checkerMap.emplace(node, checker);
		_enabledMap.emplace(node, true);
		for(auto& item : _behaviorMap) {
			item.second.emplace(node, nullptr);
		}
		_maxTouchMap.emplace(node, 6);
		_orderMap.emplace(node, 0);
		//_isBegan.emplace(node, false);
	}
}
/*
void apTouchManager::setNode(cocos2d::Node* node, apTouchBehavior behavior,
		apTouchChecker checker) {
	this->RegisterNode(node, behavior, checker, node->getGlobalZOrder());
}

void apTouchManager::setNode(cocos2d::Node* node, apTouchBehavior behavior) {
	this->RegisterNode(node, behavior, createDefaultChecker(node), node->getGlobalZOrder());
}
*/
void APTouchManager::setBehavior(cocos2d::Node* node, APTouchBehavior behavior, APTouchType timing) {
	if (_behaviorMap[timing].find(node) != _behaviorMap[timing].end()) {
		_behaviorMap.at(timing).at(node) = behavior;
		//cocos2d::log("timing %d set!", timing);
	}
}
void APTouchManager::setChecker(cocos2d::Node* node, APTouchChecker checker) {
	if (_checkerMap.find(node) != _checkerMap.end()) {
		_checkerMap.at(node) = checker;
	}
}

void APTouchManager::setEnabledManager(bool enabled) {
	_enabled = enabled;
}

void APTouchManager::setEnabledNode(cocos2d::Node* node, bool enabled) {
	if (_enabledMap.find(node) != _enabledMap.end()) {
		_enabledMap.at(node) = enabled;
	}
}

void APTouchManager::setEnabledAll(bool enabled) {
	for(auto& item : _enabledMap) {
		item.second = enabled;
	}
}

void APTouchManager::setMaxTouch(cocos2d::Node* node, int max) {
	if (_maxTouchMap.find(node) != _maxTouchMap.end()) {
		_maxTouchMap[node] = max;
	}
}

void APTouchManager::setOrder(cocos2d::Node* node, int order) {
	if (_orderMap.find(node) != _orderMap.end()) {
		_orderMap[node] = order;
	}


}


void APTouchManager::setBeganFromOutsideEnabled(bool enabled) {

}

void APTouchManager::clear() {

	for(auto& item:_behaviorMap) {
		for(auto& item2:item.second) {
			item2.first->release();
		}

		item.second.clear();
	}
	_checkerMap.clear();
	_enabledMap.clear();
	_maxTouchMap.clear();
	_orderMap.clear();



	//_isBegan.clear();
}

void APTouchManager::removeNode(cocos2d::Node* node) {
	if (_checkerMap.find(node) != _checkerMap.end()) {
		for(auto& item:_behaviorMap) {
			item.second.erase(node);
		}
		_checkerMap.erase(node);
		_enabledMap.erase(node);
		_maxTouchMap.erase(node);
		_orderMap.erase(node);
		//_isBegan.erase(node);

		node->release();
	}
}

void APTouchManager::cancelAllTouch() {
	for (int i = 0; i < 6; i++) {
		_beganNodeMap[i] = nullptr;
		_nowNodeMap[i] = nullptr;
	}
}


APTouchChecker APTouchManager::createDefaultChecker(cocos2d::Node* node) {
	auto size = node->getContentSize();

	return [size,node](cocos2d::Touch* touch)->bool {
		auto position = node->getPosition();
		auto anchor = node->getAnchorPoint();
		auto parent = node->getParent();
		if (parent != nullptr) {
			position = parent->convertToWorldSpace(position);
		}
		auto rect = cocos2d::Rect(position.x - size.width * anchor.x,
					position.y - size.height * anchor.y, size.width, size.height);

		//cocos2d::log("name: %s, %f %f %f %f",node->getName().c_str(), rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);

		return rect.containsPoint(touch->getLocation());

	};
}

cocos2d::Rect APTouchManager::createDefaultRect(cocos2d::Node* node) {
	auto size = node->getContentSize();
	auto position = node->getPosition();
	auto anchor = node->getAnchorPoint();
	auto parent = node->getParent();
	if (parent != nullptr) {
		position = parent->convertToWorldSpace(position);
	}

	auto rect = cocos2d::Rect(position.x - size.width * anchor.x,
			position.y - size.height * anchor.y, size.width, size.height);
	cocos2d::log("%f %f %f %f",position.x - size.width * anchor.x,
			position.y - size.height * anchor.y, size.width, size.height);
	return rect;

}

APTouchChecker APTouchManager::createCheckerWithRect(const cocos2d::Rect& rect) {

	return [rect](cocos2d::Touch* touch)->bool{
		return rect.containsPoint(touch->getLocation());

	};
}

APTouchChecker APTouchManager::createCheckerWithRect(cocos2d::Node* node, const cocos2d::Rect& rect) {

	return [rect, node](cocos2d::Touch* touch)->bool {
		auto position = node->getPosition();
		auto parent = node->getParent();
		if (parent != nullptr) {
			position = parent->convertToWorldSpace(position);
		}

		auto r = cocos2d::Rect(rect);
		r.origin += position;
		return r.containsPoint(touch->getLocation());
	};
}

bool APTouchManager::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){

	//if touch count is over 6
	//if(touch->getID() > 5) return true;
/*
	if (_touchIDMap.find(touch->getID()) == _touchIDMap.end()) {
		_touchIDMap.insert(decltype(_touchIDMap)::value_type(touch->getID, nullptr));
	} else {
		cocos2d::log("weird touchStarted");
	}
*/
	auto nowNode = getTouchedNode(touch);


	if (nowNode != nullptr) {


		_beganNodeMap[touch->getID()] = nowNode;
		_nowNodeMap[touch->getID()] = nowNode;

		if (!isBehaviorNullptr(nowNode, APTouchType::Began)) {
			_behaviorMap[APTouchType::Began][nowNode](touch);
		}
	}
	return true;
/*
	for(const auto& item :_checkerMap) {
		if(item.second(touch) && _globalZOrderVec.at(item.first) > biggest) {
			node = item.first;
			biggest = _globalZOrderVec.at(item.first);
		}
	}
	if (node) {
		_behaviorMap.at(node)();
	}
*/
}

void APTouchManager::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event){
	auto id = touch->getID();
	// if not began
	if (_beganNodeMap[id] == nullptr || id > 5)
		return;

	auto nowNode = getTouchedNode(touch);
	auto beforeNode = _nowNodeMap[id];
	_nowNodeMap[id] = nowNode;

	auto originNode = _beganNodeMap[id];
	// outside
	if (beforeNode != originNode && nowNode != originNode
			&& !isBehaviorNullptr(originNode, APTouchType::MovedOutside)) {
		_behaviorMap[APTouchType::MovedOutside][originNode](touch);

	}
	// inside
	else if (beforeNode == originNode && nowNode == originNode
			&& !isBehaviorNullptr(originNode, APTouchType::MovedInside)) {
		_behaviorMap[APTouchType::MovedInside][originNode](touch);
	}

	// go to out
	else if (beforeNode == originNode && nowNode != originNode
			&& !isBehaviorNullptr(originNode, APTouchType::MovedOuter)) {
		_behaviorMap[APTouchType::MovedOuter][originNode](touch);

	}

	// inner
	else if (beforeNode != originNode && nowNode == originNode
			&& !isBehaviorNullptr(originNode, APTouchType::MovedInner)) {
		_behaviorMap[APTouchType::MovedInner][originNode](touch);
	}

}
void APTouchManager::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
	touchEnded(touch);
}
void APTouchManager::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event){
	touchEnded(touch, true);

}
void APTouchManager::touchEnded(cocos2d::Touch* touch, bool isCancelled) {
	auto id = touch->getID();
	// if not began
	if (_beganNodeMap[id] == nullptr || id > 5)
		return;

	auto originNode = _beganNodeMap[id];
	auto nowNode = getTouchedNode(touch);
	if (nowNode != originNode
			&& !isBehaviorNullptr(originNode, APTouchType::EndedOut)) {
		_behaviorMap[APTouchType::EndedOut][originNode](touch);

	} else if (nowNode == originNode
			&& !isBehaviorNullptr(originNode, APTouchType::EndedIn)) {
		_behaviorMap[APTouchType::EndedIn][originNode](touch);

	}

	_beganNodeMap[id] = nullptr;
	_nowNodeMap[id] = nullptr;


	/*
	if (_touchIDMap.find(touch->getID()) != _touchIDMap.end()) {
		_touchIDMap.erase(touch->getID());
	} else {
		cocos2d::log("weird touchEnded");
	}*/
}

/*
void APTouchManager::onEvent(cocos2d::Touch* touch, cocos2d::Event* event, APTouchType timing) {
	std::vector<cocos2d::Node*> TouchedNodes;

	for (const auto& item : _checkerMap) {
		if (item.second(touch)) {
			TouchedNodes.push_back(item.first);
		}
	}

	if (TouchedNodes.empty()) {
		return;
	} else {

		std::sort(TouchedNodes.begin(), TouchedNodes.end(),
				[](const cocos2d::Node* left, const cocos2d::Node* right)->bool {
					return left->getGlobalZOrder() > right->getGlobalZOrder();
				});
		if (_behaviorMap[timing][TouchedNodes[0]] != nullptr) {
			_behaviorMap[timing][TouchedNodes[0]](touch);
		}
	}

}*/

cocos2d::Node* APTouchManager::getTouchedNode(cocos2d::Touch* touch) {


	std::vector<cocos2d::Node*> TouchedNodes;
	std::vector<cocos2d::Node*> NodesToBeRemoved;

	for (const auto& item : _checkerMap) {

		auto node = item.first;

		// check if item is not available. then push to remove vector
		if (!node->isRunning() && node->getParent() == nullptr)
			NodesToBeRemoved.emplace_back(node);


		// checker true and enabled true and max Touch is irrelevant
		else if (node->isRunning() && item.second(touch) && _enabledMap[node] && touch->getID() < _maxTouchMap[node])
			TouchedNodes.emplace_back(node);

	}

	// node remove
	for (const auto& item : NodesToBeRemoved) {
		removeNode(item);
	}

	// nothing touched.
	if (TouchedNodes.empty()) {
		return nullptr;
	}

	// something touched
	else {

		// sort
		std::sort(TouchedNodes.begin(), TouchedNodes.end(),
				[this](cocos2d::Node* left, cocos2d::Node* right)->bool {
					return _orderMap[left] > _orderMap[right];
				});
		return TouchedNodes[0];

	}


}
bool APTouchManager::isBehaviorNullptr(cocos2d::Node* node, APTouchType type) {
	return _behaviorMap[type][node] == nullptr;
}

cocos2d::Vec2 APTouchManager::getAbsolutePosition(cocos2d::Node* node) {
	auto position = node->getPosition();

	auto parent = node->getParent();
	if (parent != nullptr) {
		position = parent->convertToWorldSpace(position);
	}

	return position;

}
void APTouchManager::touchStarted(cocos2d::Touch* touch) {

}


APTouchManager::~APTouchManager() = default;

APTouchManager::APTouchManager() = default;

APTouchManager::APTouchManager(APTouchManager&& rhs) = default;

APTouchManager& APTouchManager::operator=(APTouchManager&& rhs) = default;


}
