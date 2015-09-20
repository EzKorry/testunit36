/*
 * asPointManager.cpp
 *
 *  Created on: 2015. 4. 12.
 *      Author: EzKorry
 */

#include "asPointManager.h"
#include "asPoint.h"
#include <cmath>

asPointTreeNode::asPointTreeNode(std::shared_ptr<asPointTreeNode> p,
		asPoint* point) :
		parent(p), node(point) {
}
asPointTreeNode::~asPointTreeNode() {
}

std::shared_ptr<asPointTreeNode> asPointTreeNode::create(
		std::shared_ptr<asPointTreeNode> parent, asPoint* point) {
	if (point == nullptr)
		return nullptr;
	auto p = std::make_shared<asPointTreeNode>(parent, point);
	return p;
}

asPointManager::asPointManager() {
	// TODO Auto-generated constructor stub

}

asPointManager::~asPointManager() {
	// TODO Auto-generated destructor stub
}

void asPointManager::init() {

}

void asPointManager::addPoint(asPoint* p) {
	// if cannnot find element,
	if (_isConnected.find(p) == _isConnected.end()) {
		_isConnected.emplace(p, std::set<asPoint*>());
	}
}

void asPointManager::delPoint(asPoint* p) {

	_isConnected.erase(p);

	for (auto item : _isConnected) {
		if (item.second.find(p) != item.second.end()) {
			item.second.erase(p);
		}
	}

}

asPoint* asPointManager::createAndAddPoint() {
	auto p = asPoint::create();
	_isConnected.emplace(p, std::set<asPoint*>());

	return p;

}

void asPointManager::connectPoint(asPoint* p1, asPoint* p2) {
	// Check if there's element.
	if (isConnected(p1, p2))
		return;
	if (p1 == p2)
		return;

	_isConnected[p1].emplace(p2);
	_isConnected[p2].emplace(p1);

	drawRoute(p1, p2);

}

void asPointManager::disconnectPoint(asPoint* p1, asPoint* p2) {
	if (!isConnected(p1, p2))
		return;

	_isConnected[p1].erase(p2);
	_isConnected[p2].erase(p1);
}

bool asPointManager::isConnected(asPoint* p1, asPoint* p2) {
	//if p1 not added
	if (_isConnected.find(p1) == _isConnected.end())
		return false;

	//return _isConnected[p1] contains p2.
	return _isConnected[p1].find(p2) != _isConnected[p1].end();
}

std::list<asPoint*> asPointManager::findPath(asPoint* p1, asPoint* p2) {

	NodeContainer closedNodes;
	NodeContainer openedNodes;
	std::unordered_map<asPoint*, NodePtr> pointToNode;
	std::unordered_map<NodePtr, std::set<NodePtr>> isConnected;

// create NodePtr and push to isConnected container
	int num=0;
	for (auto& item : _isConnected) {
		auto node = asPointTreeNode::create(nullptr, item.first);
		node->num = num;
		num++;
		pointToNode.emplace(item.first, node);
		isConnected.emplace(node, std::set<NodePtr>());
	}

// copy _isConnected(asPoint* ver.) to isConnected(NodePtr(shared_ptr<asPointTreeNode>) ver.)
	for (auto& item : isConnected) {
		for (auto& connected : _isConnected[item.first->node]) {
			item.second.emplace(pointToNode[connected]);
		}
	}

	auto startingPoint = pointToNode[p1];
	auto destinationPoint = pointToNode[p2];

	// starting point to closed nodes
	closedNodes.emplace(startingPoint);

// push nodes that is connected starting point to openlist
	for (auto& item : isConnected[startingPoint]) {
		openedNodes.emplace(item);
		item->parent = startingPoint;
	}


	auto success = false;

	// loop until success or openedNodes get empty.
	while (!success && !openedNodes.empty()) {

		//select lowest F Node in openlist
		NodePtr lowFNode;
		auto low_F = 999999999;

		for (auto& item : openedNodes) {
			auto f = getG(item) + getH(item, destinationPoint);
			if (low_F > f) {
				low_F = f;
				lowFNode = item;
			}

		}

		// logging
		/*
		auto low_g = getG(lowFNode);
		auto low_h = getH(lowFNode, destinationPoint);
		cocos2d::log("(%.2f, %.2f) >> parent:(%.2f, %.2f), g:%d, h:%d, f:%d",
				lowFNode->node->getPositionX(), lowFNode->node->getPositionY(),
				lowFNode->parent->node->getPositionX(),
				lowFNode->parent->node->getPositionY(), low_g, low_h,
				low_g + low_h);*/

		// delete lowest F node in opened list and
		// add this in closed list
		openedNodes.erase(lowFNode);
		closedNodes.emplace(lowFNode);

		// if this node is the destination, success!
		if(lowFNode == destinationPoint) {
			success = true;
			break;
		}

		auto neighborNodes = isConnected[lowFNode];

		// neighbor of now node(lowest F Node)
		for(auto& item : neighborNodes) {

			// if neighborNode not exists in closed list
			if(closedNodes.find(item) == closedNodes.end()) {

				// if neighborNode not exists in opened list
				// add openlist and set parent
				if(openedNodes.find(item) == openedNodes.end()) {
					openedNodes.emplace(item);
					item->parent = lowFNode;

				}

				// if neighbroNode exists in opened list
				// recalculate G cost.
				else {
					auto beforeG = getG(item);
					auto originParent = item->parent;
					item->parent = lowFNode;
					auto afterG = getG(item);

					if(afterG < beforeG) {

					} else {
						item->parent = originParent;
					}
				}
			}
		}
	}

	std::list<asPoint*> finalList;

	if(success) {
		auto temp = destinationPoint;
		while(temp != nullptr) {
			finalList.push_front(temp->node);
			temp = temp->parent;
		}
	}

	return finalList;

}

std::shared_ptr<asPointManager> asPointManager::create() {

	cocos2d::log("asPointManager create()");
	auto ptr = std::make_shared<asPointManager>();

	ptr->init();
	return ptr;

}

void asPointManager::drawRoute(asPoint* p1, asPoint* p2) {
	if (p1 < p2)
		p1->drawRoute(p2);
	else
		p2->drawRoute(p1);

}

int asPointManager::getG(NodePtr p) const {
	auto temp = p;
	auto g = 0;
	while (temp->parent != nullptr) {
		auto parent = temp->parent;
		g += getDistance(temp->node->getPosition(), parent->node->getPosition());
		temp = parent;
	}
	//cocos2d::log("get g: %d", g);

	return g;
}

int asPointManager::getH(NodePtr p, NodePtr destination) const {
	int a;
	a = getDistance(p->node->getPosition(), destination->node->getPosition());
	return a;
}

inline float asPointManager::getDistance(const cocos2d::Vec2& a, const cocos2d::Vec2& b) const {
	return sqrt(pow((a.x - b.x) * _ratio, 2.0)
			 + pow((a.y - b.y) * _ratio, 2.0));
}
