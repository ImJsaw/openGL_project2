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
// Include GLM
#include <include/glm/glm.hpp>
#include <include/glm/gtc/matrix_transform.hpp>
using namespace glm;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Keyboardup(unsigned char key, int x, int y);

void	Deep_Timer(int val);
float	deep_interval = 60;
float	deepTime = 0.0f;
double	deepSpeed = 1.0f;

void	Jump_Timer(int val);
float	jump_interval = 30;
float	currentTime = 0.0f;
float	deltatime = 0.0f;
float	time_for_a_jump = 1.2f;
int is_move_when_jump; // 0 �� �D�A1 �� �V�k�A2 �� �V��

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

//the function for the skybox
unsigned int loadTexture(char const * path);
unsigned int loadCubemap(vector<std::string> faces);


bool isFrame;
GLint isMotionSwitch; // �ഫ�ʧ@���Ǧ����L���]��0�A���|���ʧ@�ഫ���~�ӤW�@�����ʧ@

GLuint VAO; // for deep
GLuint VBO;
GLuint EBO;

GLuint VAOskill;
GLuint VBOskill;
GLuint EBOskill;




unsigned int programDeep; // deep-program
unsigned int programSkill; // deep-program
int pNo;
int mode;
GLuint modeID;
GLuint timeID;

//-----------------------
// deep-shader ID ��}
//-----------------------
GLuint deepController;
GLuint deepcontrollerID;
GLuint offsetID;
GLuint deepxID;
GLuint deepyID;
GLuint isLeftID;
GLuint deepImageID;

//-----------------------
// deep-shader ID ��}
//-----------------------
//GLuint deepController;
//GLuint mariocontrollerID;
GLuint offsetSkillID;
GLuint skillxID;
GLuint skillyID;
GLuint isLeftSkillID;
GLuint skillImageID;



//-----------------------
// deep-variables
//-----------------------
mat4 offset; // deep����
int deepx; // ���ʶK�Ϯy��
int deepy; // ���ʶK�Ϯy��
int deep_0; // �Ϥ�1
int deep_1;
int deep_2;
int isLeft; // �O���O����
int deepDirection; // ���W�k�U
int deepImage;

//-----------------------
// skill-variables
//-----------------------

mat4 offsetSkill;
int skillx;
int skilly;
int twinsflame; // �B���M�y�Ϥ�
int skillImage;
int drawSkill;

// it is for deep
float deepVertices[] = {
	// positions          // colors           // texture coords for img 0/1
	-0.84f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,  0.1f, 1.0f,      
	-0.84f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,   0.1f, 0.87f,    
	-1.0f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.87f,   
	-1.0f,  0.1f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   
};
float deepTexCoord2[] = { // deep���ĤG�i�Ϯy�Ф��P
	0.1f, 1.0f,// top right
	0.1f, 0.75f,// bottom right
	0.0f, 0.75f,// bottom left
	0.0f, 1.0f// top left
};
unsigned int deepIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

float skillVertices[] = {
	// positions							// texture coords for img twinsflame/bat/sword-blow(orange/red/blue/yellow)
	-0.68f,  0.08f, 0.0f,   1.0f, 0.0f, 0.0f,   0.25f, 1.0f,
	-0.68f, -0.08f, 0.0f,   0.0f, 1.0f, 0.0f,   0.25f, 0.5f,
	-0.84f, -0.08f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.5f,
	-0.84f,  0.08f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
unsigned int skillIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
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

float position = 0.0;
float angle = 0.0;
float eyeAngley = 0.0;
float eyedistance = 20.0;
float size = 1;
GLfloat movex,movey;
GLint MatricesIdx;
GLuint ModelID;
GLuint ViewPosID;// new added


mat4 Projection;
mat4 View;
mat4 Model;
GLuint ProjectionskyID; // this both matrix(�x�}) ID is for loading into skybox
GLuint ViewskyID;
GLuint cubemapTexture;

//int mode;
int action;
int xMove, yMove;

#define DOR(angle) (angle*3.1415/180);

mat4 translate(float x, float y, float z) {
	vec4 t = vec4(x, y, z, 1);//w = 1 ,�hx,y,z=0�ɤ]��translate
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