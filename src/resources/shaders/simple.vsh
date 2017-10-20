#version 440

uniform mat4 u_mvp;

in vec3 a_vp;
in vec4 a_col;
out vec4 fcol;

void main()
{
    gl_Position = u_mvp * vec4( a_vp, 1.0 );
    fcol = a_col;
}
