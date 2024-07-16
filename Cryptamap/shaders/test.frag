#version 430 core
out vec3 color;
in vec3 col;

void main()
{
    color = vec3(col);
}