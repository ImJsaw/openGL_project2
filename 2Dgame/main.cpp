#include "main.h"

//vec3 camera = vec3(0,0,20);
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4,3);//以OpenGL version4.3版本為基準
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("OpenGL 4.3 - 2D Game - LF2");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;//c error
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);

	glutTimerFunc(deep_interval, Deep_Timer, 0);
	glutTimerFunc(jump_interval, Jump_Timer, 0);
	glutTimerFunc(deep_walk_interval, Deep_Walk_Timer, 0);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboardup);

	//glutKeyboardFunc(Keyboard2);
	//glutKeyboardUpFunc(Keyboardup2);
	int ParticleNumMenu, ParticleSpeedMenu, ParticleDirectionMenu, ParticleLifeMenu;


	int ParticleMenu;

	ParticleNumMenu = glutCreateMenu(ParticleNumMenuEvents);//建立右鍵菜單
	glutAddMenuEntry("500", 0);
	glutAddMenuEntry("50", 1);
	glutAddMenuEntry("10", 2);

	ParticleSpeedMenu = glutCreateMenu(ParticleSpeedMenuEvents);//建立右鍵菜單
	glutAddMenuEntry("1", 0);
	glutAddMenuEntry("0.1", 1);
	glutAddMenuEntry("10", 2);
	glutAddMenuEntry("1000", 3);

	ParticleDirectionMenu = glutCreateMenu(ParticleDirectionMenuEvents);//建立右鍵菜單
	glutAddMenuEntry("up", 0);
	glutAddMenuEntry("left", 1);
	glutAddMenuEntry("down", 2);
	glutAddMenuEntry("right", 3);

	ParticleLifeMenu = glutCreateMenu(ParticleLifeMenuEvents);//建立右鍵菜單
	glutAddMenuEntry("1", 0);
	glutAddMenuEntry("5", 1);
	glutAddMenuEntry("0.1", 2);
	glutAddMenuEntry("10", 3);


	ParticleMenu = glutCreateMenu(ParticleMenuEvents);//建立右鍵菜單
												  //加入右鍵物件
	glutAddSubMenu("number", ParticleNumMenu);
	glutAddSubMenu("speed", ParticleSpeedMenu);
	glutAddSubMenu("direction", ParticleDirectionMenu);
	glutAddSubMenu("life", ParticleLifeMenu);


	glutCreateMenu(MenuEvents);//建立右鍵菜單，主菜單
	glutAddSubMenu("particles", ParticleMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯



	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutMainLoop();
	return 0;
}

void ChangeSize(int w,int h){
	if(h == 0) h = 1;
	glViewport(0,0,w,h);
	Projection = perspective(80.0f,(float)w/h,0.1f,100.0f);
}

void Deep_Timer(int val){
	glutPostRedisplay();
	glutTimerFunc(deep_interval, Deep_Timer, val);
	deepTime += deep_interval * deepSpeed * 0.001f;
	
	//讓燈源左右移動
	lightPos[0] = sin(deepTime);
	lightPos[2] = cos(deepTime);

	//---------------------------------------------
	//腳色的連續圖(特殊動作)
	//---------------------------------------------
	if (deepImage == 2) { // 火焰攻擊

		// 移動圖片deep
		if (isLeft == 0) {
			offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			offsetSkill = translate(deep_interval * 0.001f, 0, 0) * offsetSkill;
		}
		else if (isLeft == 1) {
			offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			offsetSkill = translate(-deep_interval * 0.001f, 0, 0) * offsetSkill;
		}

		// deep連續圖動畫
		if (deepx == 1 && deepy == 4) {
			deepImage = 0; // 換回圖片0
			deepx = 1; // 回到靜止圖
			deepy = 1;
		}
		else if ((deepy == 1 && deepx == 10) || (deepy == 3 && deepx == 10) || (deepy == 2 && deepx == 1) ) { // 往下一排走的條件
			deepy++;
		}
		else if ((deepy == 1 && deepx != 10) || (deepy == 3 && deepx != 10)) { // deepx 往右走
			deepx++;
		}
		else if ((deepy == 2 && deepx != 1) || (deepy == 4 && deepx != 1)) { // deepx 往左走
			deepx--;
		}

		
	}
	else if (deepImage == 1 && deepy == 4) { // 一般攻擊(在連續圖的第四行)
		if (isLeft == 0) {
			offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
		}
		else if (isLeft == 1) {
			offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
		}

		if (deepx == 10) {
			deepImage = 0; // 換回圖片0
			deepx = 1; // 回到靜止圖
			deepy = 1;
		}
		else if(deepx != 10){ // deepx 往右走
			deepx++;
		}
	}
	else if (deepImage == 1 && deepy == 2) { // 丟東西(圖片1的第二行)
		if (isLeft == 0) {
			offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
		}
		else if (isLeft == 1) {
			offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
		}

		if (deepx == 8) {
			deepImage = 0; // 換回圖片0
			deepx = 1; // 回到靜止圖
			deepy = 1;
		}
		else if (deepx != 10) { // deepx 往右走
			deepx++;
		}
	}
	else if (deepImage == 0 && deepy == 4) {
		if (isLeft == 0) {
			offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
		}
		else if (isLeft == 1) {
			offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
		}

		if (deepx == 6) {
			deepImage = 0; // 換回圖片0
			deepx = 1; // 回到靜止圖
			deepy = 1;
		}
		else if (deepx != 6) { // deepx 往右走
			deepx++;
		}
	}
	//---------------------------------------------
	//技能特效連續圖
	//---------------------------------------------
	if (drawSkill == 1 && skillImage == 0) { // 冰火劍氣播放連續圖
		
		if (isLeft == 0) {
			
			offsetSkill = translate((currentTime-deltatime)*10 * deep_interval * 0.0001f, 0, 0) * offsetSkill;
		}
		else if (isLeft == 1) {
			
			offsetSkill = translate((currentTime - deltatime) * 10 * -deep_interval * 0.0001f, 0, 0) * offsetSkill;
		}

		// skill 連續圖動畫
		if (skillx == 4 && skilly == 2) {
			skillx = 1; // 回到靜止圖
			skilly = 1;
			drawSkill = 0;
		}
		else if ((skilly == 1 && skillx != 4) || (skilly == 2 && skillx != 4)) {
			skillx++;
		}
		else if (skilly == 1 && skillx == 4) {
			skilly++;
			skillx = 1;
		}

	}
	else if (drawSkill == 1 && skillImage == 1) { // 紅龍播放連續圖
		
		if (isLeft == 0) {

			offsetSkill = translate(deep_interval * 0.002f, 0, 0) * offsetSkill;
		}
		else if (isLeft == 1) {

			offsetSkill = translate(-deep_interval * 0.002f, 0, 0) * offsetSkill;
		}

		// skill 連續圖動畫
		if (skillx == 2 && skilly == 5) {
			skillx = 1; // 回到靜止圖
			skilly = 1;
			drawSkill = 0;
		}
		else if (skillx != 2) { // x : 1 -> 2
			skillx++;
		}
		else if (skillx == 2) { // y : 1 -> 2 ； x : 1 
			skilly++;
			skillx = 1;
		}
	}
	else if (drawSkill == 1 && skillImage == 2) { // 朱利安柱子連續圖
		if (isLeft) offsetSkill = translate(-0.2f, 0, 0) * offset;
		else offsetSkill = translate(-0.2f, 0, 0) * offset;

		// skill 連續圖動畫
		if (skillx == 4 && skilly == 2) {
			skillx = 1; // 回到靜止圖
			skilly = 1;
			drawSkill = 0;
		}
		else if (skillx != 4) { // x : 1 -> 2
			skillx++;
		}
		else if (skillx == 4) { // y : 1 -> 2 ； x : 1 
			skilly++;
			skillx = 1;
		}
	}

	//---------------------------------------------
	//血條動畫
	//---------------------------------------------
	offsetDeepBlood = translate(0, 0.16f, 0) * offset;
	
}

void Deep_Walk_Timer(int val) {
	glutPostRedisplay();
	glutTimerFunc(deep_walk_interval, Deep_Walk_Timer, val);
	deepWalkTime += deep_walk_interval * 0.001f;
	/*if (deepDirection != -1) {//normal move
		offset = translate(deep_walk_interval * 0.0001* xMove, deep_walk_interval * 0.0001* yMove, 0) * offset;
		deepx++;
		if (deepx == 8) deepx = 3;
	}*/
}

void Jump_Timer(int val) {

	glutPostRedisplay();
	glutTimerFunc(jump_interval, Jump_Timer, val);
	currentTime += jump_interval * 0.001f; 

	

	// jump_interval 為 30 時，最接近正常每秒的速度
	if (deepController == 3 || deepController == 7) { // 跳起到著地的時間
		if ((currentTime - deltatime) >= time_for_a_jump) { // 要著地了，看向哪一方
			deltatime = 0.0f;
			is_move_when_jump = 0;
			if (deepController == 3) {
				deepController = 0; // 著地後，看向右方
			}
			else if (deepController == 7) {
				deepController = 4; // 著地後，看向左方
			}
		}
		else if ((currentTime - deltatime) < time_for_a_jump) {
			float radian = DOR((currentTime - deltatime) * (180 / (time_for_a_jump+0.017f)));
			// currentTime - deltatime : 0 到 time_for_a_jump(1.2)
			// 180/(time_for_a_jump+0.015) : 讓currentTime - deltatime從0到180(度)，0.015是誤差，計時器和正常時間有偏差
			if (is_move_when_jump == 1) { // 邊跳邊向右
				offset = translate(jump_interval * 0.0001f, 0, 0) * offset;
			}
			else if (is_move_when_jump == 2) { // 邊跳邊向左
				offset = translate(-jump_interval * 0.0001f, 0, 0) * offset;
			}
			offset = translate(0, cos( radian )*0.07f, 0) * offset; // 跳躍的矩陣
		}
	}
	if(deepDirection != -1){//normal move
		offset = translate(jump_interval * 0.001f* xMove, jump_interval * 0.001f* yMove, 0) * offset;
		deepx++;
		if (deepx == 8) deepx = 3;
	}

	//------------------------------
	//particle zone
	//-----------------------------
	// Add new particles
	for (GLuint i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		vec4 deepPos = offset * deepPosition; // deep的位置
		//printf("deepPos = %d , %d\n", deepPos[0], deepPos[1]);
		RespawnParticle(particles[unusedParticle], vec2(deepPos[0], deepPos[1]), vec2(0.1, 0.1));
		//printf("Respawn...\n");
		// (生命結束的粒子, 主角位置, 主角半徑)
	}
	// Uupdate all particles
	for (GLuint i = 0; i < particleNum; ++i)
	{
		Particle &p = particles[i];
		// jump_interval * 0.001 是 每一禎的時間差
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f){	// particle is alive, thus update
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5f;
		}
	}

}

//  deepController : 跳7,走65,站4:左   右:0站,12走,3跳
// deepDirection : 0左 1上 2右 3下
void Keyboard(unsigned char key, int x, int y) { // 各種按鈕按下去的反應
	switch (key) {
	case 'q': // 扣血
	case 'Q':
		deepImage = 0;
		deepy = 4;
		if (offsetDeepBloodLength > 0) offsetDeepBloodLength -= 0.1f;
		break;
	case 'e': // 朱利安柱
	case 'E':
		deepImage = 1;
		deepy = 2;
		skillImage = 2; // 朱利安柱子圖片
		if (isLeft) offsetSkill = offset;
		else offsetSkill = offset;
		deltatime = currentTime; // 讓柱子有向外飛的感覺
		drawSkill = 1;
		break;
	case 'z': // 一般攻擊狀態，飛龍特效啟動
	case 'Z':
		deepImage = 1;
		deepy = 4;

		skillImage = 1; // 劍氣圖片
		if (isLeft) offsetSkill = translate(-0.4f, 0, 0) * offset;
		else offsetSkill = translate(0.4f, 0, 0) * offset;
		drawSkill = 1;

		break;
	case 'x': // 火焰攻擊狀態，劍氣特效啟動
	case 'X':
		deepImage = 2;
		skillImage = 0;
		if(isLeft) offsetSkill = translate(-0.1f, 0, 0) * offset;
		else offsetSkill = translate(0.1f, 0, 0) * offset;
		drawSkill = 1;

		deltatime = currentTime;

		break;
	case 'c': // 跳躍
	case 'C':
		if (deepController == 1 || deepController == 0) { // 上一個時刻面向右邊
			
			if (deepController == 1) {
				is_move_when_jump = 1;
			}
			deltatime = currentTime;
			deepController = 3; // 換成向右跳躍的圖
		}
		else if(deepController == 5 || deepController == 4){ // 上一個時刻面向左邊
			
			if (deepController == 5) {
				is_move_when_jump = 2;
			}
			deltatime = currentTime;
			deepController = 7; // 換成向左跳躍的圖
		}
		 // 跳躍
		break;
	case 'W': // 往上走
	case 'w':
		deepDirection = 1;
		yMove++;
		break;
	case 'S': // 往下走
	case 's':
		deepDirection = 3;
		yMove--;
		break;
	case 'a':
	case 'A': // 往左走
		xMove--;
		if (deepController == 7) { // 上一個時刻向左跳
			deepController = 7; // 仍然向左
			is_move_when_jump = 2; // 邊跳邊向左移動
		}
		else if (deepController == 3) { // 上一個時刻向右跳
			deepController = 7; // 改成向左
			is_move_when_jump = 2; // 邊跳邊向左移動
		}
		else {
			deepController = 5; // 向左走
			isLeft = 1;
		}
		deepDirection = 0;
		break;
	case 'd':
	case 'D':
		xMove++;
		if (deepController == 7) { // 上一個時刻向左跳
			deepController = 3; // 改成向右
			is_move_when_jump = 1; // 邊跳邊向右移動
		}
		else if (deepController == 3) { // 上一個時刻向右跳
			deepController = 3; // 仍然向右
			is_move_when_jump = 1; // 邊跳邊向右移動
		}
		else {
			deepController = 1; // 向右走
			isLeft = 0;
		}
		deepDirection = 2;
		break;
	}
	glutPostRedisplay();
}
void Keyboardup(unsigned char key, int x, int y) { // 一般走路按鈕放開即停止
	switch (key) {
		
		case 'W': // 往上走
		case 'w':
			yMove--;
			if(!xMove) deepDirection = -1;
			deepx = 3; // 回到站立
			deepy = 1;
			break;
		case 'S': // 往下走
		case 's':
			yMove++;
			if (!xMove) deepDirection = -1;
			deepx = 3; // 回到站立
			deepy = 1;
			break;
		case 'a':
		case 'A':
			xMove++;
			if (deepController == 7) { // 上一個時刻向左跳
				deepController = 7; // 仍然向左
				is_move_when_jump = 0;
			}
			else if (deepController == 3) { // 上一個時刻向右跳
				deepController = 7; // 改成向左
				is_move_when_jump = 0;
			}
			else {
				deepController = 4; // 向左站立
				deepx = 1; // 回到站立
				deepy = 1;
			}
			if (!yMove) deepDirection = -1;
			
			break;
		case 'd':
		case 'D':
			xMove--;
			if (deepController == 7) { // 上一個時刻向左跳
				deepController = 3; // 改成向右
				is_move_when_jump = 0;
			}
			else if (deepController == 3) { // 上一個時刻向右跳
				deepController = 3; // 仍然向右
				is_move_when_jump = 0;
			}
			else {
				deepController = 0; // 向右站立
				deepx = 1; // 回到站立
				deepy = 1;
			}
			if (!yMove) deepDirection = -1;
			break;
	}
	glutPostRedisplay();
}



void init() {
	particleLife = 1.0f;
	particleSpeed = 0.1f;
	//-----------------------
	// deep-setting
	//-----------------------
	deepController = 0; // 向右站立
	offset = translate(-0.9f, 0, 0); // 初始化矩陣，只有腳色的矩陣translate到螢幕左邊
	deepx = 1; // 貼圖座標移動矩陣
	deepy = 1;
	xMove = 0;
	yMove = 0;

	is_move_when_jump = 0;

	isLeft = 0;
	deepDirection = -1; 
	deepImage = 0;

	ShaderInfo deepShader[] = {
		{ GL_VERTEX_SHADER, "deep.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "deep.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programDeep = LoadShaders(deepShader);//讀取shader

	glUseProgram(programDeep);//uniform參數數值前必須先use shader

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(deepVertices) +sizeof(deepTexCoord2), deepVertices, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(deepVertices), sizeof(deepTexCoord2), deepTexCoord2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deepIndices), deepIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute(for image1)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(32 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glUseProgram(programDeep);
	
	//deep_0 = loadTexture("sys/deep_0.png");
	//deep_1 = loadTexture("sys/deep_1.png");
	//deep_2 = loadTexture("sys/deep_2.png");
	
	for (int spriteID = 0; spriteID < objectCount; ++spriteID)
	{
		deepSheets[spriteID] = new Sprite2D();
	}
	deepSheets[0]->Init("sys/deep_0.png", 7, 10, 40);
	deepSheets[1]->Init("sys/deep_1.png", 7, 10, 40);
	deepSheets[2]->Init("sys/deep_2.png", 4, 10, 40);
	for (int spriteID = 0; spriteID < objectCount; ++spriteID)
	{
		deepNormalSheets[spriteID] = new Sprite2D();
	}
	deepNormalSheets[0]->Init("sys/deep_0_normal.png", 7, 10, 40);
	deepNormalSheets[1]->Init("sys/deep_1_normal.png", 7, 10, 40);
	deepNormalSheets[2]->Init("sys/deep_2_normal.png", 4, 10, 40);

	//glUniform1i(glGetUniformLocation(programDeep, "deep_0"), 0);
	//glUniform1i(glGetUniformLocation(programDeep, "deep_1"), 1);
	//glUniform1i(glGetUniformLocation(programDeep, "deep_2"), 2);

	deepcontrollerID = glGetUniformLocation(programDeep, "mario_controller");
	timeID = glGetUniformLocation(programDeep, "time");
	offsetID = glGetUniformLocation(programDeep, "offset"); // 少了這行，讓offset沒有傳入，使得position*offset未知，圖跑不出來
	deepxID = glGetUniformLocation(programDeep, "deepx");
	deepyID = glGetUniformLocation(programDeep, "deepy");
	isLeftID = glGetUniformLocation(programDeep, "isLeft");
	deepImageID = glGetUniformLocation(programDeep, "deepImage");
	projectionDeepID = glGetUniformLocation(programDeep, "projection");
	viewDeepID = glGetUniformLocation(programDeep, "view");
	viewPosDeepID = glGetUniformLocation(programDeep, "viewPos");
	lightPosDeepID = glGetUniformLocation(programDeep, "lightPos");

	//-----------------------
	// skill-setting
	//-----------------------
	offsetSkill = scale(1,1,1);
	skillx = 1;
	skilly = 1;
	skillImage = 0;
	drawSkill = 0;

	ShaderInfo skillShader[] = {
		{ GL_VERTEX_SHADER, "deepskill.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "deepskill.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programSkill = LoadShaders(skillShader);//讀取shader

	glUseProgram(programSkill);//uniform參數數值前必須先use shader

	//---------------------------------
	//dragon instance initialize(五個龍的位置offset)
	//---------------------------------
	int index = 0;
	for (int y = -2; y <= 2; y++)
	{
		glm::vec2 translation;
		translation.x = abs(y * 0.1f);
		translation.y = y * 0.1f;
		dragonOffset[index++] = translation;
	}
	// --------------------------------------
	// store instance data in an array buffer(一次生成五龍，該VBO放置算好的偏差值)
	// --------------------------------------
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 5, &dragonOffset[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//---------------------------------
	//dragon instance initialize(五個柱子的位置offset)
	//---------------------------------
	int index2 = 0;
	for (int x = -6; x <= 6; x+=3)
	{
		glm::vec2 translation;
		translation.x = x * 0.1f;
		translation.y = 0;
		columnOffset[index2++] = translation;
	}
	// --------------------------------------
	// store instance data in an array buffer(一次生成五龍，該VBO放置算好的偏差值)
	// --------------------------------------
	unsigned int instanceVBO2;
	glGenBuffers(1, &instanceVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 5, &columnOffset[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// --------------------------------------
	// normal VAO VBO EBO
	// --------------------------------------
	glGenVertexArrays(1, &VAOskill);
	glGenBuffers(1, &VBOskill);
	glGenBuffers(1, &EBOskill);

	glBindVertexArray(VAOskill);

	glBindBuffer(GL_ARRAY_BUFFER, VBOskill);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skillVertices)+sizeof(skillVertices2), skillVertices, GL_STATIC_DRAW); // A+B的空間中先綁定A
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(skillVertices), sizeof(skillVertices2), skillVertices2); // 再綁定B，才不會圖片有問題

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOskill);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deepIndices), deepIndices, GL_STATIC_DRAW);

	//---------------------------------
	//sword-light
	//---------------------------------
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute(for image-刀流)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//---------------------------------
	//dragon-fly
	//---------------------------------
	// position aattribute(dragon is longer than sword-light)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(32 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// texture coord attribute(for image-dragon)
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(35 * sizeof(float)));
	glEnableVertexAttribArray(4);

	// 五個龍的偏差值，另外傳入(因為使用的是不同的VBO)
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // 從instanceVBO傳入的
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(5, 1); // tell OpenGL this is an instanced vertex attribute.
	//(index of attribpointer, update/indice)

	// 五個朱利安柱子的偏差值，另外傳入(因為使用的是不同的VBO)
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO2); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // 從instanceVBO傳入的
	glEnableVertexAttribArray(6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(6, 1); // tell OpenGL this is an instanced vertex attribute.
	//(index of attribpointer, update/indice)


	glUseProgram(programSkill);

	twinsflame = loadTexture("sys/twinsflame.png");
	firedragon = loadTexture("sys/fire_dragon.png");
	juliancolumn = loadTexture("sys/julianColumn.png");
	//deep_1 = loadTexture("sys/deep_1.png");
	//deep_2 = loadTexture("sys/deep_2.png");

	glUniform1i(glGetUniformLocation(programSkill, "twinsflame"), 0);
	glUniform1i(glGetUniformLocation(programSkill, "firedragon"), 1);
	glUniform1i(glGetUniformLocation(programSkill, "julianColumn"), 2);
	//glUniform1i(glGetUniformLocation(programs, "deep_2"), 2);

	
	offsetSkillID = glGetUniformLocation(programSkill, "offset"); // 少了這行，讓offset沒有傳入，使得position*offset未知，圖跑不出來
	skillxID = glGetUniformLocation(programSkill, "skillx");
	skillyID = glGetUniformLocation(programSkill, "skilly");
	isLeftSkillID = glGetUniformLocation(programSkill, "isLeft");
	skillImageID = glGetUniformLocation(programSkill, "skillImage");
	skillTimeID = glGetUniformLocation(programSkill, "time");


	//----------------------------
	//deepblood setting
	//---------------------------
	offsetDeepBlood = translate(0, 0.16f, 0);
	offsetDeepBloodLength = 1;

	ShaderInfo deepBloodShader[] = {
		{ GL_VERTEX_SHADER, "deepblood.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "deepblood.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programDeepBlood = LoadShaders(deepBloodShader);//讀取shader

	glUseProgram(programDeepBlood);//uniform參數數值前必須先use shader

	glGenVertexArrays(1, &VAOdb);
	glGenBuffers(1, &VBOdb);
	glGenBuffers(1, &EBOdb);

	glBindVertexArray(VAOdb);

	glBindBuffer(GL_ARRAY_BUFFER, VBOdb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(deepBloodVertices), deepBloodVertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deepBloodIndices), deepBloodIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute(for image1)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glUseProgram(programDeepBlood);

	deepBloodImg = loadTexture("sys/brightred.jpg");

	glUniform1i(glGetUniformLocation(programDeepBlood, "deepBlood"), 0);
	
	offsetDeepBloodID = glGetUniformLocation(programDeepBlood, "offset"); // 少了這行，讓offset沒有傳入，使得position*offset未知，圖跑不出來
	offsetDeepBloodLengthID = glGetUniformLocation(programDeepBlood, "offsetBlood");

	//----------------------------
	//background setting
	//---------------------------

	ShaderInfo backShader[] = {
		{ GL_VERTEX_SHADER, "back.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "back.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programBack = LoadShaders(backShader);//讀取shader

	glUseProgram(programBack);//uniform參數數值前必須先use shader

	glGenVertexArrays(1, &VAOb);
	glGenBuffers(1, &VBOb);
	glGenBuffers(1, &EBOb);
	glBindVertexArray(VAOb);

	glBindBuffer(GL_ARRAY_BUFFER, VBOb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backVertices), backVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backIndices), backIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute(for image1)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glUseProgram(programBack);

	backgroundImg = loadTexture("bk/back03.jpg");

	glUniform1i(glGetUniformLocation(programBack, "back"), 0);

	//--------------------------
	//particle system
	//--------------------------

	offsetParticle = scale(1,1,1);

	// debug，記得改shader
	ShaderInfo particleShader[] = {
		{ GL_VERTEX_SHADER, "particle.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "particle.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programParticle = LoadShaders(particleShader);//讀取shader

	glUseProgram(programParticle);//uniform參數數值前必須先use shader
	
	glGenVertexArrays(1, &VAOp);
	glGenBuffers(1, &VBOp);
	glGenBuffers(1, &EBOp);
	glBindVertexArray(VAOp);

	glBindBuffer(GL_ARRAY_BUFFER, VBOp);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backIndices), backIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute(for image1)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glUseProgram(programParticle);

	particleImg = loadTexture("sys/particle.png");

	glUniform1i(glGetUniformLocation(programParticle, "sprite"), 0);

	offsetParticleID = glGetUniformLocation(programParticle, "offset"); // 少了這行，讓offset沒有傳入，使得position*offset未知，圖跑不出來
	colorParticleID = glGetUniformLocation(programParticle, "color");
	projectionID = glGetUniformLocation(programParticle, "projection");
	offsetParticleMatrixID = glGetUniformLocation(programParticle, "offsetMat");
	particleTimeID = glGetUniformLocation(programParticle, "time");
	particleLifeID = glGetUniformLocation(programParticle, "life");

	for (GLuint i = 0; i < particleNum; ++i)
		particles.push_back(Particle());

	//------------------------------------------------------------
	//framebuffer zone : 將螢幕畫面取下，經過後製特效，再傳回的方式
	//------------------------------------------------------------
	ShaderInfo shaderframe[] = {
		{ GL_VERTEX_SHADER, "framebuffer.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "framebuffer.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programFrame = LoadShaders(shaderframe);//讀取shader

	glUseProgram(programFrame);//uniform參數數值前必須先use shader

	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);


	glUseProgram(programFrame);
	glUniform1i(glGetUniformLocation(programFrame, "screenTexture"), 0);

	// framebuffer configuration
	// -------------------------
	
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void display() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//-------------------------------------------
	//夾心餅乾，夾在framebuffer中間
	//-------------------------------------------
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//-----------------------
	// background-draw
	//-----------------------
	// bind textures on corresponding texture units
	glUseProgram(programBack);
	glBindVertexArray(VAOb);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backgroundImg);
	
	glUseProgram(programBack);
	glBindVertexArray(VAOb);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	
	//-----------------------
	// skill-draw
	//-----------------------
	// 在deep之後畫skill，skill才能跟deep一起移動
	glUseProgram(programSkill);
	glBindVertexArray(VAOskill);
	glUniformMatrix4fv(offsetSkillID, 1, false, &offsetSkill[0][0]);
	glUniform1i(skillxID, skillx);
	glUniform1i(skillyID, skilly);
	glUniform1i(isLeftSkillID, isLeft);
	glUniform1i(skillImageID, skillImage);
	glUniform1f(skillTimeID, currentTime - deltatime);
	// bind textures on corresponding texture units

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, twinsflame);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, firedragon);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, juliancolumn);

	/*glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, deep_2);*/
	// render container
	glUseProgram(programSkill);
	glBindVertexArray(VAOskill);


	if (drawSkill == 1) {
		if (skillImage == 0) {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else if (skillImage == 1) {
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 5);
			glBindVertexArray(0);
		}
		else if (skillImage == 2) {
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 5);
			glBindVertexArray(0);
		}
	}
	
	// -----------------------
	//deep-particle draw
	// -----------------------
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glUseProgram(programParticle);
	glBindVertexArray(VAOp);
	for (Particle particle : particles){
		if (particle.Life > 0.0f){
			glUniform2fv(offsetParticleID, 1, &particle.Position[0]);
			glUniform4fv(colorParticleID, 1, &particle.Color[0]);
			glUniformMatrix4fv(projectionID, 1, false, &projection[0][0]);
			glUniformMatrix4fv(offsetParticleMatrixID, 1, false, &offsetParticle[0][0]);
			glUniform1f(particleTimeID, currentTime);
			glUniform1f(particleLifeID, particle.Life);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, particleImg);
			glBindVertexArray(VAOp);
			glUseProgram(programParticle);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
	// Don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//-----------------------
	// deep-draw
	//-----------------------
	glUseProgram(programDeep); // 記得要useprogram 和 vao，不然東西會綁到上一個skillProgram中
	glBindVertexArray(VAO);

	glUniform1i(deepcontrollerID, deepController);
	glUniform1f(timeID, deepTime);
	glUniformMatrix4fv(offsetID, 1, false, &offset[0][0]);
	glUniform1i(deepxID, deepx);
	glUniform1i(deepyID, deepy);
	glUniform1i(isLeftID, isLeft);
	glUniform1i(deepImageID, deepImage);
	// bind textures on corresponding texture units
	glm::mat4 projection = glm::ortho(0.0f, SCR_WIDTH, SCR_HEIGHT, 0.0f, -1.0f, 1.0f);  
	glm::mat4 view = camera.GetViewMatrix();
	glUseProgram(programDeep);
	glUniformMatrix4fv(projectionDeepID, 1, false, &projection[0][0]);
	glUniformMatrix4fv(viewDeepID, 1, false, &view[0][0]);

	glUniform3fv(viewPosDeepID, 1, &camera.Position[0]);
	glUniform3fv(lightPosDeepID, 1, &lightPos[0]);


	glActiveTexture(GL_TEXTURE0);
	deepSheets[deepImage]->Enable();
	glActiveTexture(GL_TEXTURE1);
	deepNormalSheets[deepImage]->Enable();


	// render container
	glUseProgram(programDeep);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	deepSheets[deepImage]->Disable();
	deepNormalSheets[deepImage]->Disable();
	//-----------------------
	// deepblood-draw
	//-----------------------
	glUseProgram(programDeepBlood); // 記得要useprogram 和 vao，不然東西會綁到上一個skillProgram中
	glBindVertexArray(VAOdb);

	glUniformMatrix4fv(offsetDeepBloodID, 1, false, &offsetDeepBlood[0][0]);
	glUniform1f(offsetDeepBloodLengthID, offsetDeepBloodLength);
	
	// bind textures on corresponding texture units

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, deepBloodImg);
	
	// render container
	glUseProgram(programDeepBlood);
	glBindVertexArray(VAOdb);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	//------------------------------------
	//framebufferobject--mainmap(因為原本畫在自己的framebuffer，回到原本的buffer)
	//-----------------------------------


	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (really necessery actually, since we will be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT); // 先清空主要buffer的存的pixel顏色

	glUseProgram(programFrame);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices1), &quadVertices1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//screenShader.use();
	glUseProgram(programFrame);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);



	//------------------------------------
	//framebufferobject--mipmap
	//-----------------------------------
	

	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (really necessery actually, since we will be able to see behind the quad anyways)
	//glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(programFrame);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices2), &quadVertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//screenShader.use();
	glUseProgram(programFrame);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);





	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}

GLuint FirstUnusedParticle()
{
	// Search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < particleNum; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Override first particle if all others are alive
	lastUsedParticle = 0;
	return 0;
}

void RespawnParticle(Particle &particle, glm::vec2 charPos, glm::vec2 offsett)
{
	//GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat random = ((rand() % 100) - 99) / 5000.0f;
	GLfloat random2 = ((rand() % 100) - 99) / 5000.0f;
	GLfloat rColor = 0.5f + ((rand() % 100) / 100.0f);
	//particle.Position = charPos + random + offsett; // 主角位置 + 隨機 + 主角半徑
	particle.Position = vec2(charPos.x + random, charPos.y + random2);
	offsetParticle = translate(particle.Position.x, particle.Position.y, 0.0) * offsetParticle;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = particleLife;
	particle.Velocity = vec2(dt, dt) * particleSpeed;
	//printf("%d , %d\n", particle.Position.x, particle.Position.y);
}

static inline float random_float(){
	float res;
	unsigned int tmp;
	seed *= 16807;
	tmp = seed ^ (seed >> 4) ^ (seed << 15);
	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;
	return (res - 1.0f);
}


void MenuEvents(int option) {}
void ParticleMenuEvents(int option) {}

void ParticleNumMenuEvents(int option) {
	switch (option) {
	case 0:
		particleNum = 500;
		break;
	case 1:
		particleNum = 50;
		break;
	case 2:
		particleNum = 10;
		break;
	}
}
void ParticleSpeedMenuEvents(int option) {
	switch (option) {
	case 0:
		particleSpeed = 0.1f;
		break;
	case 1:
		particleSpeed = 0.5f;
		break;
	case 2:
		particleSpeed = 0.01f;
		break;
	case 3:
		particleSpeed = 1.0f;
		break;
	}
	cout << "particleSpeed : " << particleSpeed << endl;
}
void ParticleDirectionMenuEvents(int option) {
	switch (option) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}
void ParticleLifeMenuEvents(int option) {
	switch (option) {
	case 0:
		particleLife = 1.0f;
		break;
	case 1:
		particleLife = 5.0f;
		break;
	case 2:
		particleLife = 0.1f;
		break;
	case 3:
		particleLife = 10.0f;
		break;
	}
	cout << "particleLife : " << particleLife << endl;
}