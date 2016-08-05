//
//  SingletonTemplate.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/20.
//
//

#ifndef SingletonTemplate_hpp
#define SingletonTemplate_hpp

template <class T>
class SingletonTemplate {
private:
    static T* _instance;
    
public:
    static T* getInstance();
    static void destroyInstance();
protected:
    SingletonTemplate(){};
    ~SingletonTemplate(){};
};

#endif /* SingletonTemplate_hpp */
