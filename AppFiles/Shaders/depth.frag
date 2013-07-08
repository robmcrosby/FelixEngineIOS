//varying mediump vec2 vTex;

//uniform sampler2D Sampler0;

varying mediump float vDepth;

void main(void) {
   //highp float d = texture2D(Sampler0, vTex).r;
   
   gl_FragColor = vec4(vDepth, vDepth, vDepth, 1.0);
}
