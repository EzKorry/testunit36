/*
 * apField.h
 *
 *  Created on: 2015. 4. 24.
 *      Author: EzKorry
 */

#ifndef APFIELD_H_
#define APFIELD_H_

#include <cocos2d.h>
#include <set>
#include <unordered_map>
#include <apPoint.h>
USING_NS_CC;

namespace arphomod {


class apField : public cocos2d::Node {
public:
	apField();
	virtual ~apField();

	CREATE_FUNC(apField);


	bool init() override;

	// first, you should make Polygon.
	// must NOT overlap with other polygon0.


	void makePolygon(std::vector<apPointPtr> vec);

	// all making polygon tasks are end, then generatePath.
	// distinguish Available dots(movable area > 180 )
	// then make path.
	void generatePath();

	// set start point
	void setStart(apPointPtr p);

	// set Destination with apPoint.
	void setDestination(apPointPtr p);

	// do they have same polygon? and are they neighbor dots? and is not between polygons?
	bool isSidePoint(apPointPtr p1, apPointPtr p2);

	// do dots have same polygon?
	// 0: no, 1:yes.just1. 2:haveSameTwoPolygon
	int inSamePolygon(apPointPtr p1, apPointPtr p2);

	// is the point in the polygon?
	bool pointInPolygon(apPointPtr p, int polygonId);

	std::set<int> getPolygon(apPointPtr p);

	std::list<apPointPtr> findPath();

	void drawThings();

	inline float getDistance(const float& ax,const float& ay,const float& bx,const float& by) const;

	static bool getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y,
		    float p2_x, float p2_y, float p3_x, float p3_y /*float *i_x, float *i_y*/)
		{
		    float s1_x, s1_y, s2_x, s2_y;
		    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
		    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

		    float s, t;
		    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
		    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

		    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		    {
		        // Collision detected
		        /*if (i_x != nullptr)
		            *i_x = p0_x + (t * s1_x);
		        if (i_y != nullptr)
		            *i_y = p0_y + (t * s1_y);*/
		        return true;
		    }

		    return false; // No collision
		}


private:
	// get points which are in same polygon.
	std::unordered_map<int, std::list<apPointPtr>> _points;

	// point and polygon id. if set is empty, no polygon.
	// points can be on several polygons. so, it is std::set.
	std::unordered_map<apPointPtr, std::set<int>> _polygon;

	// left is child, right is parent.
	// used when a* algorithm goes.
	std::unordered_map<apPointPtr, apPointPtr> _parent;

	// connected points.
	// used when a* algorithm goes.
	std::unordered_map<apPointPtr, std::set<apPointPtr>> _isConnected;

	// when add polygon, that polygon's id.
	// finish adding, _id ++.
	int _id { 0 };

	apPointPtr _startingPoint { nullptr };
	apPointPtr _destinationPoint { nullptr };
	const float _ratio = 1.0;

	cocos2d::DrawNode* _drawNode { nullptr };




};

} /* namespace arphomod */

#endif /* APFIELD_H_ */
