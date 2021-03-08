#version 330

layout(location = 0) in vec4 posAttr;
out VS_OUT {
    vec4 posAttr2;
    vec4 baseColour;
} vs_out;
in vec4 colAttr;
//out vec4 colour;

void main()
{
    vs_out.baseColour = colAttr;
    vs_out.posAttr2 = posAttr;
    gl_Position = posAttr;
    //colour = colAttr;
    //gl_FrontColor  = colAttr;
}
