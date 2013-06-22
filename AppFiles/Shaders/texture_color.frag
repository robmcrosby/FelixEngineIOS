varying mediump vec2 vTex;
varying mediump vec4 vColor;

uniform sampler2D Sampler0;

void main(void)
{
   gl_FragColor = texture2D(Sampler0, vTex) * vColor;
}
