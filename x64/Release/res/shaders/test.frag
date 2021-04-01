#version 420 core

layout (binding = 0) uniform sampler2D ALB_Texture;
layout (binding = 1) uniform sampler2D DECAL_MAP_Texture;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
	vec3 color = texture(ALB_Texture, vec2(TexCoord.x, TexCoord.y)).rgb;
	vec3 decalMap = texture(DECAL_MAP_Texture, vec2(TexCoord.x, TexCoord.y)).rgb;
	//FragColor = vec4(color * decalMap, 1);
	FragColor = vec4(color, 1);
}  