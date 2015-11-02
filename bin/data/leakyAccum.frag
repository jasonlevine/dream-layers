#version 120

uniform sampler2DRect accumTex;
uniform sampler2DRect newTex;

void main( void )
{
    vec4 accumCol = texture2DRect( accumTex, gl_TexCoord[0].st );
    vec4 newCol = texture2DRect( newTex, gl_TexCoord[0].st );
    
    float decay = 0.9;
    float accumScale = 0.6;
    
    accumCol.r *= decay;
    accumCol.g *= decay;
    accumCol.b *= decay;
    
    accumCol.r += newCol.r * accumScale;
    accumCol.g += newCol.g * accumScale;
    accumCol.b += newCol.b * accumScale;
    
//    if (accumCol.r > 1.0) accumCol.r = 1.0;
//    if (accumCol.g > 1.0) accumCol.g = 1.0;
//    if (accumCol.b > 1.0) accumCol.b = 1.0;

    
    accumCol.a = 1.0;
    accumCol = newCol;
    
    gl_FragColor = accumCol;

}