#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
//#include <cmath>

#include "vgl.h"
#include "LoadShaders.h"
#include "objloader.hpp"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#define PARTSNUM 18
#define BODY 0
#define LEFTSHOUDER 1
#define ULEFTARM 2
#define DLEFTARM 3
#define LEFTHAND 4

void updateModels();

void init();
void init2();

void ChangeSize(int w,int h);
void display();
void display2();
void Keyboard(unsigned char key, int x, int y);
void Keyboardup(unsigned char key, int x, int y);
void Mouse(int button,int state,int x,int y);

void	Deep_Timer(int val);
float	deep_interval = 40;
float	deepTime = 0.0f;
double	deepSpeed = 1.0f;

void	Jump_Timer(int val);
float	jump_interval = 30;
float	currentTime = 0.0f;
float	deltatime = 0.0f;
float	time_for_a_jump = 1.2f;
int is_move_when_jump; // 0 為 非，1 為 向右，2 為 向左

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void ShaderMenuEvents(int option);

void idle(int dummy);

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

void Obj2Buffer();
void load2Buffer( char* obj,int);

void updateObj3(int); // new added gangnam function
void updateObj2(int); // new added dance function
void updateObj(int);
void resetObj(int);

//the function for the skybox
unsigned int loadTexture(char const * path);
unsigned int loadCubemap(vector<std::string> faces);



bool isFrame;
GLint isMotionSwitch; // 轉換動作時藉此讓他先設為0，不會讓動作轉換時繼承上一秒的動作

GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint uVBO;
GLuint nVBO;
GLuint mVBO;
GLuint UBO;
GLuint VBOs[PARTSNUM];
GLuint uVBOs[PARTSNUM];
GLuint nVBOs[PARTSNUM];
unsigned int skyboxVAO, skyboxVBO; // this set is for skybox
unsigned int quadVAO, quadVBO; // 給framebuffer的螢幕面
GLuint program;
unsigned int programs; // this program is for skybox
unsigned int programf;// 製作後製特效的shader-program
int pNo;
int mode;
GLuint modeID;
GLuint timeID;
GLuint CubemapID;
GLuint pNoID;
GLuint lenID;
int frametimeID;
int channel0ID;
int channel1ID;
int texturechar[8];
int textureback;
GLuint deepController;
GLuint mariocontrollerID;
GLuint offsetID;
GLuint deepxID;
GLuint deepyID;
GLuint isLeftID;
GLuint deepImageID;

mat4 offset; // 馬力歐移動
int deepx; // 移動貼圖座標
int deepy; // 移動貼圖座標
int deep_0; // 圖片1
int deep_1;
int deep_2;
int isLeft; // 是不是左邊
int deepDirection; // 左上右下
int deepImage;

// it is for deep
float deepVertices[] = {
	// positions          // colors           // texture coords for img 0/1
	0.08f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,  0.1f, 1.0f,      
	0.08f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,   0.1f, 0.87f,    
	-0.08f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.87f,   
	-0.08f,  0.1f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   
};
float deepTexCoord2[] = {
	0.1f, 1.0f,// top right
	0.1f, 0.75f,// bottom right
	0.0f, 0.75f,// bottom left
	0.0f, 1.0f// top left
};

//it is for deep too
unsigned int deepIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};
float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};
unsigned int framebuffer; // 這三個buffer是要做整張畫面的特效使用
unsigned int textureColorbuffer;
unsigned int rbo;

float yangles[PARTSNUM]; // new add for 手臂旋轉
float zangles[PARTSNUM]; // new add for 手臂左右平舉
float angles[PARTSNUM];
float position = 0.0;
float angle = 0.0;
float eyeAngley = 0.0;
float eyedistance = 20.0;
float size = 1;
GLfloat movex,movey;
GLint MatricesIdx;
GLuint ModelID;
GLuint ViewPosID;// new added

int vertices_size[PARTSNUM];
int uvs_size[PARTSNUM];
int normals_size[PARTSNUM];
int materialCount[PARTSNUM];

std::vector<std::string> mtls[PARTSNUM];//use material
std::vector<unsigned int> faces[PARTSNUM];//face count
map<string,vec3> KDs;//mtl-name&Kd
map<string,vec3> KSs;//mtl-name&Ks

mat4 Projection;
mat4 View;
mat4 Model;
mat4 Models[PARTSNUM];

GLuint ProjectionskyID; // this both matrix(矩陣) ID is for loading into skybox
GLuint ViewskyID;
GLuint cubemapTexture;

#define leftHand 0
#define rightHand 1
#define leftFoot 2
#define rightFoot 3
#define GANGNAM 3 // new add gangnam keyword
#define DANCE 2 // new add dance keyword
#define WALK 1
#define IDLE 0
//int mode;
int action;

#define DOR(angle) (angle*3.1415/180);

mat4 translate(float x, float y, float z) {
	vec4 t = vec4(x, y, z, 1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1, 0, 0, 0);
	vec4 c2 = vec4(0, 1, 0, 0);
	vec4 c3 = vec4(0, 0, 1, 0);
	mat4 M = mat4(c1, c2, c3, t);
	return M;
}
mat4 scale(float x, float y, float z) {
	vec4 c1 = vec4(x, 0, 0, 0);
	vec4 c2 = vec4(0, y, 0, 0);
	vec4 c3 = vec4(0, 0, z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	mat4 M = mat4(c1, c2, c3, c4);
	return M;
}
mat4 rotate(float angle, float x, float y, float z) {
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r) + (1 - cos(r))*x*x, (1 - cos(r))*y*x + sin(r)*z, (1 - cos(r))*z*x - sin(r)*y, 0);
	vec4 c2 = vec4((1 - cos(r))*y*x - sin(r)*z, cos(r) + (1 - cos(r))*y*y, (1 - cos(r))*z*y + sin(r)*x, 0);
	vec4 c3 = vec4((1 - cos(r))*z*x + sin(r)*y, (1 - cos(r))*z*y - sin(r)*x, cos(r) + (1 - cos(r))*z*z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}