varying mediump vec2 vTex;

uniform sampler2D Sampler0;

void main(void) {
   highp float d = texture2D(Sampler0, vTex).z;
   gl_FragColor = vec4(d, d, d, 1.0);
}
