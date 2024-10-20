#version 430 core
layout (location = 0) in vec3 aPos;
out vec2 fragPos;

uniform float scale;
uniform vec2 translate;
uniform float aspectRatio;

void main() 
{
    mat2 aspectRatio = {{1, 0}, {0, aspectRatio}};
    vec2 pos = aspectRatio * aPos.xy;
    gl_Position = vec4(pos*scale + translate, aPos.z, 1.0);
    fragPos = (aPos.xy + vec2(1,1))/2;
}                                                           