#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Audio.h"

#include "Template.h"
#include "Global.h"
USING_NS_CC;

void addAroundEdge(Sprite* t){
    Size s = t->getContentSize();
    auto e = DrawNode::create();
    
    auto radius = 1;
    auto color = Color4F(0, 0, 0, 1.0f);
    
    auto lb = Vec2(0, 0), rb = Vec2(s.width, 0);
    auto lt = Vec2(0, s.height), rt = Vec2(s.width, s.height);
    
    e->drawSegment(lb, rb, radius, color);
    e->drawSegment(lb, lt, radius, color);
    e->drawSegment(rb, rt, radius, color);
    e->drawSegment(lt, rt, radius, color);
    e->setName("edge");
    t->addChild(e);
}

/*
void MySprite::init(string s){
}
*/

vector<Vec2> MySprite::_pos;
vector<Node*> MySprite::_pot;
MySprite* MySprite::_target;
DB MySprite::_atan2, MySprite::_dist2;
 
void MySprite::init2(bool _isMoveable = 1, bool _isZoomable = 1, bool _isRotatable = 1){
    
    isMoveable = _isMoveable;
    isZoomable = _isZoomable;
    isRotatable = _isRotatable;
    
    auto listener = EventListenerTouchAllAtOnce::create();
    
    listener->onTouchesBegan = CC_CALLBACK_2(MySprite::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(MySprite::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(MySprite::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), this);
    
    addAroundEdge(this);
}


static const Color3B* s_TouchColors[5] = {
    &Color3B::YELLOW,
    &Color3B::BLUE,
    &Color3B::GREEN,
    &Color3B::RED,
    &Color3B::MAGENTA
};

struct TouchPoint : public Node
{
    
    
    TouchPoint(const Vec2 &touchPoint, const Color3B &touchColor)
    {
        DrawNode* drawNode = DrawNode::create();
        auto s = Director::getInstance()->getWinSize();
        Color4F color(touchColor.r/255.0f, touchColor.g/255.0f, touchColor.b/255.0f, 1.0f);
        
        /*drawNode->drawLine(Vec2(-s.width, touchPoint.y), Vec2(s.width, touchPoint.y), color);
        drawNode->drawLine(Vec2(touchPoint.x, -s.height), Vec2(touchPoint.x, s.height), color);
        drawNode->drawDot(touchPoint, 3, color);*/
        
        drawNode->drawLine(Vec2(-s.width, 0), Vec2(s.width, 0), color);
        drawNode->drawLine(Vec2(0, -s.height), Vec2(0, s.height), color);
        drawNode->drawDot(Vec2(0, 0), 3, color);
        addChild(drawNode);
    }
    
    static TouchPoint* touchPointWithParent(Node* pParent, const Vec2 &touchPoint, const Color3B &touchColor)
    {
        auto pRet = new (std::nothrow) TouchPoint(touchPoint, touchColor);
        pRet->setContentSize(pParent->getContentSize());
        pRet->setAnchorPoint(Vec2(0.0f, 0.0f));
        pRet->autorelease();
        return pRet;
    }
};


void MySprite::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
    auto target = static_cast<MySprite*>(event->getCurrentTarget());

    for (auto &touch: touches)
    {
        auto location = touch->getLocation();
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        if (!rect.containsPoint(locationInNode)){
            return;
        }
    }
    
    if (_target != nullptr && _target != target) return;
    
    for (auto &touch: touches)
    {
        auto location = touch->getLocation();
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        
        //点击范围判断检测
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        auto touchPoint = TouchPoint::touchPointWithParent(this, location, *s_TouchColors[touch->getID()%5]);
        touchPoint->setName("touchPoint");
        Global::game->addChild(touchPoint);
        touchPoint->setPosition(location);
        
        _pos.PB(location), _pot.PB(touchPoint);
    }
    
    _target = target;
    
    if (_pos.size() == 2){
        //log("%.2f %.2f %.2f %.2f\n", _pos[0].x, _pos[0].y, _pos[1].x, _pos[1].y);
        _dist2 = _pos[0].getDistance(_pos[1]);
        Vec2 d = _pos[1] - _pos[0];
        _atan2 = atan2(d.y, d.x);
    }
}

void MySprite::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    
    auto target = static_cast<MySprite*>(event->getCurrentTarget());
    if (target != _target) return;
    if (_pos.empty()) return;
    
    vector<Vec2> new_pos;
    
    for( auto &item: touches)
    {
        
        //if (state[item] == false) continue;
        
        auto touch = item;

        auto location = touch->getLocation();
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        //点击范围判断检测
        
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        //if (!rect.containsPoint(locationInNode)) continue;
        new_pos.PB(location);
    }
    
    if (_pos.size() != new_pos.size()) return;
    
    REP(i, _pos.size()){
        _pot[i]->setPosition(new_pos[i]);
    }
    
    if (_pos.size() == 1){ // 平移...
//        log("%.2f %.2f  %.2f %.2f", target->getPosition().x, target->getPosition().y, (t->getPosition() - target->_pos).x , (t->getPosition() - target->_pos).y );
        target->setPosition(target->getPosition() + (new_pos[0] - _pos[0]));
        _pos = new_pos;
    }
    else if (_pos.size() == 2){

        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        auto new_dist2 = new_pos[0].getDistance(new_pos[1]);
        target->setScale((DB)target->getScale()/_dist2*new_dist2);
        _dist2 = new_dist2;
        //log("%.2f %.2f %.2f\n", _dist2, new_dist2, target->getScale()/_dist2*new_dist2);

        Vec2 d = new_pos[1] - new_pos[0];
        DB new_atan2 = atan2(d.y, d.x);
        target->setRotation( target->getRotation() -  (new_atan2 - _atan2) / PI * 180);
        _atan2 = new_atan2;
    }
}

void MySprite::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    
    auto target = static_cast<MySprite*>(event->getCurrentTarget());
    
    while (Global::game->getChildByName("touchPoint") != nullptr){
        Global::game->getChildByName("touchPoint")->removeFromParent();
    }
    _pos.clear(); _pot.clear();
    _target = nullptr;
    
    
    /*for ( auto &item: touches )
    {
        //if (state[item] == false) continue;
        
        auto touch = item;        auto pTP = s_map.at(target);
       
        if (pTP == nullptr) continue;
        
        pTP->removeFromParent();
        s_map.erase(target);
    }*/
    
    
    /*for ( auto &item: touches ){
        state.erase(item);
    }*/
}

void MySprite::onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event)
{
    onTouchesEnded(touches, event);
}




void LongTouch::upd(){
    if (counter > 0){
        if (--counter == 0){
            Audio::playSE("coin.mp3");
        }
    }
}

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

void HelloWorld::update(float dt){
    longTouch->upd();
    return;
}


bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
    {
        return false;
    }
    
    
    Global::game = this;
    
    longTouch = new LongTouch;
    
    auto size = Director::getInstance()->getWinSize();
    
    auto t1 = static_cast<MySprite*>(Sprite::create("star_on.png")); t1->init2();
    t1->setPosition(size / 2); addChild(t1);
    
    auto t2 = static_cast<MySprite*>(Sprite::create("star_on.png")); t2->init2();
    t2->setPosition(Vec2(size.width / 2 - 100, size.height / 2 - 100) ); addChild(t2);
    
    
    auto t3 = static_cast<MySprite*>(Sprite::create("star_on.png")); t3->init2();
    t3->setPosition(Vec2(size.width / 2 + 100, size.height / 2 + 100) ); addChild(t3);
    
    
    /*
    auto _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    
    _touchListener->onTouchBegan = [&](Touch* touch, Event* event){
        
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        //点击范围判断检测
        if (rect.containsPoint(locationInNode))
        {
            longTouch->counter = 20;
            longTouch->target = target;
            return true;
        }
        return false;
    };
    
    _touchListener->onTouchEnded = [&](Touch* touch, Event* event){
        if (longTouch->counter == 0) return;
        longTouch->counter = 0;
        Audio::playSE("error.mp3");
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener->clone(), t);
    scheduleUpdate(); */
    return true;
}



