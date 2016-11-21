#version 150

//---- ins
in  vec4 color;
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;
in  vec2 tex_coord;

//---- outs
out vec4 FragColor;

//---- uniforms
uniform vec4 AmbientLight, DiffuseLight, SpecularLight;
uniform vec4 ka, kd, ks;
uniform float Shininess;
uniform bool light_out;
uniform vec4 LightPosition;
uniform mat4 view;
uniform bool texture_on;

uniform sampler2D myTextureSampler;

vec4 kA, kD, kS;

void main()
{
    // Normalize the input lighting vectors
    vec3 N = normalize(fN);
    if (light_out) N = -N;
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    
    vec3 H = normalize( L + E );
    
    
    //---- get texture value
    vec4 texel = vec4(1.0, 1.0, 1.0, 1.0);
    
    if (texture_on)
    {
        texel = texture( myTextureSampler, tex_coord );
        
        kA = vec4(1.0,1.0,1.0,1.0);
        kD = vec4(1.0,1.0,1.0,1.0);
        kS = vec4(0.9,0.9,0.9,1.0);
    }
    else
    {
        kA = ka;
        kD = kd;
        kS = ks;
    }
    
    // Compute terms in the illumination equation
    vec4 ambient = AmbientLight * kA * texel;
    
    vec4  diffuse;
    if (light_out)
        diffuse = DiffuseLight * kD * texel * max( dot(L, N), 0.0 );
    else
        diffuse = DiffuseLight * kD * texel * max( dot(L, N), 0.0 )/(0.1*pow(length(fL),2.0));

    vec4  specular = SpecularLight * kS * texel * pow( max(dot(reflect(-L,N), E), 0.0), Shininess );
    //vec4  specular = SpecularLight * ks * pow( max(dot(N,H), 0.0), Shininess );
    
    if ( dot(L, N) < 0.0 ) specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    FragColor = ambient + diffuse + specular;
    FragColor.a = 1.0;
}


