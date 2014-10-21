#version 330 core
/// @brief the vertex passed in
layout (location = 0) in vec3 inVert;
/// @brief the in uv
layout (location = 1) in vec2 inUV;

out vec2 vertUV;


void main()
{

// calculate the vertex position
gl_Position = vec4(inVert,1.0);
vertUV=inUV;
}
