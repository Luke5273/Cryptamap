#version 430 core
out vec4 FragColor;
in vec2 fragPos;
in vec4 gl_FragCoord;

uniform ivec2 boxSize;
uniform ivec2 mapSize;
uniform float scale;
uniform vec2 translate;
uniform float widgetWidth;

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
    vec2 coord = fragPos * mat2(1.f, 0.f, 0.f, float(mapSize.x)/mapSize.x);
    float x = fract(coord.x * boxSize.x);
    float y = fract(coord.y * boxSize.y);
    if(compare(vec2(x,y), vec2(0.5f), vec2(0.45)*widgetWidth/500.f)) 
    {
        discard;
    }
    vec3 col = vec3(0.f);
    FragColor = vec4(col, 1.f);
}