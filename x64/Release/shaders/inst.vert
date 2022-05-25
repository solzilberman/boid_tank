#version 410

layout (location=0) in  vec4 pos;
layout (location=1) in  vec3 norm;
layout (location=2) in  vec4 m0;
layout (location=3) in  vec4 m1;
layout (location=4) in  vec4 m2;
layout (location=5) in  vec4 m3;
out vec3 opos;  
out vec3 onorm;

uniform mat4  view;
uniform  mat4 proj;

void main()					
{     
	mat4 mod = {
	m0,m1,m2,m3
	};
	gl_Position = proj*view*mod*pos;  //standard vertex out	   
	onorm = norm;
	opos = vec3(mod*pos);
}
