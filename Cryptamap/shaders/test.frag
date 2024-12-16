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
    float x = floor(fragPos.x * boxSize.x);
    float y = floor(fragPos.y * boxSize.y);
    float z = floor(x+y);
    vec3 col = vec3(hash11(x),hash11(y),hash11(z));
    FragColor = vec4(col, 1.f);
}