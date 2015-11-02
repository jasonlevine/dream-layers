#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform sampler2DRect tex3;
uniform sampler2DRect tex4;
uniform sampler2DRect tex5;
uniform sampler2DRect tex6;
uniform sampler2DRect tex7;
uniform sampler2DRect tex8;
uniform sampler2DRect tex9;
uniform sampler2DRect tex10;
uniform sampler2DRect tex11;
uniform sampler2DRect tex12;
//uniform sampler2DRect tex13;


uniform sampler2DRect noiseTex;
uniform sampler2DRect diffTex;



uniform float time;
uniform vec2 windowSize;
uniform vec2 mousePos;
uniform float layerMix;

float wave(float low, float high, float speed){
    float sineWave  = sin(time * speed) * 0.5 + 0.5;
    return sineWave * (high - low) + low;
}

void main( void )
{
    
    float dist = distance(gl_FragCoord.xy, mousePos);
    
    int numTex = 13;
    vec4 col[13];
    
    
    col[0] = texture2DRect( tex0, gl_TexCoord[0].st );
    col[1] = texture2DRect( tex1, gl_TexCoord[0].st );
    col[2] = texture2DRect( tex2, gl_TexCoord[0].st );
    col[3] = texture2DRect( tex3, gl_TexCoord[0].st );
    col[4] = texture2DRect( tex4, gl_TexCoord[0].st );
    col[5] = texture2DRect( tex5, gl_TexCoord[0].st );
    col[6] = texture2DRect( tex6, gl_TexCoord[0].st );
    col[7] = texture2DRect( tex7, gl_TexCoord[0].st );
    col[8] = texture2DRect( tex8, gl_TexCoord[0].st );
    col[9] = texture2DRect( tex9, gl_TexCoord[0].st );
    col[10] = texture2DRect( tex10, gl_TexCoord[0].st );
    col[11] = texture2DRect( tex11, gl_TexCoord[0].st );
    col[12] = texture2DRect( tex12, gl_TexCoord[0].st );
  

    float r, g, b;
    
//    float waveVal = wave(0.0, 1.0, 0.25);
    vec4 noiseCol = texture2DRect( noiseTex, gl_TexCoord[0].st );
    vec4 diffCol = texture2DRect( diffTex, gl_TexCoord[0].st );
    
    float layerVal = (noiseCol.r + noiseCol.g + noiseCol.b)/3.0;// * diffCol.r * 0.9 + noiseCol.r *  0.1;
    
    float inc = 1.0 / (numTex);
    
    r = col[12].r;
    g = col[12].g;
    b = col[12].b;
    
    for (int i = 0; i < numTex-1; i++){
        if (layerVal >= i * inc && layerVal <= (i+1) * inc) {
            float val = (layerVal - i * inc) * numTex;
            r = col[i+1].r * val + col[i].r * (1.0 - val);
            g = col[i+1].g * val + col[i].g * (1.0 - val);
            b = col[i+1].b * val + col[i].b * (1.0 - val);
        }
    }
    
    
    
	gl_FragColor = vec4(r, g, b, 1.0);
    
//    gl_FragColor = noiseCol * diffCol;// + vec4(r, g, b, 1.0) * 0.5;
}