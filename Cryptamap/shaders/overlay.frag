#version 430 core
out vec4 FragColor;
in vec2 fragPos;
in vec4 gl_FragCoord;

uniform ivec2 boxSize;
uniform ivec2 mapSize;
uniform float scale;
uniform vec2 translate;
uniform float aspectRatio;

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
    vec2 coord = gl_FragCoord.xy/mapSize - translate/2;
    coord = vec2(coord.x,coord.y/aspectRatio);
    //float x = fract(fragPos.x * boxSize.x);
    float x = fract(coord.x * boxSize.x);
    //float y = fract(fragPos.y * boxSize.y);
    float y = fract(coord.y/aspectRatio * boxSize.y);
    if(compare(vec2(x,y), vec2(0.5f), vec2(0.45))) 
    {
        discard;
    }
    vec4 col = vec4(vec3(0.f), 1.f);
    FragColor = vec4(col);
}