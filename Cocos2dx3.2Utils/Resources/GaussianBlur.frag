varying vec4 v_fragmentColor;
varying vec2 v_texCoord;


void main() {
    
    vec2 blurSize = vec2((1.0/1024.0)*3.0,(1.0/768.0)*3.0);
    
    vec4 sum =vec4(0.0);
    
    sum += texture2D(CC_Texture0, v_texCoord -4.0 * blurSize) *0.063327;
    
    sum += texture2D(CC_Texture0, v_texCoord -3.0 * blurSize) *0.093095;
    
    sum += texture2D(CC_Texture0, v_texCoord -2.0 * blurSize) *0.122589;
    
    sum += texture2D(CC_Texture0, v_texCoord -1.0 * blurSize) *0.144599;
    
    sum += texture2D(CC_Texture0, v_texCoord                 ) *0.152781;
    
    sum += texture2D(CC_Texture0, v_texCoord +1.0 * blurSize) *0.144599;
    
    sum += texture2D(CC_Texture0, v_texCoord +2.0 * blurSize) *0.122589;
    
    sum += texture2D(CC_Texture0, v_texCoord +3.0 * blurSize) *0.093095;
    
    sum += texture2D(CC_Texture0, v_texCoord +4.0 * blurSize) *0.063327;
    
    
    gl_FragColor = sum * v_fragmentColor;
    
}