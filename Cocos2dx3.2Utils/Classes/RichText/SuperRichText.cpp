//
//  SuperRichText.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/11.
//
//

#include "SuperRichText.hpp"

SuperRichText::SuperRichText(){
    FontInfo defaultFont;
    defaultFont.fontColor = Color3B(255, 255, 255);
    defaultFont.fontSize = 24.0f;
    defaultFont.fontOpacity = 255;
    defaultFont.fontName = "";
    
    fontList.push_back(defaultFont);
    addNewLine();
}

SuperRichText::~SuperRichText(){

}


void SuperRichText::renderHtml(const char* htmlStr){
    tinyxml2::XMLDocument doc;
    doc.Parse(htmlStr);
    renderNode(doc.FirstChild());
    updateLineList();
}

void SuperRichText::renderNode(tinyxml2::XMLNode* node){
    while (node != nullptr) {
        if (node->ToText()) {
            log("Text!!!!!!");
            XMLText *text = node->ToText();
            //换行\n    返回行首\r
            //编写xml可能会包含回车换行符，我们这边用br来表示。过滤回车换行符（移除回车换行符），需要将utf-8转成utf-16
            std::u16string textStr;
            StringUtils::UTF8ToUTF16(text->Value(), textStr);
            
            std::u16string::size_type pos = 0;
            pos = textStr.find('\n');
            //不等于终点
            while (pos != std::u16string::npos) {
                textStr.erase(pos, 1);
                pos = textStr.find('\n', pos);
            }
            
            pos = 0;
            pos = textStr.find('\r');
            while (pos != std::u16string::npos) {
                textStr.erase(pos, 1);
                pos = textStr.find('\r', pos);
            }
            
            std::string utf8Text;
            StringUtils::UTF16ToUTF8(textStr, utf8Text);
            auto fontInfo = fontList[fontList.size() - 1];
            auto richElementText = RichElementText::create(0, fontInfo.fontColor, fontInfo.fontOpacity, utf8Text, fontInfo.fontName, fontInfo.fontSize);
            line->pushBackElement(richElementText);
            
            
        } else if(node->ToElement()){
            log("Element!!!!!!");
            XMLElement* ele = node->ToElement();
            std::string eleName = ele->Name();
            //将字母转换成大写字母
            std::transform(eleName.begin(), eleName.end(), eleName.begin(), ::toupper);
            
            if (eleName == "FONT") {
                log("font title!!!!!");
                //解析属性
                auto attr = ele->FirstAttribute();
                //取上次的FontInfo
                FontInfo fontInfo = fontList[fontList.size() - 1];
                while (attr != nullptr) {
                    std::string attrName = attr->Name();
                    std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::toupper);
                    
                    if (attrName == "COLOR") {
                        fontInfo.fontColor = charToColor3B(attr->Value());
                    } else if (attrName == "SIZE")
                    {
                        fontInfo.fontSize = attr->FloatValue();
                    } else if (attrName == "OPACTITY")
                    {
                        fontInfo.fontOpacity = attr->IntValue();
                    } else if (attrName == "NAME")
                    {
                        fontInfo.fontName = attr->Value();
                    }
                    attr = attr->Next();
                }
                fontList.push_back(fontInfo);//添加新字体 font标签的子集font用父font标签的 fontInfo
                
                //其他标签是font标签的子集 递归渲染这个节点里面的节点 font里面的Text“你好”和其他标签属于兄弟节点
                renderNode(ele->FirstChild());
                fontList.pop_back();//移除新字体
            } else if (eleName == "IMAGE") {
                log("image title!!!!!");
                auto attr = ele->FirstAttribute();
                const char* src;
                Color3B color(255, 255, 255);
                GLubyte opacity = 255;
                while (attr != nullptr) {
                    std::string attrName = attr->Name();
                    std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::toupper);
                    
                    if (attrName == "SRC") {
                        src = attr->Value();
                    } else if (attrName == "COLOR")
                    {
                        color = charToColor3B(attr->Value());
                    } else if (attrName == "OPACTITY")//不透明度
                    {
                        opacity = attr->IntValue();
                    }
                    attr = attr->Next();
                }
                auto richElementImage = RichElementImage::create(0, color, opacity, src);
                line->pushBackElement(richElementImage);
            } else if (eleName == "BR") {
                log("br title!!!!!");
                addNewLine();
            }
        }
        node = node->NextSibling();
    }
}

//遇到br标签
void SuperRichText::addNewLine(){
    line = RichText::create();
    lineList.push_back(line);
    this->addChild(line);
}

//调整不同行的位置
void SuperRichText::updateLineList(){
    float maxWidth = 0.0f, maxHeight = 0.0f;
    for (auto it = lineList.end() - 1; it > lineList.begin() - 1; it--) {
        (*it)->formatText();
        auto size = (*it)->getContentSize();
        (*it)->setPosition(Vec2(size.width / 2, maxHeight));
        if (size.width > maxWidth) {
            maxWidth = size.width;
        }
        maxHeight += size.height;
    }
    //设置超级富文本大小
    this->setContentSize(Size(maxWidth, maxHeight));
}