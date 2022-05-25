#version 410

layout (location=0) in  vec4 pos;
layout (location=1) in  vec3 norm;
layout (location=0) out vec4 oPosCam;
layout (location=1) out vec3 oNormalCam;
uniform mat4  model;
uniform mat3  modelViewN;
uniform mat4  view;
uniform  mat4 proj;
void main()					
{     
	oNormalCam=normalize(modelViewN*norm);	  //n to camera coords
	oPosCam=view*model*pos;				  //v to camera coords	
	gl_Position = proj*view*model*pos;  //standard vertex out	          
}
