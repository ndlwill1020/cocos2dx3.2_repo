varying vec4 v_color;
varying vec2 v_uvCoord;
//uniform vec4 u_color;
//uniform sampler2D CC_Texture0
void main(){
    //gl_FragColor = v_color * texture2D(CC_Texture0, v_uvCoord);
    gl_FragColor = texture2D(CC_Texture0, v_uvCoord);
}