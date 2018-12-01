#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 FragPos;

flat in int mariocontroller;
flat in int deepImg;
flat in int deep_x;
flat in int deep_y;

uniform sampler2DArray tex;
uniform sampler2DArray normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float time;
vec2 iResolution = vec2(800., 600.);
void main()
{    
	
	// obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, vec3(TexCoord, (deep_y-1) * 10 + (deep_x-1))).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
	if(texture(tex, vec3(TexCoord, (deep_y-1) * 10 + (deep_x-1))).a < 0.1){
		discard;
	}
    vec3 color = texture(tex, vec3(TexCoord, (deep_y-1) * 10 + (deep_x-1))).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);

}