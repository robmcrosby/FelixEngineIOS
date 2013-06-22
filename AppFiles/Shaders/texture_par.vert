attribute vec2 Tex;
attribute vec3 Pos;

uniform mat4 ProjMtx;
uniform mat4 ViewMtx;
uniform mat4 ModelMtx;
uniform vec3 CamLoc;

uniform vec4 Particles[96];

varying vec2 vTex;
varying vec4 vColor;

mat3 transpose(mat3 m)
{
   mat3 ret;
   ret[0].y = m[1].x;
   ret[0].z = m[2].x;
   ret[1].x = m[0].y;
   ret[1].z = m[2].y;
   ret[2].x = m[0].z;
   ret[2].y = m[1].z;
   return ret;
}

mat3 rotate(vec3 fwd, vec3 up)
{
   mat3 rot;
   vec3 bi;
   
   fwd = normalize(fwd);
   bi = normalize(cross(fwd, up));
   up = cross(bi, fwd);
   
   rot[0] = bi;
   rot[1] = up;
   rot[2] = fwd;
   
   return rot;
}

void main(void)
{
   int ndx = int(Pos.z)*3;
   vec4 center = Particles[ndx];
   vec4 data = Particles[ndx+1];
   vec3 pos = vec3(Pos.xy, 0) * center.w;
   
   center.w = 1.0;
   center = ModelMtx * center;
   
   pos = rotate(CamLoc - center.xyz, vec3(0, 1, 0)) * pos;
   pos += center.xyz;
   
   vColor = Particles[ndx+2];
   gl_Position = ProjMtx * ViewMtx * vec4(pos, 1);
   
   vTex = Tex * data.z + data.xy;
}