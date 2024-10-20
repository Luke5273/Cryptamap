#version 430 core
out vec4 FragColor;
in vec2 fragPos;

uniform ivec2 boxSize;
uniform ivec2 mapSize;
uniform float scale;

float hash11(float p)
{
    p = fract(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

bool compare(vec2 a, vec2 b, vec2 epsilon)
{
    if(abs(a.x-b.x) <= epsilon.x && abs(a.y-b.y) <= epsilon.y)
    {
        return true;
    }
    return false;
}

void main()
{
    //float x = fract(fragPos.x * boxSize.x);
    //float y = fract(fragPos.y * boxSize.y);
    //vec3 col = compare(vec2(x,y), vec2(0.5f), vec2(0.45f)*min(sqrt(scale),1)) ? vec3(1.f) : vec3(0.f);
    FragColor = vec4(0.f, 1.f, 0.f, 1.f);
}