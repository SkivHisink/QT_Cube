#version 330

out highp vec4 fragColor;

in vec4 colour;
void main()
{
   fragColor = colour;
}
