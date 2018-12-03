
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

#include "vgl.h"
#include "LoadShaders.h"


//#define STB_IMAGE_IMPLEMENTATION
#include <Common.h>

#include "Sprite2D.h"
#include "SpriteObject.h"
#include "camera.h"

GLfloat SCR_WIDTH = 800;
GLfloat SCR_HEIGHT = 600;


void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Keyboardup(unsigned char key, int x, int y);
void MenuEvents(int option);
void ParticleMenuEvents(int option);

void ParticleNumMenuEvents(int option);
void ParticleSpeedMenuEvents(int option);
void ParticleDirectionMenuEvents(int option);
void ParticleLifeMenuEvents(int option);

void	Deep_Timer(int val);
float	deep_interval = 60;
float	deepTime = 0.0f;
float	deepSpeed = 1.0f;

void Deep_Walk_Timer(int val);
float	deep_walk_interval = 80;
float	deepWalkTime = 0.0f;
double	deepWalkSpeed = 1.0f;

void	Jump_Timer(int val);
float	jump_interval = 30;
float	currentTime = 0.0f;
float	deltatime = 0.0f;
float	time_for_a_jump = 0.5f;
int is_move_when_jump; // 0 �� �D�A1 �� �V�k�A2 �� �V��

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

//the function for the skybox
unsigned int loadTexture(char const * path);

bool isFrame;
GLint isMotionSwitch; // �ഫ�ʧ@���Ǧ����L���]��0�A���|���ʧ@�ഫ���~�ӤW�@���ʧ@


GLuint VAO; // for deep
GLuint VBO;
GLuint EBO;

GLuint VAOskill;
GLuint VBOskill;
GLuint EBOskill;

GLuint VAOdb;
GLuint VBOdb;
GLuint EBOdb;

GLuint VAOb;
GLuint VBOb;
GLuint EBOb;

GLuint VAOp;
GLuint VBOp;
GLuint EBOp;

GLuint VAOpr;
GLuint VBOpr;
GLuint EBOpr;

GLuint VAOpr2;
GLuint VBOpr2;
GLuint EBOpr2;

GLuint quadVAO;
GLuint quadVBO;
unsigned int framebuffer; // �o�T��buffer�O�n����i�e�����S�Ĩϥ�
unsigned int textureColorbuffer;
unsigned int rbo;

unsigned int programDeep; // deep-program
unsigned int programSkill; // deep-program
unsigned int programDeepBlood;
unsigned int programBack;
unsigned int programParticle;
unsigned int programParticleRain;
unsigned int programParticleRain2;
unsigned int programFrame;


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
GLuint projectionDeepID;
GLuint viewDeepID;
GLuint viewPosDeepID;
GLuint lightPosDeepID;

//-----------------------
// skill-shader ID ��}
//-----------------------
GLuint offsetSkillID;
GLuint skillxID;
GLuint skillyID;
GLuint isLeftSkillID;
GLuint skillImageID;
GLuint skillTimeID;

//-----------------------
// deepBlood-shader ID ��}
//-----------------------
GLuint offsetDeepBloodID;
GLuint offsetDeepBloodLengthID;

//-----------------------
// background-shader ID ��}
//-----------------------

//-----------------------
// particle-shader ID ��}
//-----------------------
GLuint offsetParticleID;
GLuint colorParticleID;
GLuint projectionID;
GLuint offsetParticleMatrixID;
GLuint particleTimeID;
GLuint particleLifeID;

//-------------------------------
//particle-rain-shader ID
//-------------------------------
GLuint offsetParticleRainID;
GLuint colorParticleRainID;
GLuint particleTimeRainID;
GLuint particleLifeRainID;

//-------------------------------
//framebuffer-shader ID
//-------------------------------
GLuint frameColorID;

//-----------------------
// deep-variables
//-----------------------
mat4 offset; // deep����
int deepx; // ���ʶK�Ϯy��
int deepy; // ���ʶK�Ϯy��
int deep_0; // �Ϥ�1
int deep_1;
int deep_2;
Sprite2D* deepSheets[3];
Sprite2D* deepNormalSheets[3];
int objectCount = 3;
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
int firedragon; // ���s�b��
int juliancolumn; // ����W�l
Sprite2D* deepSkillSheets[3];
glm::vec2 dragonOffset[5];
glm::vec2 columnOffset[5];
int skillImage;
int drawSkill;

//-----------------------
// deep-blood-variables
//-----------------------
int deepBloodImg; // ����Ϥ�
mat4 offsetDeepBlood; // ����x�}�A���Hdeep
float offsetDeepBloodLength; // ����

//-----------------------
// background-variables
//-----------------------
int backgroundImg;

//-----------------------
//particle-variables
//-----------------------
int particleImg;
mat4 offsetParticle;
mat4 projectionParticle;

//-----------------------
// deep-vertices
//-----------------------
/*float deepVertices[] = {
	// positions          // colors           // texture coords for img 0/1
	0.08f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,  0.1f, 1.0f,      
	0.08f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,   0.1f, 0.87f,    
	-0.08, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.87f,   
	-0.08f,  0.1f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   
};*/
float deepVertices[] = {
	// positions          // colors           // texture coords for img 0/1
	0.1f,  0.12f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	0.1f, -0.12f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.1f, -0.12f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.1f,  0.12f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
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

float deepPosX = 0, deepPosY = 0;

//-----------------------
// skill-vertices
//-----------------------
/*float skillVertices[] = {
	// positions							// texture coords for img twinsflame/bat/sword-blow(orange/red/blue/yellow)
	0.08f,  0.08f, 0.0f,   1.0f, 0.0f, 0.0f,   0.25f, 1.0f,
	0.08f, -0.08f, 0.0f,   0.0f, 1.0f, 0.0f,   0.25f, 0.5f,
	-0.08f, -0.08f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.5f,
	-0.08f,  0.08f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
float skillVertices2[] = { // ���s
	0.4f,  0.15f, 0.0f,     0.5f, 1.0f, // up-right
	0.4f, -0.15f, 0.0f,     0.5f, 0.8f, // down-right
	-0.4f, -0.15f, 0.0f,     0.0f, 0.8f,  // down-left
	-0.4f,  0.15f, 0.0f,     0.0f, 1.0f   // up-left
};*/
float skillVertices[] = {
	// positions							// texture coords for img twinsflame/bat/sword-blow(orange/red/blue/yellow)
	0.08f,  0.08f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	0.08f, -0.08f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.08f, -0.08f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.08f,  0.08f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
float skillVertices2[] = { // ���s
	0.4f,  0.15f, 0.0f,     1.0f, 1.0f, // up-right
	0.4f, -0.15f, 0.0f,     1.0f, 0.0f, // down-right
	-0.4f, -0.15f, 0.0f,     0.0f, 0.0f,  // down-left
	-0.4f,  0.15f, 0.0f,     0.0f, 1.0f   // up-left
};
unsigned int skillIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

//-----------------------
// deepblood-variables
//-----------------------
float deepBloodVertices[] = {
	// positions							// texture coords for img twinsflame/bat/sword-blow(orange/red/blue/yellow)
	0.05f,  0.01f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	0.05f, -0.01f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.05f, -0.01f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.05f,  0.01f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
unsigned int deepBloodIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

//-----------------------
// background-variables
//-----------------------
float backVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions                            // texCoords
	 1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.4f, 1.0f,   // up-right
	 1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom-right
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.0f, //bottom-left
	-1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, //up-left 
};
unsigned int backIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

//----------------------------------------
//�ɤl�t��(learn opengl code)
//----------------------------------------
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

float particleLife, particleSpeed;

GLuint particleNum = 500; // particle�`��
vec2 particleDir = vec2(1.0,1.0); 

std::vector<Particle> particles; // ��particle�����l
GLuint nr_new_particles = 2; // �C���n�s�W��particle
GLuint lastUsedParticle = 0; // ��FirstUnusedParticle()�ϥ�
GLuint FirstUnusedParticle();
void RespawnParticle(Particle &particle, glm::vec2 charPos, glm::vec2 offset);

GLfloat particle_quad[] = {
	-0.01f, 0.01f, 0.0f, 1.0f, // left-up
	0.01f, -0.01f, 1.0f, 0.0f, //right-down
	-0.01f, -0.01f, 0.0f, 0.0f, //left-down

	-0.01f, 0.01f, 0.0f, 1.0f, // left-up
	0.01f, 0.01f, 1.0f, 1.0f, //right-up
	0.01f, -0.01f, 1.0f, 0.0f //right-down
};
float particleVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions                            // texCoords
	 0.01f,  0.01f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // up-right
	 0.01f, -0.01f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom-right
	-0.01f, -0.01f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom-left
	-0.01f,  0.01f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f //up-left 
};
unsigned int particleIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};
glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), static_cast<GLfloat>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
glm::vec4 deepPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);

GLfloat dt = jump_interval * 0.001;

//----------------------------------------
//�ɤl�t�ΤU�B(learn opengl code)
//----------------------------------------
std::vector<Particle> particleRain; // ��particle�����l
GLuint lastUsedParticleRain = 0; // ��FirstUnusedParticle()�ϥ�
GLuint FirstUnusedParticleRain();
void RespawnParticleRain(Particle &particle);
GLuint particleRainNum = 2000; // particle�`��
float rainLife, rainSpeed;
vec2 rainDir = vec2(1.0, 1.0);
GLuint particleRainImg;



//--------------------------------------
//framebufferobject�p�a��
//--------------------------------------
float quadVertices2[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// �p�a�ϡG�k�W-0.5,1.0�G�k�U-0.5,0.5�G���U-1.0,0.5�G���W-1.0,1.0
	// positions   // texCoords
	0.5,   1.0,  0.0f, 1.0f, // ���W
	0.5,   0.5,  0.0f, 0.0f, // ���U
	1.0,   0.5,  1.0f, 0.0f, // �k�U

	0.5,  1.0f,  0.0f, 1.0f, // ���W
	1,   0.5,  1.0f, 0.0f, // �k�U
	1,   1.0,  1.0f, 1.0f // �k�W
};
float quadVertices1[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

//---------------------------------------------
//yao chih yuan code
//---------------------------------------------
static unsigned int seed = 0x13371337;
enum
{
	NUM_STARS = 1000
};
struct star_t
{
	glm::vec3     position;
	glm::vec3     color;
	GLfloat       life;
};
star_t * star;
static inline float random_float();
GLuint starLife[NUM_STARS];
GLuint m_texture;
GLuint proj_location;
GLuint time_Loc;
mat4 proj_matrix;

GLfloat currentTimeStar;
GLfloat lastTime;
GLfloat deltaTimeStar;



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

#define DOR(angle) (angle*3.1415f/180);

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
	double r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r) + (1 - cos(r))*x*x, (1 - cos(r))*y*x + sin(r)*z, (1 - cos(r))*z*x - sin(r)*y, 0);
	vec4 c2 = vec4((1 - cos(r))*y*x - sin(r)*z, cos(r) + (1 - cos(r))*y*y, (1 - cos(r))*z*y + sin(r)*x, 0);
	vec4 c3 = vec4((1 - cos(r))*z*x + sin(r)*y, (1 - cos(r))*z*y - sin(r)*x, cos(r) + (1 - cos(r))*z*z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}