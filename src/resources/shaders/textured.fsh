#version 440

uniform sampler2D s2D_tex;

in vec2 tex_coord;
out vec4 frag_colour;

void main()
{
    frag_colour = texture( s2D_tex, tex_coord );
}
