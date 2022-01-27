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

uniform float time;
uniform float rd;


out vec3 color;

float rand(vec3 co){
      return fract(sin(dot(co, vec3(12.9898, 78.233, 43758.5453))) * 43758.5453);
}


void main()
{
    // Send output to fragment shader
    frag_position = v_position;
    frag_normal = v_normal;
    frag_texture = v_texture;
    frag_color = v_color;

    color = object_color;

    vec3 new_pos = v_position + v_normal * sin(time * 10 + v_position.z * 20) * rand(v_position);

    // Compute gl_Position
   gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}
