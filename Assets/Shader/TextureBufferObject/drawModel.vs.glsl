#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 um4v;
uniform mat4 um4p;
uniform samplerBuffer tbo;

out VertexData
{
	vec2 texCoord;
} vertexOut;

void main()
{
	int idx = gl_InstanceID * 4;
	vec4 posV4 = vec4(vertex, 1.0);
	mat4 model = mat4(texelFetch(tbo, idx),
		texelFetch(tbo, idx + 1),
		texelFetch(tbo, idx + 2),
		texelFetch(tbo, idx + 3) );
	mat4 modelView = um4v * model;
	vertexOut.texCoord = texCoord;

	gl_Position = um4p * modelView * posV4;
}
