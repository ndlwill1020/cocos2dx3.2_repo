varying vec2 v_texCoord;

vec4 composite(vec4 texColor, vec4 shadowColor)
{
    return texColor + (1.0 - texColor.a) * shadowColor;
}

void main(){
    vec2 shadowOffset = vec2(0.03, 0.03);
    //vec2 shadowOffset = vec2(0.5, 0.5);
    //vec2 shadowOffset = vec2(1.0, 1.0);
    vec4 textureColor = texture2D(CC_Texture0, v_texCoord + shadowOffset);
    float shadowMask = texture2D(CC_Texture0, v_texCoord).a;
    const float shadowOpacity = 0.5;
    vec4 shadowColor = vec4(0, 0, 0, shadowMask * shadowOpacity);
    gl_FragColor = composite(textureColor, shadowColor);
}