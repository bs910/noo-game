#version 440

uniform vec4 u_color;

in vec4 ffcol;
out vec4 frag_colour;

void main()
{
    frag_colour = ffcol * u_color;
}
