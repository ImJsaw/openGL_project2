#version 410 core

out vec4 fragColor;

in VertexData
{
	vec3 vNormal;
	vec3 vPos;
} vertexIn;

uniform samplerCube tex;
uniform vec3 cameraPos;
uniform float eta;
uniform bool reflectOrRefract;

void main(void) 
{
	vec3 R;
	if (reflectOrRefract)
	{
		R = reflect(normalize(vertexIn.vPos - cameraPos), normalize(vertexIn.vNormal));
	}
	else
	{
		R = refract(normalize(vertexIn.vPos - cameraPos), normalize(vertexIn.vNormal), eta);
	}
	R *= vec3(1, -1, -1);
	fragColor = texture(tex, R);
}