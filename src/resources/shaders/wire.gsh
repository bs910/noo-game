#version 440

layout ( triangles ) in;
layout ( line_strip, max_vertices = 4 ) out;

in vec4 fcol[];
out vec4 ffcol;

void main()
{
    gl_Position = gl_in[ 0 ].gl_Position;
    ffcol = fcol[0];
    EmitVertex();

    gl_Position = gl_in[ 1 ].gl_Position;
    ffcol = fcol[1];
    EmitVertex();

    gl_Position = gl_in[ 2 ].gl_Position;
    ffcol = fcol[2];
    EmitVertex();

    gl_Position = gl_in[ 0 ].gl_Position;
    ffcol = fcol[0];
    EmitVertex();

    EndPrimitive();

}
