//
//  XMLTools.cpp
//  BabyFarm
//
//  Created by macuser on 15/4/29.
//
//

#include "XMLTools.h"
#include "tinyxml2/tinyxml2.h"  //windows系统“\”表示目录#include "tinyxml2\tinyxml2.h"    mac“/”表示目录
using namespace tinyxml2;


XMLTools* XMLTools::xml_instance=nullptr;

XMLTools::XMLTools(){
    
}

XMLTools::~XMLTools(){
    
}


XMLTools* XMLTools::getInstance(){
    if(xml_instance==nullptr){
        xml_instance=new XMLTools();
    }
    return xml_instance;
}

void XMLTools::freeInstance(){
    if(xml_instance!=nullptr){
        CC_SAFE_DELETE(xml_instance);
    }
}


void XMLTools::makeXML(const char* fileName,std::map<std::string,std::vector<Value>> &data){
    std::string filePath=FileUtils::getInstance()->getWritablePath()+fileName;
    tinyxml2::XMLDocument* pDoc=new tinyxml2::XMLDocument();
    XMLDeclaration* pDel=pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    pDoc->LinkEndChild(pDel);
    
    XMLElement* plistElement=pDoc->NewElement("plist");//const char*
    plistElement->SetAttribute("version","1.0");
    pDoc->LinkEndChild(plistElement);
    
    XMLComment* pCom=pDoc->NewComment("this is xml comment");
    plistElement->LinkEndChild(pCom);
    
    XMLElement* arrayElement=pDoc->NewElement("array");
    plistElement->LinkEndChild(arrayElement);
    
    std::map<std::string,std::vector<Value>>::iterator it;
    for(it=data.begin();it!=data.end();it++){
        XMLElement* dicElement=pDoc->NewElement("dict");
        arrayElement->LinkEndChild(dicElement);
        
        XMLElement* keyElement=pDoc->NewElement("key");
        keyElement->LinkEndChild(pDoc->NewText((it->first).c_str()));
        dicElement->LinkEndChild(keyElement);
        
        XMLElement* valuesElement=pDoc->NewElement("array");
        dicElement->LinkEndChild(valuesElement);
        
        //XMLElement* valueElement=nullptr;
        for(int i=0;i<3;i++){
            auto temp=(it->second).at(i).asString();
            /*if(0==i){
             valueElement=pDoc->NewElement("id");
             }else if(1==i){
             valueElement=pDoc->NewElement("x");
             }else if(2==i){
             valueElement=pDoc->NewElement("y");
             }*/
            XMLElement* valueElement=pDoc->NewElement("string");//ValueVector÷–µƒ±Í«©√˚±ÿ–Î“ª—˘
            valueElement->LinkEndChild(pDoc->NewText(temp.c_str()));
            valuesElement->LinkEndChild(valueElement);
        }
    }
    
    pDoc->SaveFile(filePath.c_str());
    pDoc->Print();
    delete pDoc;
}

std::map<std::string,std::vector<Value>> XMLTools::parseXML(const char* fileName){
    std::string filePath=FileUtils::getInstance()->getWritablePath()+fileName;
    ValueVector vectors=FileUtils::getInstance()->getValueVectorFromFile(filePath);
    
    
    
    std::map<std::string,std::vector<Value>> temp;
    std::vector<Value> datas;
    char name[16];
    
    int i_number=vectors.size();
    log("%d",i_number);
    
    for(int i=0;i<i_number;i++){
        sprintf(name,"data_%.2d",i);
        auto valueMapDatas=vectors.at(i).asValueMap();
        ValueVector valueVectorDatas=valueMapDatas.at(name).asValueVector();
        int number=valueVectorDatas.size();
        log("number:%d",number);
        datas.clear();
        for(int j=0;j<number;j++){
            auto data=valueVectorDatas.at(j);
            datas.push_back(data);
        }
        temp[name]=datas;
    }
    
    return temp;
}
