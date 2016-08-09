attribute vec4 a_position;
attribute vec4 a_color;

varying vec4 v_color;

void main(){
	gl_Position = CC_MVPMatrix * a_position;
	v_color = a_color;
}