varying mediump vec2 vTex;

uniform sampler2D Sampler0;

void main(void) {
   gl_FragColor = texture2D(Sampler0, vTex);
}
