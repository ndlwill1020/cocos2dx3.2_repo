varying vec2 v_texCoord;

uniform float u_h;
uniform float u_s;
uniform float u_v;

void main(){
    vec4 pixelColor = texture2D(CC_Texture0, v_texCoord);
    float r = pixelColor.r;
    float g = pixelColor.g;
    float b = pixelColor.b;
    float a = pixelColor.a;
    
    //语句块{} 可以定义与上面变量名相同的变量名 在这个语句快中生效（作用域不同）
    
    
    float temp_h;
    float temp_s;
    float temp_v;
    
    float maxValue = max(max(r, g), b);
    float minValue = min(min(r, g), b);
    //rgb-hsv
    //h
    if (maxValue == r) {
        temp_h = (g - b) / (maxValue - minValue) * 60.0;
    } else if (maxValue == g) {
        temp_h = 120.0 + (b - r) / (maxValue - minValue) * 60.0;
    } else if (maxValue == b) {
        temp_h = 240.0 + (r - g) / (maxValue -minValue) * 60.0;
    }
    
    //###不能写0 会报错 float不能和int做比较###
    if (temp_h < 0.0) {
        temp_h = temp_h + 360.0;
    }
    //s
    temp_s = (maxValue - minValue) / maxValue;
    //v
    temp_v = maxValue;
    
    //hsv-rgb
    temp_h = min(360.0, max(0.0, temp_h + u_h));
    temp_s = min(1.0, max(0.0, temp_s + u_s));
    temp_v = min(1.0, max(0.0, temp_v + u_v));
    
    vec4 outColor;
    
    float x;
    float y;
    float z;
    
    if (temp_s == 0.0) {
        r = g = b = temp_v;
    } else {
        temp_h /= 60.0;
        int iH = int(temp_h);
        float f = temp_h - float(iH);
        x = temp_v * (1.0 - temp_s);
        y = temp_v * (1.0 - temp_s * f);
        z = temp_v * (1.0 - temp_s * (1.0 - f));
        
        if (iH == 0) {
            r = temp_v;
            g = z;
            b = x;
        } else if (iH == 1) {
            r = y;
            g = temp_v;
            b = x;
        } else if (iH == 2) {
            r = x;
            g = temp_v;
            b = z;
        } else if (iH == 3) {
            r = x;
            g = y;
            b = temp_v;
        } else if (iH == 4) {
            r = z;
            g = x;
            b = temp_v;
        } else if (iH == 5) {
            r = temp_v;
            g = x;
            b = y;
        }
    }
    outColor = vec4(r, g, b, a);
    gl_FragColor = outColor;
    
}