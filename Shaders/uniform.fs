#version 330 core
out vec4 color;
uniform vec4 ourColor;
void main()
{
	//gl_Position = vec4(position.x, position.y, position.z, 1.0);
	color = ourColor;
}