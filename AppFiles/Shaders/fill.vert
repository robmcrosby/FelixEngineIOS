attribute vec3 Pos;
attribute vec2 Tex;

varying vec2 vTex;

void main(void) {
   gl_Position = vec4(Pos, 1) * vec4(2, 2, 1, 1);
   vTex = Tex;
}