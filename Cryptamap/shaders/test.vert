#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 col;

uniform float scale;
uniform vec2 translate;

void main() 
{
    vec2 pos = aPos.xy * scale;
    gl_Position = vec4(pos + translate, aPos.z, 1.0); 
    col = aColor; 
}                                                           