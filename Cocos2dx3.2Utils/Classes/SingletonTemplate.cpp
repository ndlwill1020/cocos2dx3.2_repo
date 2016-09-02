//
//  SingletonTemplate.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/20.
//
//

#include "SingletonTemplate.hpp"

template <class T>
T* SingletonTemplate<T>::_instance = nullptr;

template <class T>
T* SingletonTemplate<T>::getInstance(){
    if (!_instance) {
        _instance = new T();
    }
    return _instance;
}

template <class T>
void SingletonTemplate<T>::destroyInstance(){
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}