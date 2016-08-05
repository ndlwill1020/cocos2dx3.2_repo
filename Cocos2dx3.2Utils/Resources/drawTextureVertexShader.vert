attribute vec3 a_position;
attribute vec4 a_color;
attribute vec2 a_uvCoord;

varying vec4 v_color;
varying vec2 v_uvCoord;

void main(){
    gl_Position = CC_MVPMatrix * vec4(a_position.xyz, 1);
	v_color = a_color;
    v_uvCoord = a_uvCoord;
}