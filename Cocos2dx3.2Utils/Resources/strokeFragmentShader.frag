uniform float u_strokeWidth;
uniform vec3 u_strokeColor;
uniform vec2 u_texSize;

uniform vec4 cosArray[3];
uniform vec4 sinArray[3];

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;


//0 30 60 90 120 150 180 210 240 270 300 330角度
//const float cosArray[12] = {1, 0.866, 0.5, 0, -0.5, -0.866, -0.1, -0.866, -0.5, 0, 0.5, 0.866};
//const float sinArray[12] = {0, 0.5, 0.866, 1, 0.866, 0.5, 0, -0.5, -0.866, -1, -0.866, -0.5};


//判断当前像素点在这个角度上距离为outlineSize那一点是不是透明
int isTransparentPixelWithAngleIndex(int index, int subIndex){
    int count = 0;
    //texture2D接收的是一个0~1的纹理坐标，而不是像素坐标
    float alpha = texture2D(CC_Texture0, vec2(v_texCoord.x + u_strokeWidth * cosArray[index][subIndex] / u_texSize.x, v_texCoord.y + u_strokeWidth * sinArray[index][subIndex] / u_texSize.y)).a;
    
    if (alpha >= 0.5) {
        count = 1;
    }
    
    return count;
}




void main(){
    vec4 pixelColor = texture2D(CC_Texture0, v_texCoord);
    
    if (pixelColor.a >= 0.5) {
        gl_FragColor = v_fragmentColor * pixelColor;
        return;
    }
    
    int strokeCount = 0;
    strokeCount += isTransparentPixelWithAngleIndex(0, 0);
    strokeCount += isTransparentPixelWithAngleIndex(0, 1);
    strokeCount += isTransparentPixelWithAngleIndex(0, 2);
    strokeCount += isTransparentPixelWithAngleIndex(0, 3);
    strokeCount += isTransparentPixelWithAngleIndex(1, 0);
    strokeCount += isTransparentPixelWithAngleIndex(1, 1);
    strokeCount += isTransparentPixelWithAngleIndex(1, 2);
    strokeCount += isTransparentPixelWithAngleIndex(1, 3);
    strokeCount += isTransparentPixelWithAngleIndex(2, 0);
    strokeCount += isTransparentPixelWithAngleIndex(2, 1);
    strokeCount += isTransparentPixelWithAngleIndex(2, 2);
    strokeCount += isTransparentPixelWithAngleIndex(2, 3);
    
    if (strokeCount > 0) {
        pixelColor.rgb = u_strokeColor;
        pixelColor.a = 1.0;
    }
    
    gl_FragColor = v_fragmentColor * pixelColor;
}
