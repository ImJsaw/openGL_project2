#include "main.h"

vec3 camera = vec3(0,0,20);

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4,3);//以OpenGL version4.3版本為基準
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 4.3 - Robot");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//init(); // 初始化最先
	init2();
	glutDisplayFunc(display2);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(deep_interval, Deep_Timer, 0);
	glutTimerFunc(jump_interval, Jump_Timer, 0);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboardup);
	/*int ActionMenu,ModeMenu,ShaderMenu;

	ActionMenu = glutCreateMenu(ActionMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("idle",0);
	glutAddMenuEntry("walk",1);
	glutAddMenuEntry("dance1", 2);
	glutAddMenuEntry("gangnam style", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ModeMenu = glutCreateMenu(ModeMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Line",0);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Reverse", 2);
	glutAddMenuEntry("Gray", 3);
	glutAddMenuEntry("Shine", 4);
	glutAddMenuEntry("Mirrow", 5);
	glutAddMenuEntry("Toon", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯


	ShaderMenu = glutCreateMenu(ShaderMenuEvents);
	
	glutAddMenuEntry("Normal", 0);
	glutAddMenuEntry("Inverse", 1);
	glutAddMenuEntry("GrayScale", 2);
	glutAddMenuEntry("Kernal effect", 3);
	glutAddMenuEntry("Blur", 4);
	glutAddMenuEntry("Edge detection", 5);
	glutAddMenuEntry("Circle Pixel", 6);
	glutAddMenuEntry("Emboss", 7);
	glutAddMenuEntry("Triangle Pixel", 8);
	glutAddMenuEntry("Rectangular Pixel", 9);
	glutAddMenuEntry("Sketch", 10);
	glutAddMenuEntry("Water Wave", 11);
	glutAddMenuEntry("lightning", 12);
	glutAddMenuEntry("Snow", 13);
	glutAddMenuEntry("RotatingRing", 14); // 2018/11/11new add

	glutAddMenuEntry("Laser", 11);
	glutAddMenuEntry("lightning", 12);
	glutAddMenuEntry("Snow", 13);
	glutAddMenuEntry("RotatingRing", 14);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutCreateMenu(menuEvents);//建立右鍵菜單，主菜單
	//加入右鍵物件
	glutAddSubMenu("action",ActionMenu);
	glutAddSubMenu("mode",ModeMenu);
	glutAddSubMenu("robot shader", ShaderMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯*/



	//glutMouseFunc(Mouse);
	//glutTimerFunc (100, idle, 0); // 計數器 : 每100毫秒計一次，並將0傳入idle函式
	glutMainLoop();
	return 0;
}


void ChangeSize(int w,int h){
	if(h == 0) h = 1;
	glViewport(0,0,w,h);
	Projection = perspective(80.0f,(float)w/h,0.1f,100.0f);
}

void Deep_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(deep_interval, Deep_Timer, val);
	deepTime += deep_interval * deepSpeed * 0.001;
	
	
	if (deepImage == 2) { 
		if (isLeft == 0) {
			offset = translate(deep_interval * 0.0005, 0, 0) * offset;
		}
		else if (isLeft == 1) {
			offset = translate(-deep_interval * 0.0005, 0, 0) * offset;
		}

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
	

}

void Jump_Timer(int val) {

	glutPostRedisplay();
	glutTimerFunc(jump_interval, Jump_Timer, val);
	currentTime += jump_interval * 0.001; 
	// jump_interval 為 30 時，最接近正常每秒的速度

	if (deepController == 3 || deepController == 7) { // 設定馬力歐跳起到著地的時間


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
			float radian = DOR((currentTime - deltatime) * (180 / (time_for_a_jump+0.017)));
			// currentTime - deltatime : 0 到 time_for_a_jump(1.2)
			// 180/(time_for_a_jump+0.015) : 讓currentTime - deltatime從0到180(度)，0.015是誤差，計時器和正常時間有偏差
			if (is_move_when_jump == 1) { // 邊跳邊向右
				offset = translate(jump_interval * 0.001, 0, 0) * offset;
			}
			else if (is_move_when_jump == 2) { // 邊跳邊向左
				offset = translate(-jump_interval * 0.001, 0, 0) * offset;
			}
			offset = translate(0, cos( radian )*0.05, 0) * offset; // 跳躍的矩陣
			
		}
	}

	// 左上右下分別考慮，停下來deepDirection是-1；左右方向用isLeft，右0左1
	else if (deepDirection == 0) {
		offset = translate(-jump_interval * 0.001, 0, 0) * offset;
		if (deepx == 8) {
			deepx = 1;
		}
		else {
			deepx++;
		}
		printf("%d\n", deepx);
	}
	else if (deepDirection == 2) {
		offset = translate(jump_interval * 0.001, 0, 0) * offset;
		if (deepx == 8) {
			deepx = 1;
		}
		else {
			deepx++;
		}

	}
	else if (deepDirection == 1) {
		offset = translate(0, jump_interval * 0.001, 0) * offset;
		if (deepx == 8) {
			deepx = 1;
		}
		else {
			deepx++;
		}

	}
	else if (deepDirection == 3) {
		offset = translate(0, -jump_interval * 0.001, 0) * offset;
		if (deepx == 8) {
			deepx = 1;
		}
		else {
			deepx++;
		}

	}

}

//  deepController : 跳7,走65,站4:左   右:0站,12走,3跳
// deepDirection : 0左 1上 2右 3下
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'x': // 攻擊狀態
	case 'X':
		deepImage = 2;
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
		break;
	case 'S': // 往下走
	case 's':
		deepDirection = 3;
		break;
	case 'a':
	case 'A': // 往左走
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
void Keyboardup(unsigned char key, int x, int y) {
	switch (key) {
		
		case 'W': // 往上走
		case 'w':
			deepDirection = -1;
			deepx = 1; // 回到站立
			deepy = 1;
			break;
		case 'S': // 往下走
		case 's':
			deepDirection = -1;
			deepx = 1; // 回到站立
			deepy = 1;
			break;
		case 'a':
		case 'A':
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
			deepDirection = -1;
			
			break;
		case 'd':
		case 'D':
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
			deepDirection = -1;
			break;
	}
	glutPostRedisplay();
}
/*
void Mouse(int button,int state,int x,int y){
	if(button == 2) isFrame = false;
}
void idle(int dummy){
	isFrame = true;
	int out = 0;

	if (action == GANGNAM) {
		if (isMotionSwitch == -1) { // isMotionSwitch == -1 時，代表上一秒是其他動作，要讓動作從頭開始
			resetObj(dummy);// 讓動作從頭開始(至idle狀態)
			updateObj3(out);// 接著，讓動作開始第一個狀態
			isMotionSwitch = 3; // isMotionSwitch = 3，數字0,1,2,3代表不同狀態(idle, walk, dance, gangnam)，因此不會再進入這個迴圈
		}
		else {
			updateObj3(dummy);
			out = dummy + 1;
			if (out > 12) out = 1;
		}
		
	}
	else if (action == DANCE) {
		if (isMotionSwitch == -1) {
			resetObj(dummy);
			updateObj2(out);
			isMotionSwitch = 2;
		}
		else {
			updateObj2(dummy);
			out = dummy + 1;
			if (out > 12) out = 1;
		}
		
	}
	else if(action == WALK){
		if (isMotionSwitch == -1) {
			resetObj(dummy);
			updateObj(out);
			isMotionSwitch = 1;
		}
		else {
			updateObj(dummy);
			out = dummy + 1;
			if (out > 12) out = 1;
		}
		
	}
	else if(action == IDLE){
		if (isMotionSwitch == -1) {
			isMotionSwitch = 0;
		}
		resetObj(dummy);
		out = 0;

	}
	glutPostRedisplay();
	
	glutTimerFunc (150, idle, out); 
}
void resetObj(int f){
	for(int i = 0 ; i < PARTSNUM;i++){
		angles[i] = 0.0f;
		zangles[i] = 0.0f;
		yangles[i] = 0.0f;
	}
	//zangles[1] = 10;
	//zangles[6] = -10;
	//zangles[12] = 10;
	//zangles[15] = -10;
}
void updateObj(int frame){
	switch(frame){
	case 0:

		//左手
		//angles[2] = -45;//前臂前彎
		//右手

		//腿
		angles[13] = 45;//小腿後彎	
		angles[16] = 45;
		//angles[13] = 90;

		break;
	case 1:
	case 2:
	case 3:
		angles[1] +=10;//左上手臂先往後
		angles[6] -= 10;//右上手臂先往前
		angles[12] -=15;//左大腿先往前
		angles[15] += 15;//右大腿先往後
		angles[16] -= 15;
		position += 0.1;//骨盆以上向正y軸移動
		break;
	case 4:
	case 5:
	case 6:
		angles[1] -=10;
		angles[6] += 10;
		angles[12] += 15;//左大腿往後
		angles[15] -= 15;//右大腿往前
		angles[13] -= 15;//左小腿前彎
		angles[16] = 0;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		angles[1] -=10;
		angles[6] += 10;
		angles[12] += 15;
		angles[15] -= 15;
		angles[13] = 0;//小腿打直
		angles[16] += 15;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		angles[1] +=10;
		angles[6] -= 10;
		angles[12] -= 15;
		angles[15] += 15;
		angles[13] += 15;//小腿後彎
		
		position -= 0.1;
		break;
	}
}

void updateObj2(int frame) {


	switch (frame) {
	case 0:

		resetObj(0);

		//左手
		//angles[2] = -45;//前臂前彎
						//右手

						//腿
		angles[13] = 30;//小腿後彎	
		angles[16] = 30;				
		//angles[13] = 90;

		break;
	case 1:
	case 2:
	case 3:
		zangles[1] += 10;//左上手臂向左甩
		zangles[6] -= 10;//右上手臂向右甩
		zangles[12] += 10;
		zangles[15] -= 10;
		position += 0.1;//骨盆以上向正y軸移動
		break;
	case 4:
	case 5:
	case 6:
		zangles[1] += 10;
		zangles[6] -= 10;
		zangles[12] += 10;
		zangles[15] -= 10;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		zangles[1] -= 10;
		zangles[6] += 10;
		zangles[12] -= 10;
		zangles[15] += 10;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		zangles[1] -= 10;
		zangles[6] += 10;
		zangles[12] -= 10;
		zangles[15] += 10;
		position -= 0.1;
		break;
	}
}

void updateObj3(int frame) {
	switch (frame) {
	case 0:
		angles[1] = -45;//左上手臂前彎(負值)
		angles[6] = -50;//右上手臂前彎
		//angles[2] = -45;//雙前臂前彎
		yangles[1] = -90;//左上手臂內旋
		yangles[6] = 90;//右上手臂內旋

		zangles[12] = 30;//左大腿外開
		zangles[15] = -30;
		yangles[12] = 30;//左大腿外八(外旋使用y軸轉動)
		yangles[15] = -30;
		angles[12] = -30;//左大腿向前
		angles[15] = -30;

		angles[13] = 45;//小腿後彎
		angles[16] = 45;

		angles[14] = 45;//雙腳掌顛腳
		angles[17] = 45;


		break;
	case 1:
	case 2:
	case 3:
		zangles[1] -= 5;
		zangles[6] -= 5;//雙手左右晃動
		angles[1] -= 5;
		angles[6] -= 5;
		angles[12] -= 6;//左大腿上提
		angles[13] += 8;//左小腿上提
		position += 0.1;//骨盆以上向正y軸移動
		break;
	case 4:
	case 5:
	case 6:
		zangles[1] += 5;
		zangles[6] += 5;
		angles[1] += 5;
		angles[6] += 5;
		angles[12] += 6;
		angles[13] -= 8;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		zangles[1] += 5;
		zangles[6] += 5;
		angles[1] -= 5;
		angles[6] -= 5;
		angles[15] -= 6;
		angles[16] += 8;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		zangles[1] -= 5;
		zangles[6] -= 5;
		angles[1] += 5;
		angles[6] += 5;
		angles[15] += 6;
		angles[16] -= 8;
		position -= 0.1;
		break;
	}
}
//儲存material 的子變數的ID
 GLuint M_KaID;
 GLuint M_KdID;
 GLuint M_KsID;

void init(){
	isFrame = false;
	pNo = 0;
	mode = 0;
	isMotionSwitch = 0;
	for (int i = 0; i < PARTSNUM; i++) {
		//初始化角度陣列
		angles[i] = 0.0;
		zangles[i] = 0.0;
		yangles[i] = 0.0;
	}
	//VAO
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "DSPhong_Material.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "DSPhong_Material.fp" },//fragment shader
		{ GL_NONE, NULL }};
	program = LoadShaders(shaders);//讀取shader

	glUseProgram(program);//uniform參數數值前必須先use shader
	
	MatricesIdx = glGetUniformBlockIndex(program,"MatVP");//matvp 有 view 和 projection 矩陣
	ModelID =  glGetUniformLocation(program,"Model");//矩陣
    M_KaID = glGetUniformLocation(program,"Material.Ka");
	M_KdID = glGetUniformLocation(program,"Material.Kd");
	M_KsID = glGetUniformLocation(program,"Material.Ks");
	//or
	M_KdID = M_KaID+1;
	M_KsID = M_KaID+2;
	ViewPosID = glGetUniformLocation(program, "ViewPos");//將相機位置傳入

	Projection = glm::perspective(80.0f,4.0f/3.0f,0.1f,10000.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	View       = glm::lookAt(
		glm::vec3(0,10,25) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);

	Obj2Buffer();// load進obj，並將其頂點, 材質, 法向量裝在三個VBO中

	//UBO
	glGenBuffers(1,&UBO);
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(mat4)*2,NULL,GL_DYNAMIC_DRAW);
	//get uniform struct size
	int UBOsize = 0;
	glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);//取得在shader宣告的uniform "MatVP" 的大小，存入UBOsize
	//bind UBO to its idx
	glBindBufferRange(GL_UNIFORM_BUFFER,0,UBO,0,UBOsize);//和記憶體要求UBOsize的空間，將UBO綁定為GL_UNIFORM_BUFFER屬性
	glUniformBlockBinding(program, MatricesIdx,0);

	modeID = glGetUniformLocation(program, "mode"); // 給robot本身更改特效的，先要到劃一參數的位置，以利之後傳遞數值
	timeID = glGetUniformLocation(program, "time");

	CubemapID = glGetUniformLocation(program, "skybox");//給機器人加鏡像特效
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);


	//skybox zone
	ShaderInfo shadersky[] = {
		{ GL_VERTEX_SHADER, "skybox.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "skybox.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programs = LoadShaders(shadersky);//讀取shader

	glUseProgram(programs);//uniform參數數值前必須先use shader

						   // skybox VAO

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	vector<std::string> faces
	{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};
	cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	glUseProgram(programs);

	ViewskyID = glGetUniformLocation(programs, "view");
	ProjectionskyID = glGetUniformLocation(programs, "projection");
	
	//skyboxShader.setInt("skybox", 0);
	glUniform1i(glGetUniformLocation(programs, "skybox"), 0);


	//framebuffer zone : 將螢幕畫面取下，經過後製特效，再傳回的方式
	ShaderInfo shaderframe[] = {
		{ GL_VERTEX_SHADER, "framebuffer.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "framebuffer.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programf = LoadShaders(shaderframe);//讀取shader

	glUseProgram(programf);//uniform參數數值前必須先use shader

	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	

	glUseProgram(programf);
	glUniform1i(glGetUniformLocation(programf, "screenTexture"), 0);

	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	pNoID = glGetUniformLocation(programf, "mode");
	lenID = glGetUniformLocation(programf, "len");
	frametimeID = glGetUniformLocation(programf, "time");

	glClearColor(0.0,0.0,0.0,1);//black screen
}
*/
void init2() {

	deepController = 0; // 向右站立
	offset = scale(1, 1, 1); // 初始化矩陣
	deepx = 1; // 貼圖座標移動矩陣
	deepy = 1;

	is_move_when_jump = 0;

	isLeft = 0;
	deepDirection = -1; 
	deepImage = 0;

	ShaderInfo shadersky[] = {
		{ GL_VERTEX_SHADER, "deep.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "deep.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programs = LoadShaders(shadersky);//讀取shader

	glUseProgram(programs);//uniform參數數值前必須先use shader

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

	glUseProgram(programs);
	
	deep_0 = loadTexture("sys/deep_0.png");
	deep_1 = loadTexture("sys/deep_1.png");
	deep_2 = loadTexture("sys/deep_2.png");

	
	glUniform1i(glGetUniformLocation(programs, "deep_0"), 0);
	glUniform1i(glGetUniformLocation(programs, "deep_1"), 1);
	glUniform1i(glGetUniformLocation(programs, "deep_2"), 2);

	mariocontrollerID = glGetUniformLocation(programs, "mario_controller");
	timeID = glGetUniformLocation(programs, "time");
	deepxID = glGetUniformLocation(programs, "deepx");
	deepyID = glGetUniformLocation(programs, "deepy");
	isLeftID = glGetUniformLocation(programs, "isLeft");
	deepImageID = glGetUniformLocation(programs, "deepImage");

}

void display2() {


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);





	

	glUniform1i(mariocontrollerID, deepController);
	glUniform1f(timeID, deepTime);
	glUniformMatrix4fv(offsetID, 1, false, &offset[0][0]);
	glUniform1i(deepxID, deepx);
	glUniform1i(deepyID, deepy);
	glUniform1i(isLeftID, isLeft);
	glUniform1i(deepImageID, deepImage);
	// bind textures on corresponding texture units

	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, deep_0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deep_1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, deep_2);
	// render container
	glUseProgram(programs);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}
/*
#define DOR(angle) (angle*3.1415/180);
void display(){

	
	glUniform1i(pNoID, pNo);
	glUniform1f(lenID, sin(glutGet(GLUT_ELAPSED_TIME)*1.0f)/100.0f);
	glUniform1f(frametimeID, glutGet(GLUT_ELAPSED_TIME));

	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Debug 


	glBindVertexArray(VAO);//VAO已經在init()做宣告
	glUseProgram(program);//uniform參數數值前必須先use shader
	float eyey = DOR(eyeAngley);//eyeAngley為鋼彈為圓心，視角向左右環繞的角度值
	camera = vec3(eyedistance*sin(eyey), 5, eyedistance*cos(eyey));//在這裡直接更新相機的位置
	View       = lookAt(
		               vec3(eyedistance*sin(eyey),5,eyedistance*cos(eyey)) , // Camera is at (0,0,20), in World Space
		               vec3(0,5,0), // and looks at the origin
		               vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		                );
	updateModels();//將身體的每一個部位依據angles[]和angle(掌管身體左右旋轉)的角度做旋轉加上位移到適當位置，將矩陣乘積儲存在各個Models[]裡面
	//updated data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(mat4),&View);//將相機的觀看和深度(越遠越小)矩陣傳入
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(mat4),sizeof(mat4),&Projection);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glUniform3fv(ViewPosID, 1, &camera[0]); // 這裡將相機位置傳入
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	GLuint offset[3] = {0,0,0};//offset for vertices , uvs , normals
	for(int i = 0;i < PARTSNUM ;i++){//傳入矩陣，傳入VBO資料，傳入顏色(ambient, diffuse, specular)，畫出鋼彈
		glUniformMatrix4fv(ModelID,1,GL_FALSE,&Models[i][0][0]);//將在updateModels();製作好的Models[]矩陣依序傳入

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
							  3,				//vec3
							  GL_FLOAT,			//type
							  GL_FALSE,			//not normalized
							  0,				//strip
							  (void*)offset[0]);//buffer offset
		//(location,vec3,type,固定點,連續點的偏移量,buffer point)教程式如何使用VBO裡的資料，如何分段，並將他傳進shader裡面做運算(以layout(location = 0)，序號0做辨識要傳入哪一個變數)
		offset[0] +=  vertices_size[i]*sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1, 
							  2, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[1]);
		//(location,vec2,type,固定點,連續點的偏移量,point)
		offset[1] +=  uvs_size[i]*sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[2]);
		//(location,vec3,type,固定點,連續點的偏移量,point)
		offset[2] +=  normals_size[i]*sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1,1,1);//because .mtl excluding specular , so give it here.
		for(int j = 0;j <mtls[i].size() ;j++){//
			mtlname = mtls[i][j];	
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID,1,&KDs[mtlname][0]);
			glUniform3fv(M_KsID,1,&Ks[0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset  , faces[i][j+1]*3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j+1]*3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	
		
	}//end for loop for updating and drawing model

	
	 //try placing in the front
	glUniform1i(modeID, mode); // 將數值傳遞進入shader
	glUniform1f(timeID, glutGet(GLUT_ELAPSED_TIME)); // 將時間傳入

	eyey = DOR(eyeAngley);
	View = lookAt(
		vec3(eyedistance*sin(eyey), 2, eyedistance*cos(eyey)), // Camera is at (0,0,20), in World Space
		vec3(0, 0, 0), // and looks at the origin
		vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(programs);
	View = glm::mat4(glm::mat3(View)); // remove translation from the view matrix
	glUniformMatrix4fv(ViewskyID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ProjectionskyID, 1, GL_FALSE, &Projection[0][0]);
	//skyboxShader.setMat4("view", ViewskyID);
	//skyboxShader.setMat4("projection", ProjectionskyID);
	// skybox cube

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default



	
	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//screenShader.use();
	glUseProgram(programf);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);



	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}

void Obj2Buffer(){


	//vector為陣列的一種，可以存放各種格式(vec3, string)的資料
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture; // 用不到
	loadMTL("Obj/ironmantest.mtl",Kds,Kas,Kss,Materials,texture);//解開mtl，將材質的屬性(顏色與對光的反應)讀入(ambient, diffuse, specular為存放向量的陣列)
	//printf("%d\n",texture);
	for(int i = 0;i<Materials.size();i++){
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
	}

	//load2Buffer("Obj/ironmantest.obj", 0);

	load2Buffer("Obj/ibody.obj", 0);

	load2Buffer("Obj/ileftu8.obj", 1);//上手臂
	load2Buffer("Obj/ileftd2.obj",2);//下手臂
	load2Buffer("Obj/ilefthand2.obj",3);//手
	load2Buffer("Obj/ileftshoulder2.obj",4);//肩膀
	
	load2Buffer("Obj/ihead.obj",5);

	load2Buffer("Obj/irightu3.obj",6);
	load2Buffer("Obj/irightd2.obj",7);
	load2Buffer("Obj/irighthand2.obj",8);
	load2Buffer("Obj/irightshoulder2.obj",9);


	load2Buffer("Obj/ibodyd.obj",11);
	//load2Buffer("Obj/back2.obj",10);

	load2Buffer("Obj/ileftlegu2.obj",12);
	load2Buffer("Obj/ileftlegd2.obj",13);
	load2Buffer("Obj/ileftfoot2.obj",14);

	load2Buffer("Obj/irightlegu2.obj",15);	
	load2Buffer("Obj/irightlegd2.obj",16);	
	load2Buffer("Obj/irightfoot2.obj",17);
	//上面load2Buffer中, 陣列VBOs, uVBOs, nVBOs 被填入資料, 從0-17

	GLuint totalSize[3] = {0,0,0};
	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){//為了創立三個VBO而計算的大小(glBufferData要大小值)
		totalSize[0] += vertices_size[i]*sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//generate vbo
	glGenBuffers(1,&VBO);//頂點座標緩衝器
	glGenBuffers(1,&uVBO);//材質座標的頂點緩衝器
	glGenBuffers(1,&nVBO);//法向量的頂點緩衝器
	//bind vbo ,第一次bind也同等於 create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[0],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[1],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[2],NULL,GL_STATIC_DRAW);
	
	
	for(int i = 0;i < PARTSNUM ;i++){//這個for迴圈主要在把分散在VBOs, uVBOs, nVBOs的資料合一
		glBindBuffer(GL_COPY_WRITE_BUFFER,VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[0],vertices_size[i]*sizeof(vec3));//把源頭資料複製到指定地(源頭, 指定地, 源頭偏移值, 指定地偏移值)
		offset[0] += vertices_size[i]*sizeof(vec3);//偏移值更新
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[1],uvs_size[i]*sizeof(vec2));
		offset[1] += uvs_size[i]*sizeof(vec2);//offset(偏移值)是大小三的陣列(頂點, 材質, 法向量)
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[2],normals_size[i]*sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);
	}
	glBindBuffer(GL_COPY_WRITE_BUFFER,0);


}

void updateModels(){
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	for(int i = 0 ; i < PARTSNUM;i++){
		Models[i] = mat4(1.0f);
		Rotatation[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f); 
	}
	float r,pitch,yaw,roll;
	float alpha, beta ,gamma; // alpha為前後擺動(x軸),beta為以y軸旋轉,gamma為水平擺動(z軸)

	//Body
	beta = angle; // angle 改變的是雙腳以上身體的角度(左右轉動)
	Rotatation[0] = rotate(beta,0,1,0);// 以y軸為旋轉軸轉動beta度(beta在rotate函式中會轉為弧度)
	Translation[0] = translate(0,2.9+position,0);
	Models[0] = Translation[0]*Rotatation[0];//Models[0]儲存著身體的世界座標矩陣乘積

	//左手=======================================================
	//左上手臂
	yaw = DOR(beta);r = 3.7;
	alpha = angles[1]; // angles[1]為肩膀與身體間的關節
	gamma = zangles[1]; // 可以改變gamma值讓鋼彈的手臂向左右平舉， gamma = 50 , 鋼彈變成T字形
	beta = yangles[1]; //轉動手臂
	Rotatation[1] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1)*rotate(beta,0,1,0);//向前旋轉*向右旋轉
	//Translation[1] = translate(2.7, 5.8, -0.25);//ileftu7
	Translation[1] = translate(2.7, 6.4, -0.25);
	//Translation[1] = translate(0, 0, 0);

	Models[1] = Models[0]*Translation[1]*Rotatation[1];//除了手臂本身的世界座標矩陣，由於身體轉動手臂會跟著，所以身體矩陣也要乘進去
	
	//左肩膀
	Rotatation[4] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);//向前旋轉*向右旋轉
	//Translation[4] =translate(3.7,1,-0.5);//位移到左上手臂處
	Translation[4] = translate(2.7, 6.3, 0);
	//Models[4] =Models[0]*Translation[1]*Rotatation[1];//左肩膀矩陣和左上手臂矩陣相同
	Models[4] = Models[0] * Translation[4] * Rotatation[4];

	


	//左下手臂
	pitch = DOR(alpha);r = 3;//pitch掌管手臂的前後擺動角度
	roll = DOR(gamma);//roll掌管手臂的平舉角度
	static int i=0;
	i+=5;
	alpha = angles[2]-20;//左下手臂向前彎20度
	//上手臂+下手臂向前旋轉*向右旋轉
	Rotatation[2] = rotate(alpha,1,0,0);
	//延x軸位移以上手臂為半徑的圓周長:translate(0,r*cos,r*sin)
	//延z軸位移以上手臂為半徑角度:translate(r*sin,-rcos,0)
	//Translation[2] = translate(0,-3,0);
	Translation[2] = translate(0.3, -2.4, 0);

	Models[2] = Models[1]*Translation[2]*Rotatation[2];//除的自己本身的移動，也要加乘上手臂(上前舉時，下也跟著向前)
	
	//左手手掌
	pitch = DOR(alpha);
	//b = DOR(angles[2]);
	roll = DOR(gamma);
	//手掌角度與下手臂相同
	//Rotatation[3] = Rotatation[2];
	//延x軸位移以上手臂為半徑的圓周長:translate(0,r*cos,r*sin) ,角度為上手臂+下手臂
	Translation[3] = translate(0.75,-2,0.5);
	//Translation[3] = translate(0, 0, 0);
	Models[3] = Models[2]*Translation[3]*Rotatation[3];//因為已先乘上(繼承)下手臂的矩陣(擁有下手臂的角度)，所以不用更改rotation矩陣，若想讓手旋轉，可以更改Rotation[3]
	//============================================================
	//頭==========================================================
	alpha = angles[5] + 0;
	Translation[5] = translate(0,0.0,0.0);// 改變 : initial z = -0.5
	Rotatation[5] = rotate(alpha, 0, 1, 0);
	Models[5] = Models[0]*Translation[5]*Rotatation[5];
	//============================================================
	//右手=========================================================
	//右上手臂
	//gamma = zangles[6] = -zangles[1]; 
	//alpha = angles[6] = -angles[1]; 
	//beta = yangles[6] = -yangles[1];
	gamma = zangles[6];
	alpha = angles[6]; 
	beta = yangles[6];
	Rotatation[6] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1)*rotate(beta,0,1,0);
	//Translation[6] = translate(-2.9, 5.8, -0.25); // irightu2
	Translation[6] = translate(-2.9, 6.4, -0.25);
	Models[6] = Models[0]*Translation[6]*Rotatation[6];

	//右肩膀
	Rotatation[9] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);
	Translation[9] = translate(-2.9, 6.3, 0);
	Models[9] = Models[0]*Translation[9]*Rotatation[9];

	//右下手臂
	angles[7] = angles[2];
	pitch = DOR(alpha);r = -3;
	roll = DOR(gamma);
	alpha = angles[7]-20;
	Rotatation[7] = rotate(alpha,1,0,0);
	//Translation[7] = translate(0,-3,0);
	Translation[7] = translate(-0.3, -2.4, 0);
	Models[7] = Models[6]*Translation[7]*Rotatation[7];

	//右手掌
	pitch = DOR(alpha);
	//b = DOR(angles[7]);
	roll = DOR(gamma);
	//Translation[8] =translate(0,-6,0);
	Translation[8] = translate(-0.75, -2.5, 1.5);
	Models[8] = Models[7]*Translation[8]*Rotatation[8];
	//=============================================================
	//back&DBody===================================================
	//背包
	Translation[10] =translate(0,2,-4.5);
	Models[10] = Models[0]*Translation[10]*Rotatation[10];

	//骨盆
	Translation[11] =translate(0,0,0); // -5.3
	Models[11] = Models[0]*Translation[11]*Rotatation[11];
	//=============================================================
	//左腳
	//左大腿
	alpha = angles[12]; gamma = zangles[12]; beta = yangles[12];
	Rotatation[12] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1)*rotate(beta,0,1,0);
	//Translation[12] =translate(1.8,-4.5,0);
	//Translation[12] = translate(1.25, 3.7, 0.4); // 做動作會和骨盆脫節
	Translation[12] = translate(1.25, 0.8, 0.4);
	Models[12] = Models[11]*Translation[12]*Rotatation[12];

	//左小腿
	pitch = DOR(alpha);r = -7;
	roll = DOR(gamma);
	alpha = angles[13] + 0;
	//alpha = angles[13]+angles[12];//將繼承的東西融進translate and rotate之中
	//Translation[13] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[12];
	//Rotatation[13] = rotate(alpha,1,0,0) * rotate(gamma,0,0,1);
	//Models[13] = Translation[13]*Rotatation[13]*Models[13];
	Translation[13] = translate(1, -5, -0.36);//self
	//Translation[13] = translate(0, 0, 0);
	Rotatation[13] = rotate(alpha, 1, 0, 0);
	Models[13] = Models[12] * Translation[13] * Rotatation[13];



	//左腳掌
	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[13]);
	roll = DOR(gamma);
	alpha = angles[14] + 0;
	//Translation[14] = translate(-(r+2)*sin(roll),r*cos(pitch),r*sin(pitch)-1)*Translation[13];
	//Rotatation[14] = rotate(alpha,1,0,0) * rotate(gamma, 0, 0, 1);
	//Models[14] = Translation[14]*Rotatation[14]*Models[14];
	//Translation[14] = translate(0, -4.5, 0);//self
	Translation[14] = translate(1, -5.25, 0);
	Rotatation[14] = rotate(alpha, 1, 0, 0);
	Models[14] = Models[13] * Translation[14] * Rotatation[14];

	//=============================================================
	//右腳
	//alpha = angles[15] = -angles[12];
	//gamma = zangles[15] = -zangles[12];
	alpha = angles[15];
	gamma = zangles[15];
	beta = yangles[15];
	Rotatation[15] = rotate(alpha ,1,0,0)*rotate(gamma ,0,0,1)*rotate(beta,0,1,0);
	//Translation[15] =translate(-1.8,-4.5,0);
	//Translation[15] = translate(-1.5, 3.7, 0.4);
	Translation[15] = translate(-1.5, 0.8, 0.4);
	Models[15] = Models[11]*Translation[15]*Rotatation[15];

	//angles[16] = angles[13];
	pitch = DOR(alpha);r = -7;
	roll = DOR(gamma);
	alpha = angles[16] + 0;
	//alpha = angles[16]+angles[15];
	//Rotatation[16] = rotate(alpha,1,0,0) * rotate(gamma, 0, 0, 1);
	//Translation[16] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[15];
	//Models[16] = Translation[16]*Rotatation[16]*Models[16];
	//Translation[16] = translate(0, -7.5, 0);
	Translation[16] = translate(-1, -5, -0.36);
	Rotatation[16] = rotate(alpha, 1, 0, 0);
	Models[16] = Models[15] * Translation[16] * Rotatation[16];



	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[16]);
	roll = DOR(gamma);
	alpha = angles[17] + 0;
	//alpha = angles[15]+angles[16];
	//Translation[17] = translate(-(r+2)*sin(roll),r*cos(pitch),r*sin(pitch)-0.5)*Translation[16];
	//Rotatation[17] = rotate(alpha,1,0,0) * rotate(gamma, 0, 0, 1);
	//Models[17] = Translation[17]*Rotatation[17]*Models[17];
	//Translation[17] = translate(0, -4.5, 0);//self
	Translation[17] = translate(-0.9, -5.25, 0);
	Rotatation[17] = rotate(alpha, 1, 0, 0);
	Models[17] = Models[16] * Translation[17] * Rotatation[17];
	//=============================================================
}

void load2Buffer(char* obj,int i){
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	bool res = loadOBJ(obj, vertices, uvs, normals,faces[i],mtls[i]);//mtls(materials)共17個位子，每個位子存讀入obj的各種材質名稱
	if(!res) printf("load failed\n");

	//glUseProgram(program);

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data起始位置,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;
	
	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1,&uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1,&nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}
mat4 translate(float x,float y,float z){
	vec4 t = vec4(x,y,z,1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1,0,0,0);
	vec4 c2 = vec4(0,1,0,0);
	vec4 c3 = vec4(0,0,1,0);
	mat4 M = mat4(c1,c2,c3,t);
	return M;
} 
mat4 scale(float x,float y,float z){
	vec4 c1 = vec4(x,0,0,0);
	vec4 c2 = vec4(0,y,0,0);
	vec4 c3 = vec4(0,0,z,0);
	vec4 c4 = vec4(0,0,0,1);
	mat4 M = mat4(c1,c2,c3,c4);
	return M;
}

mat4 rotate(float angle,float x,float y,float z){
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r)+(1-cos(r))*x*x,(1-cos(r))*y*x+sin(r)*z,(1-cos(r))*z*x-sin(r)*y,0);
	vec4 c2 = vec4((1-cos(r))*y*x-sin(r)*z,cos(r)+(1-cos(r))*y*y,(1-cos(r))*z*y+sin(r)*x,0);
	vec4 c3 = vec4((1-cos(r))*z*x+sin(r)*y,(1-cos(r))*z*y-sin(r)*x,cos(r)+(1-cos(r))*z*z,0);
	vec4 c4 = vec4(0,0,0,1);
	M = mat4(c1,c2,c3,c4);
	return M;
}
void Keyboard(unsigned char key, int x, int y){
	switch(key){
	case '1':
		angle += 5;//angle控制骨盆與雙腳間的關節(左右轉動)
		if(angle>=360) angle = 0;
		printf("beta:%f\n",angle);
		break;
	case '2':
		angle -= 5;
		if(angle<=0) angle = 360;
		printf("beta:%f\n",angle);
		break;
	case 'w':
		eyedistance -= 0.2;//eyedistance是相機和鋼彈的距離
		break;
	case 's':
		eyedistance += 0.2;
		break;
	case 'a':
		eyeAngley -=10;// eyeAngley 是 以鋼彈為圓心，相機左右移動時的角度
		break;
	case 'd':
		eyeAngley +=10;
		break;
	case 'r':
		angles[1] -= 5; // angles[1]控制左肩膀與身體之間的關節角度(前後擺動)
		angles[6] -= 5; // 右肩
		if(angles[1] == -360) angles[1] = 0;
		movey = 0;
		movex = 0;
		break;
	case 't':
		angles[2] -= 5;// angles[2]控制手肘的關節(前後擺動)
		if(angles[2] == -360) angles[2] = 0;
		movey = 0;
		movex = 0;
		break;
	case 'q':
		break;
	case 'e':
		break;
	}
	glutPostRedisplay();
}
void menuEvents(int option){}
void ActionMenuEvents(int option){

	if (isMotionSwitch != option) {
		isMotionSwitch = -1;
	}

	switch(option){
	
	case 0:
		action = IDLE;
		break;
	case 1:
		
		action = WALK;
		break;
	case 2:
		
		action = DANCE;
		break;
	case 3:
		
		action = GANGNAM;
		break;
	}
}
void ModeMenuEvents(int option){
	switch(option){
	case 0:
		mode = 0;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 會讓cubemap消失
		break;
	case 1:
		mode = 1;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 2:
		mode = 2;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 3:
		mode = 3;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 4:
		mode = 4;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 5:
		mode = 5;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 6:
		mode = 6;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}
void ShaderMenuEvents(int option){
	//pNo = option;
	//new added
	switch (option) {
	case 0:
		pNo = 0;
		break;
	case 1:
		pNo = 1;
		break;
	case 2:
		pNo = 2;
		break;
	case 3:
		pNo = 3;
		break;
	case 4:
		pNo = 4;
		break;
	case 5:
		pNo = 5;
		break;
	case 6:
		pNo = 6;
		break;
	case 7:
		pNo = 7;
		break;
	case 8:
		pNo = 8;
		break;
	case 9:
		pNo = 9;
		break;
	case 10:
		pNo = 10;
		break;
	case 11:
		pNo = 11;
		break;
	case 12:
		pNo = 12;
		break;
	case 13:
		pNo = 13;
		break;
	case 14:
		pNo = 14;
		break;
	}
	
}
*/
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	stbi_set_flip_vertically_on_load(true); // 把照片轉正

	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		std::cout << "Texture load complete at path: " << path << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}