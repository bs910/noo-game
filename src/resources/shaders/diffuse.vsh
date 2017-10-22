#version 440

uniform mat4 u_mvp;

in vec3 a_vp;
in vec3 a_nrm;

out vec3 v_nrm;

void main()
{
    gl_Position = u_mvp * vec4( a_vp, 1.0 );
    v_nrm = a_nrm;
}
