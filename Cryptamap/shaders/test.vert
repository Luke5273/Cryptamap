#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 col;

uniform float scale;
uniform vec2 translate;
uniform float aspectRatio;

void main() 
{
    mat2 aspectRatio = {{scale, 0}, {0, aspectRatio * scale}};
    vec2 pos = aspectRatio * aPos.xy;
    gl_Position = vec4(pos + translate, aPos.z, 1.0); 
    col = aColor; 
}                                                           