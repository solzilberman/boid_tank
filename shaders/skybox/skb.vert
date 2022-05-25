#version 410

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aPos;
    vec4 pos = proj * view  * vec4(aPos*10.f, 1.0);
    gl_Position = pos.xyww;
}  