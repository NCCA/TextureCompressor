#version 400 core
/// @brief our output fragment colour
layout (location =0) out vec4 fragColour;
// texture sampler
uniform sampler2D tex;
// uv from input
in vec2 vertUV;


void main ()
{
  fragColour.rgb=texture(tex,vertUV ).bgr;
}

