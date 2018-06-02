#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float offset_y;
uniform float offset_z;

void main()
{
    gl_Position = projection * view * model * vec4(position.x, position.y + offset_y,
					position.z + offset_z, 1.0f);
} 