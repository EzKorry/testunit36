/*
 * apField.cpp
 *
 *  Created on: 2015. 4. 24.
 *      Author: EzKorry
 */

#define M_PI           3.14159265358979323846

#include "apField.h"
#include "apPoint.h"
#include <cmath>

namespace arphomod {

apField::apField() {
	// TODO Auto-generated constructor stub

}

apField::~apField() {
	// TODO Auto-generated destructor stub
}


bool apField::init() {
	_drawNode = cocos2d::DrawNode::create();
	this->addChild(_drawNode, 0, "drawNode");
	return true;

}

void apField::makePolygon(std::vector<apPointPtr> vec) {

	_points.emplace(_id, std::list<apPointPtr>());

	for (auto it = vec.begin(); it != vec.end(); it++) {
		auto& item = *it;
		// insert polygon.
		_points[_id].push_back(item);

		// when already exist point,
		if (_polygon.find(item) != _polygon.end()) {
			_polygon[item].emplace(_id);

		} else {
			std::set<int> temp;
			temp.emplace(_id);
			_polygon.emplace(item, temp);
		}
	}

	// for next polygon.
	_id++;

}
void apField::generatePath() {

	if(_startingPoint == nullptr || _destinationPoint == nullptr) return;


	_parent.clear();
	_isConnected.clear();

	// distinguish available dots.
	for(auto& item : _polygon) {
		auto p = item.first;
		auto np1 = _points[0].begin();
		auto np2 = _points[0].begin();

		auto polygons = item.second;
		float angle = 0.0;

		for(auto& polygon : polygons) {
			auto points = _points[polygon];
			auto ip = std::find(points.begin(), points.end(), p);

			if(ip == points.begin()) {
				np1 = std::prev(points.end());
				np2 = std::next(ip);
			}
			else if(ip == std::prev(points.end())) {
				np1 = std::prev(ip);
				np2 = points.begin();
			}
			else {
				np1 = std::prev(ip);
				np2 = std::next(ip);
			}

			auto ax = (*np1)->x - p->x;
			auto ay = (*np1)->y - p->y;
			auto bx = (*np2)->x - p->x;
			auto by = (*np2)->y - p->y;

			auto a = atan2(ay, ax);
			auto b = atan2(by, bx);

			auto r = std::abs(a-b);
			if(r > M_PI) {
				r = M_PI * 2- r;
			}
			angle += r;
		}

		if(angle < M_PI) {
			_isConnected.emplace(p, std::set<apPointPtr>());
			cocos2d::log("%d added to _isConnected!", p.get());
		}
	}

	_isConnected.emplace(_startingPoint, std::set<apPointPtr>());
	_isConnected.emplace(_destinationPoint, std::set<apPointPtr>());

	// setting path.
	for(auto it = _isConnected.begin(); it!=_isConnected.end(); it++) {
		auto p1 = it->first;

		for(auto jt = std::next(it); jt!= _isConnected.end(); jt++) {
			auto p2 = jt->first;

			auto con = inSamePolygon(p1, p2);

			// if two points are not same polygon
			if (con == 0) {

				// for checking collide with walls. if collide, it cannot be path.
				bool collide = false;

				for (auto& wallpair : _points) {
					auto walls = wallpair.second;
					for (auto itwall1 = walls.begin(); itwall1 != walls.end();
							itwall1++) {

						auto itwall2 = std::next(itwall1);

						if (itwall2 == walls.end())
							itwall2 = walls.begin();

						if(*itwall1 == p1 || *itwall1 == p2 ||
						   *itwall2 == p1 || *itwall2 == p2) {

						}

						else if (getLineIntersection(p1->x, p1->y, p2->x, p2->y,
								(*itwall1)->x, (*itwall1)->y, (*itwall2)->x,
								(*itwall2)->y)) {
							collide = true;
							break;
						}
					}

					if (collide == true)
						break;
				}

				if (collide == false) {
					_isConnected[p1].emplace(p2);
					_isConnected[p2].emplace(p1);
				}
			}

			// if two points are same polygon and not wall
			else if (con == 1) {

				if (isSidePoint(p1, p2)) {
					_isConnected[p1].emplace(p2);
					_isConnected[p2].emplace(p1);
				}

			}

			// if wall...
			else if (con == 2) {
				// don't do anything.

			}

			// well..
			else {
			}
		}
	}
}
void apField::setStart(apPointPtr p) {
	_startingPoint = p;

}
void apField::setDestination(apPointPtr p) {
	_destinationPoint = p;

}

bool apField::isSidePoint(apPointPtr p1, apPointPtr p2) {

	if(!inSamePolygon(p1,p2) == 1) return false;

	auto list = _points[*(getPolygon(p1).begin())];
	auto ip1 = std::find(list.begin(), list.end(), p1);
	auto ip2 = std::find(list.begin(), list.end(), p2);

	if(ip1 == list.begin()) {
		if(ip2 == std::prev(list.end()) || ip2 == std::next(ip1)) return true;
		return false;

	} else if(ip2 == list.begin()) {
		if(ip1 == std::prev(list.end())|| ip1 == std::next(ip2))  return true;
		return false;

	} else if(
			std::prev(ip1) == ip2
		 || std::prev(ip2) == ip1){
		return true;
	}
	return false;


}


int apField::inSamePolygon(apPointPtr p1, apPointPtr p2) {
	auto count = 0;
	for(auto& item:_polygon[p1]) {
		if(std::find(_points[item].begin(), _points[item].end(), p2) != _points[item].end())
			count++;
	}
	return count;

}



//  Globals which should be set before calling this function:
//
//  int    polyCorners  =  how many corners the polygon has
//  float  polyX[]      =  horizontal coordinates of corners
//  float  polyY[]      =  vertical coordinates of corners
//  float  x, y         =  point to be tested
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

bool apField::pointInPolygon(apPointPtr p, int polygonId) {

	int polyCorners = _points[polygonId].size();
	std::vector<float> polyX, polyY;
	for (auto& item : _points[polygonId]) {
		polyX.push_back(item->x);
		polyY.push_back(item->y);
	}
	auto x = p->x, y = p->y;

	int i, j = polyCorners - 1;
	bool oddNodes = false;

  for (i=0; i<polyCorners; i++) {
    if ((polyY[i]< y && polyY[j]>=y
    ||   polyY[j]< y && polyY[i]>=y)
    &&  (polyX[i]<=x || polyX[j]<=x)) {
      oddNodes^=(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x); }
    j=i; }

  return oddNodes;

}
std::set<int> apField::getPolygon(apPointPtr p) {
	return _polygon[p];
}

std::list<apPointPtr> apField::findPath() {

	//  std::list<apPointPtr>();
/*
	_isConnected.emplace(_startingPoint, std::set<apPointPtr>());
	_isConnected.emplace(_destinationPoint, std::set<apPointPtr>());



	// as all walls
	for (auto& item : _isConnected) {
		auto p = item.first;
		auto startingCollide = false;
		auto destinationCollide = false;

		for (auto& wallpair : _points) {
			auto walls = wallpair.second;
			for (auto itwall1 = walls.begin(); itwall1 != walls.end();
					itwall1++) {

				auto itwall2 = std::next(itwall1);

				if (itwall2 == walls.end())
					itwall2 = walls.begin();

				if (getLineIntersection(p->x, p->y, _startingPoint->x,
						_startingPoint->y, (*itwall1)->x, (*itwall1)->y,
						(*itwall2)->x, (*itwall2)->y)) {
					startingCollide = true;

				}
				if (getLineIntersection(p->x, p->y, _destinationPoint->x,
						_destinationPoint->y, (*itwall1)->x, (*itwall1)->y,
						(*itwall2)->x, (*itwall2)->y)) {
					destinationCollide = true;

				}

			}
		}

		if(startingCollide == false) {
			_isConnected[_startingPoint].emplace(p);
			_isConnected[p].emplace(_startingPoint);
		}
		if(destinationCollide == false) {
			_isConnected[_destinationPoint].emplace(p);
			_isConnected[p].emplace(_destinationPoint);
		}
	}
*/
	std::set<apPointPtr> closedNodes;
	std::set<apPointPtr> openedNodes;

	// key is child, value is parent.
	std::unordered_map<apPointPtr, apPointPtr> parentMap;
	for(auto& item : _isConnected) {
		parentMap.emplace(item.first, nullptr);
	}

	// starting point to closed nodes
	closedNodes.emplace(_startingPoint);

	// push nodes that is connected starting point to openlist
	for (auto& item : _isConnected[_startingPoint]) {
		openedNodes.emplace(item);
		parentMap[item] = _startingPoint;
	}

	auto success = false;

	// getGLambda
	auto getG =
			[&parentMap, this](apPointPtr p) -> int {
				auto temp = p;
				auto g = 0;
				while (parentMap[temp] != nullptr) {
					auto parent = parentMap[temp];
					g += getDistance(temp->x, temp->y, parent->x, parent->y);
					temp = parent;
				}
				cocos2d::log("get g: %d", g);

				return g;
			};

	// getHLambda.
	auto getH = [&parentMap, this](apPointPtr p, apPointPtr destination) ->int {
		int a = 0;
		a = getDistance(p->x, p->y, destination->x, destination->y);
		return a;
	};

	// loop until success or openedNodes get empty.
	while (!success && !openedNodes.empty()) {

		//select lowest F Node in openlist
		apPointPtr lowFNode;
		auto low_F = 999999999;

		for (auto& item : openedNodes) {
			auto f = getG(item) + getH(item, _destinationPoint);
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
		if (lowFNode == _destinationPoint) {
			success = true;
			break;
		}

		auto neighborNodes = _isConnected[lowFNode];

		// neighbor of now node(lowest F Node)
		for (auto& item : neighborNodes) {

			// if neighborNode not exists in closed list
			if (closedNodes.find(item) == closedNodes.end()) {

				// if neighborNode not exists in opened list
				// add openlist and set parent
				if (openedNodes.find(item) == openedNodes.end()) {
					openedNodes.emplace(item);
					parentMap[item] = lowFNode;

				}

				// if neighbroNode exists in opened list
				// recalculate G cost.
				else {
					auto beforeG = getG(item);
					auto originParent = parentMap[item];
					parentMap[item] = lowFNode;
					auto afterG = getG(item);

					if (afterG < beforeG) {

					} else {
						parentMap[item] = originParent;
					}
				}
			}
		}
	}

	std::list<apPointPtr> finalList;

	if (success) {
		auto temp = _destinationPoint;
		while (temp != nullptr) {
			finalList.push_front(temp);
			temp = parentMap[temp];
		}
	}

	// delete startingPoint and destinationPoint

	return finalList;
}
void apField::drawThings() {
	_drawNode->clear();

	//polygon : gray
	for(auto& item : _points) {

		std::vector<Vec2> v;
		for(auto& point : item.second) {
			v.push_back(Vec2(point->x, point->y));
		}
		//std::vector<apPointPtr> v(item.second.begin(), item.second.end());

		_drawNode->drawPolygon(v.begin().base(), v.size(), Color4F(0.3,0.3,0.3,1), 1.0, Color4F(0,0,0,0));

	}
	//polygon dots: salgu color
	for(auto&item : _polygon ) {
		_drawNode->drawDot(Vec2(item.first->x, item.first->y), 10, Color4F(1, 177.0/255.0, 158.0/255.0, 1));

	}
	//available route : purple
	for(auto& item: _isConnected) {
		for(auto& dest: item.second) {
			_drawNode->drawSegment(
					Vec2(item.first->x, item.first->y),
					Vec2(dest->x, dest->y),4,
					Color4F(112.0/255.0, 37.0/255.0, 93.0/255.0, 1));
		}
	}

	// available dots : light green.
	for (auto& item : _isConnected) {
		_drawNode->drawDot(Vec2(item.first->x, item.first->y), 15,
				Color4F(126.0 / 255.0, 210.0 / 255.0, 99.0 / 255.0, 1));
	}
}

inline float apField::getDistance(const float& ax,const float& ay,const float& bx,const float& by) const {
	return sqrt(pow((ax - bx) * _ratio, 2.0)
			 + pow((ay - by) * _ratio, 2.0));
}



} /* namespace arphomod */

