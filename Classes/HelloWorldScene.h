#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include <map>
using namespace std;

struct LongTouch{
    int counter; // 倒计时，0 的时候引爆
    Ref* target;
    void upd();
};

struct MySprite : public Sprite{
    
    static MySprite* _target; static vector<Vec2> _pos; static vector<Node*> _pot;
    static double _dist2, _atan2;
    
    bool isMoveable, isZoomable, isRotatable;

   // map<Touch*, bool> state;
    
    
    void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    
    void init2(bool,bool,bool);
};

struct HelloWorld : public LayerColor{
    LongTouch* longTouch;
    
    static Scene* createScene(); virtual void update(float);
    virtual bool init(); CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
