varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main(){
    vec4 c = texture2D(CC_Texture0, v_texCoord);
    //灰色效果的设计理念就是使原图片的RGB的值按比例减少
    gl_FragColor.xyz = vec3(0.2126*c.r + 0.7152*c.g + 0.0722*c.b);
    gl_FragColor.w = c.w;
}