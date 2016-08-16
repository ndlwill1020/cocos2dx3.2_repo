//
//  VirtualHandle.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/15.
//
//

#ifndef VirtualHandle_hpp
#define VirtualHandle_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

enum class VirtualHandleEvent{
    A,
    B,
    LEFT,
    RIGHT,
    
    CANCEL_A,
    CANCEL_B,
    CANCEL_LEFT,
    CANCEL_RIGHT
};

//只有左右的vh
class VirtualHandle : public Node{
public:
    
    
    
    CREATE_FUNC(VirtualHandle);
    virtual bool init();
    //_callback的set方法
    void setCallback(std::function<void(VirtualHandleEvent)> callback);
protected:
    VirtualHandle();
    ~VirtualHandle();
private:
    void touchEvent(Ref *obj, Widget::TouchEventType type);
    inline void performCallback(VirtualHandleEvent event){
        if (_callback) {
            _callback(event);
        }
    };
    
    inline void moveRockerPosition(Vec2 point);
    
    void cancelRockerWay();
private:
    Widget *rockerContainer;//锚点在中心点
    Button *rocker;
    Button *_a;
    Button *_b;
    
    std::function<void(VirtualHandleEvent)> _callback;
    
    int rockerRangeValue;
    
    int rockerTouchID;
    int rockerWay;//0无方向 1左 2右
    
    int rockerLastPointX;//纪录x轴的坐标
    
    
};

#endif /* VirtualHandle_hpp */
