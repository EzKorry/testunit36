/*
 * apPoint.h
 *
 *  Created on: 2015. 4. 24.
 *      Author: EzKorry
 */

#ifndef APPOINT_H_
#define APPOINT_H_

#include "cocos2d.h"
USING_NS_CC;



namespace arphomod {

class apPoint;
using apPointPtr = std::shared_ptr<apPoint>;

class apPoint {
public:
	apPoint(float x, float y);
	virtual ~apPoint();

	static std::shared_ptr<apPoint> create(float x, float y) {
		auto p = std::make_shared<apPoint>(x,y);
		return p;
	}
	float x{0.0}, y{0.0};

private:


};

} /* namespace arphomod */

#endif /* APPOINT_H_ */
