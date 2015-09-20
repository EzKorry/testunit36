/*
 * asPointManager.h
 *
 *  Created on: 2015. 4. 12.
 *      Author: EzKorry
 */

#ifndef ASPOINTMANAGER_H_
#define ASPOINTMANAGER_H_

#include <unordered_map>
#include "cocos2d.h"

class asPointTreeNode;
using NodePtr = std::shared_ptr<asPointTreeNode>;
using NodeContainer = std::set<NodePtr>;

class asPoint;

class asPointTreeNode{
public:
	asPointTreeNode(std::shared_ptr<asPointTreeNode> p,	asPoint* point);
	virtual ~asPointTreeNode();

	static std::shared_ptr<asPointTreeNode> create(std::shared_ptr<asPointTreeNode> parent, asPoint* point);
	int num=0;

	asPoint* node{nullptr};
	NodePtr parent{nullptr};


};
class asPointManager {
public:
	asPointManager();
	virtual ~asPointManager();
	static std::shared_ptr<asPointManager> create();
	void init();

	void addPoint(asPoint* p);
	void delPoint(asPoint* p);
	asPoint* createAndAddPoint();
	void connectPoint(asPoint* p1, asPoint* p2);
	void disconnectPoint(asPoint* p1, asPoint* p2);
	bool isConnected(asPoint* p1, asPoint* p2);

	// power hard
	// returns total path order list.
	// if path not exist, return empty list.
	std::list<asPoint*> findPath(asPoint* p1, asPoint* p2);



private:
	std::unordered_map<asPoint*, std::set<asPoint*>> _isConnected;
	//std::unordered_map<NodePtr, std::set<NodePtr>> _isConnectedTreeNode;

	void drawRoute(asPoint* p1, asPoint* p2);
	void delRoute(asPoint* p1, asPoint* p2);

	int getG(NodePtr p) const;
	int getH(NodePtr p, NodePtr destination) const;
	inline float getDistance(const cocos2d::Vec2& a, const cocos2d::Vec2& b) const;
	const float _ratio = 1.0;


};

#endif /* ASPOINTMANAGER_H_ */
