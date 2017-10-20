#version 440

uniform vec4 u_color;

in vec4 fcol;
out vec4 frag_colour;

void main()
{
    frag_colour = fcol * u_color;
}
