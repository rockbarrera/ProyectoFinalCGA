#version 330 core
in vec3 ex_Color;
in vec2 ex_texCoord;

out vec4 color;

uniform sampler2D textura1;
uniform sampler2D textura2;
uniform sampler2D textura3;

void main()
{
	vec4 mix1 = mix(texture(textura1, ex_texCoord), texture(textura2, ex_texCoord), 0.7);
	color = mix(mix1, texture(textura3, ex_texCoord), 0.2);
}