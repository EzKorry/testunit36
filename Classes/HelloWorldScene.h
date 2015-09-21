#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "apTouchManager.h"

using namespace arphomod;

class BaseItem;
class HelloWorld : public cocos2d::Scene
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    std::shared_ptr<APTouchManager> getAPTouchManager();

    //void registerScene(BaseItem* item);
    // check if template class is derived from BaseItem
    template <typename T, typename = typename std::enable_if<
    		std::is_base_of<BaseItem, typename std::decay<T>::type>::value>::type>
    void registerScene(std::string);




private:
    std::shared_ptr<APTouchManager> _apTouchManager{nullptr};
    std::unordered_map<BaseItem*,int> _itemOrder;
    int _order{0};
    cocos2d::Vec2 _startingPoint, _startingTouchPoint;
    cocos2d::Node* _main{nullptr};


};

#endif // __HELLOWORLD_SCENE_H__
