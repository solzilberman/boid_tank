#version 410

in vec3 opos;
in vec3 onorm;
layout (location=0) out vec4 oColor;
uniform vec3 objColor;
void main()
{
    vec3 lightPos = vec3(0,500,0);
    vec3 lightColor = vec3(1.f,1.f,1.f); 
    
    //amb
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    //diff
    vec3 norm = normalize(onorm);
    vec3 ldir = normalize(lightPos - opos);  
    float diff = max(dot(norm, ldir), 0.0);
    vec3 diffuse = diff * lightColor;


    vec3 result = (ambient+diffuse) * objColor;
	oColor=vec4(result,1);
}
