//fs
// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal;
varying vec3 v_color;
varying vec3 v_world_position;

uniform vec3 u_eyeposition;
uniform vec3 u_lightposition;

uniform sampler2D u_texturecolor;
uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform vec3 u_ia;
uniform vec3 u_id;
uniform vec3 u_is;

uniform float u_alpha;

void main()
{
	// Set the ouput color per pixel;
    
    vec3 N = normalize(v_world_normal);
    vec3 L = normalize(u_lightposition-v_world_position);
    vec3 V = normalize(u_eyeposition-v_world_position);
    vec3 R = normalize(reflect(-L, N));
    
    vec3 ambient = u_ka*u_ia;
    vec3 diffuse = u_kd*dot(L, N)*u_id;
    float RdotV = dot(R, V);
    RdotV = clamp(RdotV, 0.0, 1.0);
    vec3 specular = u_ks*pow(RdotV, u_alpha)*u_is;
                    
    vec3 final_color = ambient+diffuse+specular;

	gl_FragColor = vec4(final_color, 1.0);
}
