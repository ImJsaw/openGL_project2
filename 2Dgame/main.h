
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
void initDeep();
void initFiren();

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

void	Jump_Timer(int val);
float	jump_interval = 30;
float	currentTime = 0.0f;
float	deltatime = 0.0f;
float	time_for_a_jump = 0.5f;
int is_move_when_jump_deep; // 0 為 非，1 為 向右，2 為 向左
bool isEnemyHitByDeep(float enemyPosX, float enemyPosY, float skillRange);


//===========================================
//------------------------------------------
void	Firen_Timer(int val);
float	firen_interval = 60;
float	firenTime = 0.0f;
float	firenSpeed = 1.0f;

void	JumpFiren_Timer(int val);
float	jumpFiren_interval = 30;
float	currentTimeFiren = 0.0f;
float	deltatimeFiren = 0.0f;
float	time_for_a_jump_firen = 0.5f;
int is_move_when_jump_firen; 
bool isEnemyHitByFiren(float enemyPosX, float enemyPosY, float skillRange);

//------------------------------------------
//===========================================

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

//the function for the skybox
unsigned int loadTexture(char const * path);

bool isFrame;
GLint isMotionSwitch; // 轉換動作時藉此讓他先設為0，不會讓動作轉換時繼承上一秒的動作

//----------------------------
// deep, skill, blood
//----------------------------
GLuint VAO; // for deep
GLuint VBO;
GLuint EBO;

GLuint VAOskill;
GLuint VBOskill;
GLuint EBOskill;

GLuint VAOdb;
GLuint VBOdb;
GLuint EBOdb;

//------------------------------------------
//===========================================
GLuint VAOf; // for deep
GLuint VBOf;
GLuint EBOf;

GLuint VAOfskill;
GLuint VBOfskill;
GLuint EBOfskill;

GLuint VAOfb;
GLuint VBOfb;
GLuint EBOfb;
//===========================================
//------------------------------------------

//----------------------------
// background, particle(deep), particle(rain)
//----------------------------
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
unsigned int framebuffer; // 這三個buffer是要做整張畫面的特效使用
unsigned int textureColorbuffer;
unsigned int rbo;

unsigned int programDeep; // deep-program
unsigned int programSkill; // deep-program
unsigned int programDeepBlood;

//------------------------------------------
//===========================================
unsigned int programFiren; // deep-program
unsigned int programFirenSkill; // deep-program
unsigned int programFirenBlood;
//===========================================
//------------------------------------------

unsigned int programBack;
unsigned int programParticle;
unsigned int programParticleRain;
unsigned int programParticleRain2;

unsigned int programFrame;
unsigned int programLight;


int pNo;
int mode;
GLuint modeID;
GLuint timeDeepID;

//-----------------------
// deep-shader ID 位址
//-----------------------
GLuint deepController;
GLuint deepcontrollerID;
GLuint offsetDeepID;
GLuint deepxID;
GLuint deepyID;
GLuint isLeftID;
GLuint deepImageID;
GLuint projectionDeepID;
GLuint viewDeepID;
GLuint viewPosDeepID;
GLuint lightPosDeepID;
//-----------------------
// skill-shader ID 位址
//-----------------------
GLuint offsetSkillID;
GLuint skillxID;
GLuint skillyID;
GLuint isLeftSkillID;
GLuint skillImageID;
GLuint skillTimeID;
//-----------------------
// deepBlood-shader ID 位址
//-----------------------
GLuint offsetDeepBloodID;
GLuint offsetDeepBloodLengthID;

//------------------------------------------
//===========================================
//-----------------------
// firen-shader ID 位址
//-----------------------
GLuint firenController;
GLuint firencontrollerID;
GLuint offsetFirenID;
GLuint firenxID;
GLuint firenyID;
GLuint isLeftFirenID;
GLuint firenImageID;
GLuint projectionFirenID;
GLuint viewFirenID;
GLuint viewPosFirenID;
GLuint lightPosFirenID;
GLuint timeFirenID;
//-----------------------
// skill-shader ID 位址
//-----------------------
GLuint offsetSkillFirenID;
GLuint firenSkillxID;
GLuint firenSkillyID;
GLuint isLeftSkillFirenID;
GLuint firenSkillImageID;
GLuint firenSkillTimeID;
//-----------------------
// firenBlood-shader ID 位址
//-----------------------
GLuint offsetFirenBloodID;
GLuint offsetFirenBloodLengthID;
//===========================================
//------------------------------------------



//-----------------------
// background-shader ID 位址
//-----------------------
//-----------------------
// particle-shader ID 位址
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
//-------------------------------
//light-shader ID
//-------------------------------
GLuint lightPosID;



//-----------------------
// deep-variables
//-----------------------
mat4 offsetDeep; // deep移動
int deepx; // 移動貼圖座標
int deepy; // 移動貼圖座標
float deepPosX = 0, deepPosY = 0;

Sprite2D* deepSheets[3];
Sprite2D* deepNormalSheets[3];
int objectCount = 3;
int isLeftDeep; // 是不是左邊
int deepDirection; // 左上右下
int isDeepDie = 0;
int isDeepShine = 0;
int deepImage;

//-----------------------
// skill-variables
//-----------------------

mat4 offsetDeepSkill;
int deepSkillx;
int deepSkilly;
int twinsflame; // 冰火刀流圖片
int firedragon; // 紅龍在飛
int juliancolumn; // 紫色柱子
Sprite2D* deepSkillSheets[3];
glm::vec2 dragonOffset[5];
glm::vec2 columnOffset[5];
int deepSkillImage;
int drawSkillDeep;
int tflamePosX = 0, tflamePosY = 0;
int isFirst = 1;
int rdragonPosX = 0, rdragonPosY = 0, iniRdragonPosX = 0;
int jcolumnPosX = 0, jcolumnPosY = 0;

//-----------------------
// deep-blood-variables
//-----------------------
int deepBloodImg; // 血條圖片
mat4 offsetDeepBlood; // 血條矩陣，跟隨deep
float offsetDeepBloodLength; // 扣血

//------------------------------------------
//===========================================
//-----------------------
// firen-variables
//-----------------------
mat4 offsetFiren; // deep移動
int firenx; // 移動貼圖座標
int fireny; // 移動貼圖座標
int xMoveFiren, yMoveFiren;
Sprite2D* firenSheets[3];
Sprite2D* firenNormalSheets[3];
int objectCountFiren = 3;
int isLeftFiren; // 是不是左邊
int firenDirection; // 左上右下
int isFirenDie = 0;
int isFirenShine = 0;
int firenImage;

//-----------------------
// skill-variables
//-----------------------

mat4 offsetFirenSkill;
int firenSkillx;
int firenSkilly;
Sprite2D* firenSkillSheets[3];
glm::vec2 fireOffset[5];
glm::vec2 fireColumnOffset[5];
int firenSkillImage;
int drawSkillFiren;
int isFirstFiren = 1;
int bflamePosX = 0, bflamePosY = 0, inibflamePos = 0;
int flandPosX = 0, flandPosY = 0, iniflandPos = 0;
int fcolumnPosX = 0, fcolumnPosY = 0;

//-----------------------
// firen-blood-variables
//-----------------------
int firenBloodImg; // 血條圖片
mat4 offsetFirenBlood; // 血條矩陣，跟隨deep
float offsetFirenBloodLength; // 扣血
//===========================================
//------------------------------------------

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
//light-variables
//-----------------------
int lightImg;


//---------------------------------
//collision-variables
//----------------------------------
int isFirenHurt = 0;
int isDeepHurt = 0;
void firenHurt() {
	firenImage = 0;
	fireny = 4;
	if (offsetFirenBloodLength > 0) offsetFirenBloodLength -= 0.1f;
	deltatimeFiren = firenTime; // 計時開始，到1.2秒後deep才能再站起來
	isFirenHurt = 1;
}
void deepHurt() {
	deepImage = 0;
	deepy = 4;
	if (offsetDeepBloodLength > 0) offsetDeepBloodLength -= 0.1f;
	deltatime = deepTime; // 計時開始，到1.2秒後deep才能再站起來
	isDeepHurt = 1;
}




//-----------------------
// deep-vertices
//-----------------------
float deepVertices[] = {
	// positions          // colors           // texture coords for img 0/1
	0.1f,  0.12f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	0.1f, -0.12f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.1f, -0.12f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.1f,  0.12f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};

float deepTexCoord2[] = { // deep的第二張圖座標不同
	0.1f, 1.0f,// top right
	0.1f, 0.75f,// bottom right
	0.0f, 0.75f,// bottom left
	0.0f, 1.0f// top left
};

unsigned int deepIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};



//-----------------------
// skill-vertices
//-----------------------
float skillVertices[] = {
	// positions							// texture coords for img twinsflame/bat/sword-blow(orange/red/blue/yellow)
	0.08f,  0.08f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	0.08f, -0.08f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.08f, -0.08f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.08f,  0.08f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
float skillVertices2[] = { // 飛龍
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


//------------------------------------------
//===========================================
//-----------------------
// firen-vertices
//-----------------------
float firenVertices[] = {
	// positions          // colors           // texture coords for img 0/1
	0.1f,  0.12f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	0.1f, -0.12f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.1f, -0.12f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.1f,  0.12f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};



unsigned int firenIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

float firenPosX = 0, firenPosY = 0;
glm::vec4 firenPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//-----------------------
// skill-vertices
//-----------------------
float skillFirenVertices[] = {
	// positions							// texture coords for img twinsflame/bat/sword-blow(orange/red/blue/yellow)
	0.08f,  0.08f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	0.08f, -0.08f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.08f, -0.08f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.08f,  0.08f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
float skillFirenVertices2[] = { // 飛龍
	0.2f,  0.15f, 0.0f,     1.0f, 1.0f, // up-right
	0.2f, -0.15f, 0.0f,     1.0f, 0.0f, // down-right
	-0.2f, -0.15f, 0.0f,     0.0f, 0.0f,  // down-left
	-0.2f,  0.15f, 0.0f,     0.0f, 1.0f   // up-left
};
unsigned int skillFirenIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

//-----------------------
// firenblood-variables
//-----------------------
float firenBloodVertices[] = {
	// positions							// texture coords for img twinsflame/bat/sword-blow(orange/red/blue/yellow)
	0.05f,  0.01f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	0.05f, -0.01f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.05f, -0.01f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.05f,  0.01f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
unsigned int firenBloodIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};
//===========================================
//------------------------------------------


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
//粒子系統(learn opengl code)
//----------------------------------------
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

float particleLife, particleSpeed;

GLuint particleNum = 500; // particle總數
vec2 particleDir = vec2(1.0,1.0); 

std::vector<Particle> particles; // 裝particle的盒子
GLuint nr_new_particles = 2; // 每次要新增的particle
GLuint lastUsedParticle = 0; // 給FirstUnusedParticle()使用
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

GLfloat dt = jump_interval * 0.001f;

//----------------------------------------
//粒子系統下雨(learn opengl code)
//----------------------------------------
std::vector<Particle> particleRain; // 裝particle的盒子
GLuint lastUsedParticleRain = 0; // 給FirstUnusedParticle()使用
GLuint FirstUnusedParticleRain();
void RespawnParticleRain(Particle &particle);
GLuint particleRainNum = 2000; // particle總數
float rainLife, rainSpeed;
vec2 rainDir = vec2(1.0, 1.0);
GLuint particleRainImg;



//--------------------------------------
//framebufferobject小地圖
//--------------------------------------
float quadVertices2[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// 小地圖：右上-0.5,1.0：右下-0.5,0.5：左下-1.0,0.5：左上-1.0,1.0
	// positions   // texCoords
	0.5,   1.0,  0.0f, 1.0f, // 左上
	0.5,   0.5,  0.0f, 0.0f, // 左下
	1.0,   0.5,  1.0f, 0.0f, // 右下

	0.5,  1.0f,  0.0f, 1.0f, // 左上
	1,   0.5,  1.0f, 0.0f, // 右下
	1,   1.0,  1.0f, 1.0f // 右上
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
vec4 frameColor = vec4(1.0, 1.0, 1.0, 1.0);


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
GLuint ProjectionskyID; // this both matrix(矩陣) ID is for loading into skybox
GLuint ViewskyID;
GLuint cubemapTexture;

//int mode;
int action;
int xMoveDeep, yMoveDeep;

#define DOR(angle) (angle*3.1415f/180);

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
	double r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r) + (1 - cos(r))*x*x, (1 - cos(r))*y*x + sin(r)*z, (1 - cos(r))*z*x - sin(r)*y, 0);
	vec4 c2 = vec4((1 - cos(r))*y*x - sin(r)*z, cos(r) + (1 - cos(r))*y*y, (1 - cos(r))*z*y + sin(r)*x, 0);
	vec4 c3 = vec4((1 - cos(r))*z*x + sin(r)*y, (1 - cos(r))*z*y - sin(r)*x, cos(r) + (1 - cos(r))*z*z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}