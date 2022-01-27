#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_position;
in vec3 frag_normal;
in vec3 frag_texture;
in vec3 frag_color;

in vec3 color;

// Output
layout(location = 0) out vec4 out_color;

float rand(vec3 co){
    return fract(sin(dot(co, vec3(12.9898, 78.233, 43758.5453))) * 43758.5453);
}


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(color, 1.0);

}
