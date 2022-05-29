#version 440

#define M_PI 3.14159265359

uniform sampler2D s2D_diffuse;
uniform sampler2D s2D_position;
uniform sampler2D s2D_normal;

uniform vec3 u_light_pos;
uniform vec3 u_light_color;
uniform vec3 u_view_pos;

in vec2 v_tex_coords;

out vec4 frag_color;


float oren_nayar( vec3 L, vec3 N, vec3 V, float roughness, float alb )
{
    float NdotL = dot( N, L );
    float NdotV = dot( N, V );
    float r2 = roughness * roughness;

    float s = dot( L, V ) - NdotL * NdotV;
    float t = 1.0;

    if ( s > 0.0 )
    {
        t = max( NdotL, NdotV );
    }

    float A = ( 1.0 - 0.5 * r2 / ( r2 + 0.33 ) + 0.17 * alb * r2 / ( r2 + 0.13 ) ) / M_PI;
    float B = ( 0.45 * r2 / ( r2 + 0.09 ) ) / M_PI;

    return alb * NdotL * ( A + B * s / t );
}


void main()
{
    vec3 frag_pos = texture( s2D_position, v_tex_coords ).xyz;
    vec3 normal = normalize( texture( s2D_normal, v_tex_coords ).xyz );
    vec3 diffuse = texture( s2D_diffuse, v_tex_coords ).rgb;

    vec3 light_dir = normalize( u_light_pos - frag_pos );
    vec3 view_dir = normalize( u_view_pos - frag_pos );
    vec3 reflect_dir = reflect( -light_dir, normal );

//    float c_diff = max( dot( normal, light_dir ), 0.0 );

    float c_diff = oren_nayar( light_dir, normal, view_dir, 0.8, 1.96 );
    float c_spec = pow( max( dot( view_dir, reflect_dir ), 0.0 ), 2 );

    vec3 color = diffuse * u_light_color * c_diff;
//    color += vec3( 1, 1, 1 ) * c_spec;

    frag_color = vec4( color, 1.0 );
}
