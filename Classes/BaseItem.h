/*
 * BaseItem.h
 *
 *  Created on: 2014. 12. 24.
 *      Author: EzKorry
 */

#ifndef BASEITEM_H_
#define BASEITEM_H_

#include <cocos2d.h>
#include "apTouchManager.h"
using namespace arphomod;

// derived class must have "CREATE_FUNC"
class BaseItem: public cocos2d::Scene {
public:
	BaseItem();

	// just return title string
	virtual std::string getTitle() const = 0;

	// initializing from derived class
	virtual bool itemInit() = 0;

	template<typename T>
	static T* create(std::shared_ptr<APTouchManager> p) {

		auto pRet = new T();
		if (pRet && pRet->init(p)) {
			pRet->autorelease();
			//cocos2d::log("baseItem create success!");
			return pRet;
		} else {
			delete pRet;
			pRet = nullptr;
			//cocos2d::log("baseItem create fail");
			return nullptr;

		}

	}


	virtual bool init(std::shared_ptr<APTouchManager> apTouchManager);


	std::shared_ptr<APTouchManager> getAPTouchManager();

	virtual ~BaseItem();

private:
	std::shared_ptr<APTouchManager> _apTouchManager{nullptr};
};

#endif /* BASEITEM_H_ */
