/*
 * asPoint.h
 *
 *  Created on: 2015. 4. 12.
 *      Author: EzKorry
 */

#ifndef ASPOINT_H_
#define ASPOINT_H_

#include <cocos2d.h>
#include <unordered_map>

class asPoint: public cocos2d::Node {
public:
	asPoint();
	virtual ~asPoint();
	CREATE_FUNC(asPoint);
	bool init() override;
	void drawRoute(asPoint* p);

private:
	// destination point, drawNode
	std::unordered_map<asPoint*, cocos2d::DrawNode*> _drawing;



};

#endif /* ASPOINT_H_ */
