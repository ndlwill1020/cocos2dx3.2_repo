//
//  VirtualHandle.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/15.
//
//

#include "VirtualHandle.hpp"

VirtualHandle::VirtualHandle()
:rockerContainer(nullptr)
,rocker(nullptr)
,_a(nullptr)
,_b(nullptr)
,rockerRangeValue(300)
,rockerTouchID(-1)
,rockerWay(0)
,rockerLastPointX(0)
{

}

VirtualHandle::~VirtualHandle(){

}

bool VirtualHandle::init(){
    if (!Node::init()) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    
    
    _a = Button::create("stick.png");
    _a->setPosition(Vec2(visibleSize.width - _a->getContentSize().width, _a->getContentSize().height));
    this->addChild(_a);
    
    _b = Button::create("stick.png");
    _b->setPosition(Vec2(visibleSize.width - _b->getContentSize().width * 2.5, _b->getContentSize().height));
    this->addChild(_b);
    
    rockerContainer = Widget::create();
    rocker = Button::create("stick.png");
    
    rocker->setPosition(Vec2(rockerRangeValue / 2, rocker->getContentSize().height / 2));
    rocker->setTouchEnabled(false);
    rockerContainer->addChild(rocker);
    
    rockerContainer->setContentSize(Size(rockerRangeValue, rocker->getContentSize().height));
    rockerContainer->setPosition(Vec2(rockerRangeValue / 2, rocker->getContentSize().height));
    this->addChild(rockerContainer);
    
    _a->addTouchEventListener(std::bind(&VirtualHandle::touchEvent, this, std::placeholders::_1, std::placeholders::_2));
    _b->addTouchEventListener(CC_CALLBACK_2(VirtualHandle::touchEvent, this));
    
    EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event){
        auto touchPoint = touch->getLocation();
        log("%f %f",touchPoint.x, touchPoint.y);
        auto containerBound = rockerContainer->getBoundingBox();
        log("%f %f %f %f",containerBound.origin.x, containerBound.origin.y, containerBound.size.width, containerBound.size.height);
//        containerBound.origin = rockerContainer->convertToWorldSpace(Vec2::ZERO);//转成世界坐标 相对于世界坐标（0，0）的坐标
//        log("%f %f %f %f",containerBound.origin.x, containerBound.origin.y, containerBound.size.width, containerBound.size.height);
        
        if (containerBound.containsPoint(touchPoint)) {
            log("containPoint!!!!!!!!!!!!!!!!!!!!");
            rockerTouchID = touch->getID();
            rockerLastPointX = touchPoint.x;
            
            //左右20 不动
            if (fabsf(fabsf(touchPoint.x - containerBound.origin.x) - rockerRangeValue / 2) < 20.0f) {
                //原地不动
                
            } else if (touchPoint.x - containerBound.origin.x > rockerRangeValue / 2) {
                //右
                rockerWay = 2;
                _callback(VirtualHandleEvent::RIGHT);
                cancelRockerWay();
            } else {
                //左
                rockerWay = 1;
                _callback(VirtualHandleEvent::LEFT);
                cancelRockerWay();
            }
            moveRockerPosition(touchPoint);
            
            return true;
        }
        return false;
    };
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        if (rockerTouchID == -1 || rockerTouchID != touch->getID()) {
            return;
        }
        //略小的差值 忽略
        auto point = touch->getLocation();
        if (fabsf(rockerLastPointX - point.x) < 20.0f) {
            return;
        }
        
        log("touch move :%d", rockerWay);
        
        if (point.x > rockerLastPointX && rockerWay != 2) {
            log("move right!!!!!");
            _callback(VirtualHandleEvent::RIGHT);
            rockerWay = 2;
            cancelRockerWay();
        } else if (point.x < rockerLastPointX && rockerWay != 1) {
            log("move left!!!!!");
            _callback(VirtualHandleEvent::LEFT);
            rockerWay = 1;
            cancelRockerWay();
        }
        moveRockerPosition(point);
    };
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        if (rockerTouchID == -1 || rockerTouchID != touch->getID()) {
            return;
        }
        cancelRockerWay();
        rockerTouchID = -1;
        rockerLastPointX = 0;
        moveRockerPosition(Vec2(rockerRangeValue / 2, 0.0f));
    };
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        if (rockerTouchID == -1 || rockerTouchID != touch->getID()) {
            return;
        }
        cancelRockerWay();
        rockerTouchID = -1;
        rockerLastPointX = 0;
        moveRockerPosition(Vec2(rockerRangeValue / 2, 0.0f));
    };
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, rockerContainer);
    
    return true;
}

void VirtualHandle::moveRockerPosition(Vec2 point){
    log("%f", point.x);
    //auto value = rockerContainer->convertToNodeSpace(point).x;
    auto value = point.x;
    
    //边界判断
    if (value < rocker->getContentSize().width / 2) {
        value = rocker->getContentSize().width / 2;
    }
    
    if (value > rockerRangeValue - rocker->getContentSize().width / 2) {
        value = rockerRangeValue - rocker->getContentSize().width / 2;
    }
    
    rocker->stopAllActions();
    rocker->runAction(MoveTo::create(0.0f, Vec2(value, rocker->getPositionY())));
    
}

void VirtualHandle::cancelRockerWay(){
    if (rockerWay == 1) {
        _callback(VirtualHandleEvent::CANCEL_LEFT);
    } else if (rockerWay == 2) {
        _callback(VirtualHandleEvent::CANCEL_RIGHT);
    }
    rockerWay = 0;
}

void VirtualHandle::setCallback(std::function<void(VirtualHandleEvent)> callback){
    _callback = callback;
}

void VirtualHandle::touchEvent(Ref *obj, Widget::TouchEventType type){
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            if (obj == _a) {
                performCallback(VirtualHandleEvent::A);
            } else if (obj == _b) {
                performCallback(VirtualHandleEvent::B);
            }
            
            break;
        case Widget::TouchEventType::MOVED:
            
            break;
        case Widget::TouchEventType::ENDED:
            if (obj == _a) {
                performCallback(VirtualHandleEvent::CANCEL_A);
            } else if (obj == _b) {
                performCallback(VirtualHandleEvent::CANCEL_B);
            }
            break;
        case Widget::TouchEventType::CANCELED:
            if (obj == _a) {
                performCallback(VirtualHandleEvent::CANCEL_A);
            } else if (obj == _b) {
                performCallback(VirtualHandleEvent::CANCEL_B);
            }
            break;
            
        default:
            break;
    }
}
