//
//  XMLTools.h
//  BabyFarm
//
//  Created by macuser on 15/4/29.
//
//

#ifndef __BabyFarm__XMLTools__
#define __BabyFarm__XMLTools__

#include "cocos2d.h"
USING_NS_CC;

class XMLTools{
public:
    static XMLTools* getInstance();
    static void freeInstance();
    //typedef std::vector<Value> ValueVector;
    void makeXML(const char* fileName,std::map<std::string,std::vector<Value>> &data);
    std::map<std::string,std::vector<Value>> parseXML(const char* fileName);
private:
    XMLTools();
    ~XMLTools();
    static XMLTools* xml_instance;
};

#endif /* defined(__BabyFarm__XMLTools__) */
