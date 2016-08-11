varying vec2 v_texCoord;



void main()

{
    
    // pixel size
    
    vec2 onePixel = vec2(1.0 / 480.0,1.0 / 320.0);
    
    vec4 color;
    
    color.rgb = vec3(0.5);
    
    color -= texture2D(CC_Texture0, v_texCoord - onePixel) *5.0;
    
    color += texture2D(CC_Texture0, v_texCoord + onePixel) *5.0;
    
    // grayscale
    
    color.rgb = vec3((color.r + color.g + color.b) /3.0);
    
    gl_FragColor = vec4(color.rgb,1);
    
}