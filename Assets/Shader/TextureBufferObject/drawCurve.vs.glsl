#version 410 core

uniform mat4 um4mv;
uniform mat4 um4p;
uniform samplerBuffer tbo;

// tension = 0
uniform mat4 paramMatrix = mat4(vec4(-0.5, 1.5, -1.5, 0.5),
								vec4(1, -2.5, 2, -0.5),
								vec4(-0.5, 0, 0.5, 0),
								vec4(0, 1, 0, 0));
uniform int lineDiv = 100;
uniform bool drawLine;

void main()
{
	if (drawLine)
	{
		float t = float(gl_VertexID % lineDiv) * 1.0f / lineDiv;
		vec4 T = vec4(t * t * t, t * t, t, 1.0);

		int pointCount = textureSize(tbo);
		int pid = gl_VertexID / lineDiv;
		int p0 = (pid + 0) % pointCount;
		int p1 = (pid + 1) % pointCount;
		int p2 = (pid + 2) % pointCount;
		int p3 = (pid + 3) % pointCount;
		mat4 G = mat4(texelFetch(tbo, p0),
			texelFetch(tbo, p1),
			texelFetch(tbo, p2),
			texelFetch(tbo, p3));

		vec4 pos = G * paramMatrix * T;

		gl_Position = um4p * um4mv * vec4(pos.xyz, 1.0);
	}
	else
	{
		gl_Position = um4p * um4mv * texelFetch(tbo, gl_VertexID);
	}
}
