#version 410 core

layout(location = 0) out vec4 fragColor;

in VertexData
{
	vec2 texcoord;
} vertexData;

uniform sampler2D tex;

void main()
{
	fragColor = texture(tex, vertexData.texcoord).rgba;
}