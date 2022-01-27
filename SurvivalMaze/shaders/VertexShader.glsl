#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_texture;
out vec3 frag_color;

uniform vec3 object_color;

out vec3 color;


void main()
{
    // Send output to fragment shader
    frag_position = v_position;
    frag_normal = v_normal;
    frag_texture = v_texture;
    frag_color = v_color;

    color = object_color;

    // Compute gl_Position
   gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
