attribute vec3 Pos;

uniform mat4 ProjMtx;
uniform mat4 ViewMtx;
uniform mat4 ModelMtx;

varying float vDepth;

void main(void)
{
   gl_Position = ProjMtx * ViewMtx * ModelMtx * vec4(Pos, 1);
   vDepth = gl_Position.z/gl_Position.w;
}
