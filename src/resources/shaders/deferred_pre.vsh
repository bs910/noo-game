#version 440

uniform mat4 u_mvp;
uniform mat3 u_mat_rot;

in vec3 a_pos;
in vec3 a_nrm;

out vec3 v_frag_pos;
out vec3 v_normal;

void main()
{
    vec4 pos = u_mvp * vec4( a_pos, 1.0 );
    gl_Position = pos;
    v_frag_pos = pos.xyz;
    v_normal = normalize( u_mat_rot * a_nrm );
}
