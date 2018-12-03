#version 330 core
out vec4 FragColor;

uniform sampler2D sprite;
in vec3 pos;

void main()
{
	if(pos.z <= 0.0){
		FragColor = texture(sprite ,gl_PointCoord);
	}
	else{
		FragColor = texture(sprite ,gl_PointCoord) + vec4(0.5, 0.5, 0.5, 0.0); // set alle 4 vector values to 1.0
	}
    
}
