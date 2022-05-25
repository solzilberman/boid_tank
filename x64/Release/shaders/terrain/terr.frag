#version 410

layout (location=0) in float height;

out vec4 FragColor;

void main()
{   
    float inMin = -500.f;
    float inMax = 500.f;
    float h = 0.f + (1.f) * (height-inMin) / (inMax-inMin);
    float r,g,b;
    if(h >= 0 && h <= .5f){
        g = h/.5f;
        r = 1.f-g;
        b = 0.f;
    } else {
        r = 0.f;
        b = (h-.49f)/.5f;
        g= 1.f-b;
    }

    FragColor = vec4(r,g,b,1);
}