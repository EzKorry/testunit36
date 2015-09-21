/*
 * BaseItem.cpp
 *
 *  Created on: 2014. 12. 24.
 *      Author: EzKorry
 */

#include "BaseItem.h"
#include "apTouchManager.h"

USING_NS_CC;
using namespace arphomod;

BaseItem::BaseItem() {
	// TODO Auto-generated constructor stub

}

BaseItem::~BaseItem() {
	// TODO Auto-generated destructor stub
}


bool BaseItem::init(std::shared_ptr<APTouchManager> apTouchManager) {

	// if null, exit
	if(apTouchManager == nullptr) return false;

	// set touchManager
	this->_apTouchManager = apTouchManager;

	// set back to menu button
	auto backButton = Sprite::create("back.png");
	backButton->setPosition(Vec2(100, 100));
	this->addChild(backButton, 9999, "backButton");

	_apTouchManager->registerNode(backButton, APTouchManager::createDefaultChecker(backButton));
	_apTouchManager->setOrder(backButton, 9999);
	auto tempAp = _apTouchManager;
	_apTouchManager->setBehavior(backButton, [tempAp]() {

		cocos2d::Director::getInstance()->popScene();
		tempAp->cancelAllTouch();

	}, APTouchType::Began);

	// set label
	auto labelTitle = Label::createWithTTF(this->getTitle(), "fonts/NanumBarunGothicLight.ttf", 40);
	labelTitle->setAlignment(TextHAlignment::LEFT);
	labelTitle->setAnchorPoint(Vec2(0,1));
	labelTitle ->setPosition(Vec2(20,Director::getInstance()->getVisibleSize().height - 50));
	this->addChild(labelTitle, 1, "labelTitle");

	// derived class initializing
	if(itemInit() == false) {
		return false;
	}

	// create return

	return true;


}

std::shared_ptr<APTouchManager> BaseItem::getAPTouchManager() {
	return _apTouchManager;
}
