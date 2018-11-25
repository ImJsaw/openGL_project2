// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430
struct MaterialInfo{ //物體與其身上的材質是分開的，而不同材質對於光源的反應都不盡相同
	vec3 Ka; //ambient(各個角落都有的微弱光)
	vec3 Kd; //diffuse(會產生陰影的光)
	vec3 Ks; //specular(會有圓形光暈打在物件上)
};

uniform MaterialInfo Material;
out vec4 vFragColor;//傳到下一階段的資料

//lighting color
vec4    ambientColor = vec4(0.1,0.1,0.1,1);//已經直接把光強度和光顏色融合了(ambientColor = Strength * lightColor;)
vec4    diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4    specularColor = vec4(1,1,1,1);

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
float Shininess = 128.0;//for material specular
in vec3 viewDir;

//postprocessing stuff
uniform int mode;
uniform float time; //做煥彩特效
uniform samplerCube skybox; // 做鏡像特效
in vec3 vLight;

void main(void)
{ 
	
    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));//物體到光源(單位向量) dot 法向量(單位向量)
					

    // Multiply intensity by diffuse color, force alpha to 1.0
    vFragColor = diff * diffuseColor*vec4(Material.Kd,1); // diffuse 的結果 = 光打到角度不同造成的強弱光 * 光色 * 物體對光的反應


    // Add in ambient light
    vFragColor += ambientColor;// 要輸出的vFragColor要把三種光特性打在物件上的結果加總


    // Specular Light
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),normalize(vVaryingNormal)));//反射角(反射光角度)， 負號是因為我們要的是物體到光源的向量
								
    //float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
	float spec = max(0.0, dot(viewDir, vReflection) );
    if(diff != 0) {
		spec = pow(spec, Shininess);
		vFragColor += specularColor*vec4(Material.Ka,1)*spec; //一樣specular的結果 = 光打到角度不同造成的強弱光 * 光色 * 物體對光的反應
    }

	//測試各種變色情況
	if(mode == 2){
		vFragColor = 1.0 - vFragColor;//逆顏色
	}
	else if(mode == 3){
		float avg = (vFragColor.r + vFragColor.g + vFragColor.b)/3.0;//灰階
		vFragColor = vec4(avg, avg, avg, 1.0);
	}
	else if(mode == 4){

		vFragColor *= vec4(sin(time),cos(time), cos(tan(time)), 1.0);
	}
	else if(mode == 5){
		float ratio = 1.00 / 1.52;
		//vec3 I = normalize(Position - cameraPos);
		vec3 I = -viewDir; // viewDir = normalize(viewPos - vPosition)
		vec3 R = refract(I, normalize(vVaryingNormal), ratio);
		vFragColor = vec4(texture(skybox, R).rgb, 1.0);
	}
	else if(mode == 6){ // 新增卡通特效
		float diffuse = dot(normalize(vLight), normalize(vVaryingNormal));
		if (diffuse > 0.8) {
			diffuse = 0.9;
		}
		else if (diffuse > 0.5) {
			diffuse = 0.505;
		}
		else if (diffuse > 0.2) {
			diffuse = 0.205;
		}
		else {
			diffuse = 0.005;
		}

		vFragColor *= diffuse;
	}


}