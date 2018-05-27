#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
out vec3 ourColor;

uniform float offset_x;
uniform float offset_y;

void main(){
	gl_Position = vec4(position.x + offset_x, position.y + offset_y, position.z, 1.0);
	ourColor = color;
}