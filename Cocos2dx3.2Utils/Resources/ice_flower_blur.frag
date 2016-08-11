varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
float rand(vec2 co)

{
    
    return fract(sin(dot(co.xy ,vec2(100,100))) +
                 
                 cos(dot(co.xy ,vec2(50,50))) *5.0);
    
}


void main()

{
    
    vec2 rnd = vec2(0.0);
    
    rnd = vec2(rand(v_texCoord),rand(v_texCoord));
    
    gl_FragColor = texture2D(CC_Texture0, v_texCoord+rnd*0.02);
    
}