#version 440

layout ( location = 0 ) out vec3 gDiffuse;
layout ( location = 1 ) out vec3 gPosition;
layout ( location = 2 ) out vec3 gNormal;

uniform vec3 u_color;

in vec3 v_frag_pos;
in vec3 v_normal;

void main()
{
    gDiffuse = u_color;
    gPosition = v_frag_pos;
    gNormal = v_normal;
}
