//
//  SuperRichText.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/11.
//
//

#ifndef SuperRichText_hpp
#define SuperRichText_hpp

#include "cocos2d.h"
#include "CocosGUI.h"
#include "tinyxml2/tinyxml2.h"
USING_NS_CC;
using namespace ui;
using namespace tinyxml2;

struct FontInfo{
    std::string fontName;
    float fontSize;
    Color3B fontColor;
    GLubyte fontOpacity;
};

class SuperRichText : public Widget{
protected:
    SuperRichText();
    ~SuperRichText();
public:
    CREATE_FUNC(SuperRichText);
    void renderHtml(const char* htmlStr);
    void renderNode(tinyxml2::XMLNode* node);
    void addNewLine();
    void updateLineList();
private:
    inline GLubyte charToNumber(char c){
        GLubyte n = c - '0';//将数字字符转换成数字
        if (n > 9) {//大于9的表示字母，将字母转换成10-15的值
            n = c - (c >= 'a' ? 'a' : 'A') + 10;
        }
        return n;
    }
    inline Color3B charToColor3B(const char* str){
        Color3B color(0, 0, 0);
        if (strlen(str) == 6) {
            //公式： 十进制 ＝ charToNumber() * 第n位的权值(16^1表示16的1次方) + charToNumber() * 第n位的权值(16^0)
            color.r = charToNumber(str[0]) * 16 + charToNumber(str[1]);
            color.g = charToNumber(str[2]) * 16 + charToNumber(str[3]);
            color.b = charToNumber(str[4]) * 16 + charToNumber(str[5]);
        }
        return color;
    }
private:
    std::vector<RichText *> lineList;
    std::vector<FontInfo> fontList;
    RichText* line;
};

#endif /* SuperRichText_hpp */
