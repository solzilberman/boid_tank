#version 410

layout (location=0) in  vec4 aPos;
layout (location=1) in  vec3 norm;
layout (location=0) out float height;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    height = aPos.y;
    gl_Position = proj*view*model*aPos;
}  