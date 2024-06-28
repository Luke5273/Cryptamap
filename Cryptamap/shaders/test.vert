#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 col;

uniform float scale;
uniform float aspectRatio;
uniform vec2 translate;

void main() 
{
    vec2 pre = aPos.xy;
    mat2 transform = mat2(
        scale, 0,
        0,     scale*aspectRatio
    );
    vec2 post = transform * pre;
    gl_Position = vec4(post.xy + translate, aPos.z, 1.0); 
    col = aColor; 
}                                                           