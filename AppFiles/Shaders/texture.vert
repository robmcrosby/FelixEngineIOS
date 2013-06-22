attribute vec3 Pos;
attribute vec2 Tex;

uniform mat4 ProjMtx;
uniform mat4 ViewMtx;
uniform mat4 ModelMtx;
uniform mat3 TexMtx;

varying vec2 vTex;

void main(void)
{
   gl_Position = ProjMtx * ViewMtx * ModelMtx * vec4(Pos, 1);
   vTex = (TexMtx * vec3(Tex, 1)).xy;
}