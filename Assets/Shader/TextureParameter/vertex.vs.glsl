#version 410 core

uniform mat4 um4mv;
uniform mat4 um4p;

out VertexData
{
	vec2 texcoord;
} vertexData;

const vec2[] points = vec2[4](vec2(-5, 0), vec2(5, 0), vec2(-5, 50), vec2(5, 50));
const vec2[] uv = vec2[4](vec2(0, 0), vec2(2, 0), vec2(0, 10), vec2(2, 10));

void main()
{
	gl_Position = um4p * um4mv * vec4(points[gl_VertexID], 0.0, 1.0);
	vertexData.texcoord = uv[gl_VertexID];
}