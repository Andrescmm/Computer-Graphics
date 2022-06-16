#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

void main()
{
	FragColor = vec4(TexCoord, 1.0f);
}