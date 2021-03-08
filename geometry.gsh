#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices = 192) out;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
in VS_OUT {
    vec4 posAttr2;
    vec4 baseColour;
} gs_in[];
//out GS_OUT {
//    vec4 color;
//} gs_out;
uniform mat4 matrix;
uniform float radius;
uniform float morphForce;
uniform int morph_type;
//
uniform mat4 trate;
out vec4 colour;
//in vec4 colour;
//out vec4 coloured;
void main()
{
    //coloured=colour;
    //gs_out=gs_in.baseColour;
    vec4 vertex[192];
    int current = 3;
    for(int i = 0; i < current; ++i)
    {
        vertex[i] = gs_in[i].posAttr2;
        //problem here
        colour = gs_in[i].baseColour;
    }
    for(int i = 0; i < current; ++i)
    {
        vec3 tmp = vec3(vertex[i].x, vertex[i].y, vertex[i].z);
        if(morph_type == 1)
        {
            tmp = tmp * (1.0 - morphForce) + normalize(tmp) * radius * morphForce;
        }
        if(morph_type == 2)
        {
            tmp.x = tmp.x*(1.0-morphForce) + normalize(tmp).x * radius * morphForce;
            tmp.y = tmp.y*(1.0-morphForce) + normalize(tmp).y * radius * morphForce;
            tmp.z = tmp.z*(1.0-morphForce) + normalize(tmp).z * radius * morphForce * 0.5;
        }
        vertex[i] = vec4(tmp,1);

    }

    for(int i = 0; i < current; ++i)
    {
        gl_Position = matrix * trate * vertex[i];
        EmitVertex();
    }
    EndPrimitive();
}
