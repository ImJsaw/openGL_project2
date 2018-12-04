#include "main.h"

//vec3 camera = vec3(0,0,20);
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPosForLight(0.0f, 0.0f, 0.0f);

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

	glutTimerFunc(firen_interval, Firen_Timer, 0);
	glutTimerFunc(jumpFiren_interval, JumpFiren_Timer, 0);
	
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboardup);

	

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
	glutAddMenuEntry("left-down", 0);// 順時針
	glutAddMenuEntry("left-up", 1);
	glutAddMenuEntry("right-up", 2);
	glutAddMenuEntry("right-down", 3);

	ParticleLifeMenu = glutCreateMenu(ParticleLifeMenuEvents);//建立右鍵菜單
	glutAddMenuEntry("1", 0);
	glutAddMenuEntry("0.1", 1);
	glutAddMenuEntry("3", 2);
	glutAddMenuEntry("5", 3);


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
	
	
	lightPos[0] = sin(deepTime);//讓燈源左右移動
	lightPos[2] = cos(deepTime);//讓燈源前後移動
	lightPosForLight[0] = cos(deepTime);//讓燈源左右移動
	lightPosForLight[2] = sin(deepTime);//讓燈源前後移動
	//---------------------------------------------
	//腳色的連續圖(特殊動作)
	//---------------------------------------------
	if (deepImage == 2) { // 火焰攻擊

		// 移動圖片deep
		if (isLeftDeep == 0) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			offsetDeepSkill = translate(deep_interval * 0.001f, 0, 0) * offsetDeepSkill;
			if (deepPosX < 1) deepPosX += deep_interval * 0.0005f;
		}
		else if (isLeftDeep == 1) {
			offsetDeepSkill = translate(-deep_interval * 0.001f, 0, 0) * offsetDeepSkill;
			 if (deepPosX > -1 ) deepPosX -= deep_interval * 0.0005f;
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
		if (isLeftDeep == 0) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX < 1) deepPosX += deep_interval * 0.0005f;
		}
		else if (isLeftDeep == 1) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX > -1) deepPosX -= deep_interval * 0.0005f;
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
		if (isLeftDeep == 0) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if(deepPosX < 1)if (deepPosX > -1) deepPosX += deep_interval * 0.0005f;
		}
		else if (isLeftDeep == 1) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX > -1) deepPosX -= deep_interval * 0.0005f;
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
	else if (deepImage == 0 && deepy == 4 && isDeepDie == 0) { // 扣血
		if (isLeftDeep == 0 && deltatime + 0.2 >= deepTime) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX > -1) deepPosX -= deep_interval * 0.0005f;
		}
		else if (isLeftDeep == 1 && deltatime + 0.2 >= deepTime) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX < 1) deepPosX += deep_interval * 0.0005f;
		}

		if (deepx == 6) {
			if (offsetDeepBloodLength <= 0.0) { // 已死亡，跳出這個被打到的動畫
				isDeepDie = 1;
			}
			else if(offsetDeepBloodLength >= 0.0 && deltatime + 1.2 <= deepTime){ // 還沒死，但要躺在地上一下
				deepImage = 0; // 換回圖片0
				deepx = 1; // 回到靜止圖
				deepy = 1;
				
			}
		}
		else if (deepx != 6) { // deepx 往右走
			deepx++;
		}
	}
	//---------------------------------------------
	//技能特效連續圖
	//---------------------------------------------
	if (drawSkillDeep == 1 && deepSkillImage == 0) { // 冰火劍氣播放連續圖
		
		if (isLeftDeep == 0) {
			
			offsetDeepSkill = translate((currentTime-deltatime)*10 * deep_interval * 0.0001f, 0, 0) * offsetDeepSkill;
		}
		else if (isLeftDeep == 1) {
			
			offsetDeepSkill = translate((currentTime - deltatime) * 10 * -deep_interval * 0.0001f, 0, 0) * offsetDeepSkill;
		}

		// skill 連續圖動畫
		if (deepSkillx == 4 && deepSkilly == 2) {
			deepSkillx = 1; // 回到靜止圖
			deepSkilly = 1;
			drawSkillDeep = 0;
		}
		else if ((deepSkilly == 1 && deepSkillx != 4) || (deepSkilly == 2 && deepSkillx != 4)) {
			deepSkillx++;
		}
		else if (deepSkilly == 1 && deepSkillx == 4) {
			deepSkilly++;
			deepSkillx = 1;
		}

	}
	else if (drawSkillDeep == 1 && deepSkillImage == 1) { // 紅龍播放連續圖
		
		if (isLeftDeep == 0) {

			offsetDeepSkill = translate(deep_interval * 0.002f, 0, 0) * offsetDeepSkill;
		}
		else if (isLeftDeep == 1) {

			offsetDeepSkill = translate(-deep_interval * 0.002f, 0, 0) * offsetDeepSkill;
		}

		// skill 連續圖動畫
		if (deepSkillx == 2 && deepSkilly == 5) {
			deepSkillx = 1; // 回到靜止圖
			deepSkilly = 1;
			drawSkillDeep = 0;
		}
		else if (deepSkillx != 2) { // x : 1 -> 2
			deepSkillx++;
		}
		else if (deepSkillx == 2) { // y : 1 -> 2 ； x : 1 
			deepSkilly++;
			deepSkillx = 1;
		}
	}
	else if (drawSkillDeep == 1 && deepSkillImage == 2) { // 朱利安柱子連續圖
		if (isLeftDeep) offsetDeepSkill = translate(-0.2f, 0, 0) * offsetDeep;
		else offsetDeepSkill = translate(-0.2f, 0, 0) * offsetDeep;

		// skill 連續圖動畫
		if (deepSkillx == 4 && deepSkilly == 2) {
			deepSkillx = 1; // 回到靜止圖
			deepSkilly = 1;
			drawSkillDeep = 0;
		}
		else if (deepSkillx != 4) { // x : 1 -> 2
			deepSkillx++;
		}
		else if (deepSkillx == 4) { // y : 1 -> 2 ； x : 1 
			deepSkilly++;
			deepSkillx = 1;
		}
	}

	//---------------------------------------------
	//血條動畫
	//---------------------------------------------
	offsetDeepBlood = translate(0, 0.16f, 0) * offsetDeep; // 讓血條一直待在deep上方
	
}
void Jump_Timer(int val) {

	glutPostRedisplay();
	glutTimerFunc(jump_interval, Jump_Timer, val);
	currentTime += jump_interval * 0.001f; 

	//----------------------------------------
	// 跳起的位移和動畫連續圖
	//----------------------------------------
	// jump_interval 為 30 時，最接近正常每秒的速度
	if (deepController == 3 || deepController == 7) { // 跳起到著地的時間
		if ((currentTime - deltatime) >= time_for_a_jump) { // 要著地了，看向哪一方
			deltatime = 0.0f;
			is_move_when_jump_deep = 0;
			if (deepController == 3) {
				deepController = 0; // 著地後，看向右方
			}
			else if (deepController == 7) {
				deepController = 4; // 著地後，看向左方
			}

			deepx = 1;
			deepy = 1;
			deepImage = 0;
		}
		else if ((currentTime - deltatime) < time_for_a_jump) { 
			float radian = DOR((currentTime - deltatime) * (180 / (time_for_a_jump+0.017f)));
			// currentTime - deltatime : 0 到 time_for_a_jump(1.2)
			// 180/(time_for_a_jump+0.015) : 讓currentTime - deltatime從0到180(度)，0.015是誤差，計時器和正常時間有偏差
			if (is_move_when_jump_deep == 1) { // 邊跳邊向右
				//offset = translate(jump_interval * 0.0001f, 0, 0) * offset;
				deepPosX += deep_interval * 0.0005f;
			}
			else if (is_move_when_jump_deep == 2) { // 邊跳邊向左
				//offset = translate(-jump_interval * 0.0001f, 0, 0) * offset;
				deepPosX -= deep_interval * 0.0005f;
			}
			//offset = translate(0, cos( radian )*0.07f, 0) * offset; // 跳躍的矩陣
			deepPosY += cos(radian)*0.07f;

			if (deepx != 4) {
				deepx++;
			}
		}
	}
	else if(deepDirection != -1){//normal move
		//offset = translate(jump_interval * 0.001f* xMove, jump_interval * 0.001f* yMove, 0) * offset;
		if(deepPosX < 1 && deepPosX > -1) deepPosX += jump_interval * 0.001f* xMoveDeep;
		if (deepPosY < 0 && deepPosY > -1) deepPosY += jump_interval * 0.001f* yMoveDeep;
		deepx++;
		if (deepPosY > 0 && deepController != 7) deepPosY = -0.01f;
		if (deepx == 8) deepx = 3;
	}

	//------------------------------------
	//update pos and set the boundary
	//------------------------------------
	if (deepPosX > 1) deepPosX = 0.99f;
	if (deepPosX < -1) deepPosX = -0.99f;
	if (deepPosY < -0.89) deepPosY = -0.89f;
	offsetDeep = translate(deepPosX, deepPosY, 0);
	cout << "deep Position : " << deepPosX << " , " << deepPosY << endl;

	//------------------------------
	//particle zone
	//-----------------------------
	// Add new particles
	for (GLuint i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		vec4 deepPos = offsetDeep * deepPosition; // deep的位置
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
			p.Position -= p.Velocity;
			p.Color.a -= dt * 2.5f;
			
			p.Color.a -= dt * 2.5f;
		}
	}

	//------------------------------
	//particle rain zone
	//-----------------------------
	// Add new particles
	for (GLuint i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticleRain(); // 找到剛死亡的雨
		RespawnParticleRain(particleRain[unusedParticle]); // 初始化，讓他起死回生
	}
	// Uupdate all particles
	for (GLuint i = 0; i < particleRainNum; ++i)
	{
		Particle &p = particleRain[i];
		// jump_interval * 0.001 是 每一禎的時間差
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f) {	// particle is alive, thus update
			//p.Position += p.Velocity * dt;
			p.Position += p.Velocity;
			p.Color.a -= dt * 1.5f;
		}
	}

}

//  deepController : 跳7,走65,站4:左   右:0站,12走,3跳
// deepDirection : 0左 1上 2右 3下
void Keyboard(unsigned char key, int x, int y) { // 各種按鈕按下去的反應
	if (isDeepDie == 0 || isFirenDie == 0) { // 如果還沒死，按了按鍵才有用
		switch (key) {
		case 'q': // 扣血
		case 'Q':
			deepImage = 0;
			deepy = 4;
			if (offsetDeepBloodLength > 0) offsetDeepBloodLength -= 0.1f;
			deltatime = deepTime; // 計時開始，到1.2秒後deep才能再站起來
			break;
		case 'e': // 朱利安柱
		case 'E':
			deepImage = 1;
			deepy = 2;
			deepSkillImage = 2; // 朱利安柱子圖片
			if (isLeftDeep) offsetDeepSkill = offsetDeep;
			else offsetDeepSkill = offsetDeep;
			deltatime = currentTime; // 讓柱子有向外飛的感覺
			drawSkillDeep = 1;
			break;
		case 'z': // 一般攻擊狀態，飛龍特效啟動
		case 'Z':
			deepImage = 1;
			deepy = 4;
			deepSkillImage = 1; // 劍氣圖片
			if (isLeftDeep) offsetDeepSkill = translate(-0.4f, 0, 0) * offsetDeep;
			else offsetDeepSkill = translate(0.4f, 0, 0) * offsetDeep;
			drawSkillDeep = 1;

			break;
		case 'x': // 火焰攻擊狀態，劍氣特效啟動
		case 'X':
			deepImage = 2;
			deepSkillImage = 0;
			if (isLeftDeep) offsetDeepSkill = translate(-0.1f, 0, 0) * offsetDeep;
			else offsetDeepSkill = translate(0.1f, 0, 0) * offsetDeep;
			drawSkillDeep = 1;

			deltatime = currentTime;

			break;
		case 'c': // 跳躍
		case 'C':
			if (deepController == 1 || deepController == 0) { // 上一個時刻面向右邊

				if (deepController == 1) {
					is_move_when_jump_deep = 1;
				}
				deltatime = currentTime;
				deepController = 3; // 換成向右跳躍的圖
			}
			else if (deepController == 5 || deepController == 4) { // 上一個時刻面向左邊

				if (deepController == 5) {
					is_move_when_jump_deep = 2;
				}
				deltatime = currentTime;
				deepController = 7; // 換成向左跳躍的圖
			}
			deepx = 1;
			deepy = 7;
			deepImage = 0;

			break;
		case 'W': // 往上走
		case 'w':
			deepDirection = 1;
			yMoveDeep++;
			break;
		case 'S': // 往下走
		case 's':
			deepDirection = 3;
			yMoveDeep--;
			break;
		case 'a':
		case 'A': // 往左走
			xMoveDeep--;
			if (deepController == 7) { // 上一個時刻向左跳
				deepController = 7; // 仍然向左
				is_move_when_jump_deep = 2; // 邊跳邊向左移動
			}
			else if (deepController == 3) { // 上一個時刻向右跳
				deepController = 7; // 改成向左
				is_move_when_jump_deep = 2; // 邊跳邊向左移動
			}
			else {
				deepController = 5; // 向左走
				isLeftDeep = 1;
			}
			deepDirection = 0;
			break;
		case 'd':
		case 'D':
			xMoveDeep++;
			if (deepController == 7) { // 上一個時刻向左跳
				deepController = 3; // 改成向右
				is_move_when_jump_deep = 1; // 邊跳邊向右移動
			}
			else if (deepController == 3) { // 上一個時刻向右跳
				deepController = 3; // 仍然向右
				is_move_when_jump_deep = 1; // 邊跳邊向右移動
			}
			else {
				deepController = 1; // 向右走
				isLeftDeep = 0;
			}
			deepDirection = 2;
			break;
		case 'u': // 扣血
		case 'U':
			firenImage = 0;
			fireny = 4;
			if (offsetFirenBloodLength > 0) offsetFirenBloodLength -= 0.1f;
			deltatimeFiren = firenTime; // 計時開始，到1.2秒後deep才能再站起來
			break;
		case 'o': // 朱利安柱
		case 'O':
			firenImage = 2;
			fireny = 4;
			firenx = 6;
			firenSkillImage = 2; // 朱利安柱子圖片
			if (isLeftFiren) offsetFirenSkill = offsetFiren;
			else offsetFirenSkill = offsetFiren;
			deltatimeFiren = currentTimeFiren; // 讓柱子有向外飛的感覺
			drawSkillFiren = 1;
			break;
		case 'm': // 一般攻擊狀態，飛龍特效啟動
		case 'M':
			firenImage = 2;
			fireny = 4;
			firenx = 1;
			firenSkillImage = 1; // 劍氣圖片
			if (isLeftFiren) offsetFirenSkill = translate(-0.4f, 0, 0) * offsetFiren;
			else offsetFirenSkill = translate(0.4f, 0, 0) * offsetFiren;
			drawSkillFiren = 1;
			deltatimeFiren = firenTime;
			break;
		case ',': // 火焰攻擊狀態，吐火焰特效
		case '<':
			firenImage = 2;
			firenSkillImage = 0;
			if (isLeftFiren) offsetFirenSkill = translate(-0.1f, 0, 0) * offsetFiren;
			else offsetFirenSkill = translate(0.1f, 0, 0) * offsetFiren;
			drawSkillFiren = 1;

			deltatimeFiren = currentTimeFiren;

			break;
		case '.': // 跳躍
		case '>':
			if (firenController == 1 || firenController == 0) { // 上一個時刻面向右邊

				if (firenController == 1) {
					is_move_when_jump_firen = 1;
				}
				deltatimeFiren = currentTimeFiren;
				firenController = 3; // 換成向右跳躍的圖
			}
			else if (firenController == 5 || firenController == 4) { // 上一個時刻面向左邊

				if (firenController == 5) {
					is_move_when_jump_firen = 2;
				}
				deltatimeFiren = currentTimeFiren;
				firenController = 7; // 換成向左跳躍的圖
			}
			firenx = 1;
			fireny = 7;
			firenImage = 0;

			break;
		case 'i': // 往上走
		case 'I':
			firenDirection = 1;
			yMoveFiren++;
			break;
		case 'k': // 往下走
		case 'K':
			firenDirection = 3;
			yMoveFiren--;
			break;
		case 'j':
		case 'J': // 往左走
			xMoveFiren--;
			if (firenController == 7) { // 上一個時刻向左跳
				firenController = 7; // 仍然向左
				is_move_when_jump_firen = 2; // 邊跳邊向左移動
			}
			else if (firenController == 3) { // 上一個時刻向右跳
				firenController = 7; // 改成向左
				is_move_when_jump_firen = 2; // 邊跳邊向左移動
			}
			else {
				firenController = 5; // 向左走
				isLeftFiren = 1;
			}
			firenDirection = 0;
			break;
		case 'l':
		case 'L':
			xMoveFiren++;
			if (firenController == 7) { // 上一個時刻向左跳
				firenController = 3; // 改成向右
				is_move_when_jump_firen = 1; // 邊跳邊向右移動
			}
			else if (firenController == 3) { // 上一個時刻向右跳
				firenController = 3; // 仍然向右
				is_move_when_jump_firen = 1; // 邊跳邊向右移動
			}
			else {
				firenController = 1; // 向右走
				isLeftFiren = 0;
			}
			firenDirection = 2;
			break;
		}
	}
	



	glutPostRedisplay();
}
void Keyboardup(unsigned char key, int x, int y) { // 一般走路按鈕放開即停止

	if (isDeepDie == 0 || isFirenDie == 0) {// 如果還沒死，放開按鍵才有用
		switch (key) {
		case 'W': // 往上走
		case 'w':
			yMoveDeep--;
			if (!xMoveDeep) deepDirection = -1;
			deepx = 3; // 回到站立
			deepy = 1;
			break;
		case 'S': // 往下走
		case 's':
			yMoveDeep++;
			if (!xMoveDeep) deepDirection = -1;
			deepx = 3; // 回到站立
			deepy = 1;
			break;
		case 'a':
		case 'A':
			xMoveDeep++;
			if (deepController == 7) { // 上一個時刻向左跳
				deepController = 7; // 仍然向左
				is_move_when_jump_deep = 0;
			}
			else if (deepController == 3) { // 上一個時刻向右跳
				deepController = 7; // 改成向左
				is_move_when_jump_deep = 0;
			}
			else {
				deepController = 4; // 向左站立
				deepx = 1; // 回到站立
				deepy = 1;
			}
			if (!yMoveDeep) deepDirection = -1;

			break;
		case 'd':
		case 'D':
			xMoveDeep--;
			if (deepController == 7) { // 上一個時刻向左跳
				deepController = 3; // 改成向右
				is_move_when_jump_deep = 0;
			}
			else if (deepController == 3) { // 上一個時刻向右跳
				deepController = 3; // 仍然向右
				is_move_when_jump_deep = 0;
			}
			else {
				deepController = 0; // 向右站立
				deepx = 1; // 回到站立
				deepy = 1;
			}
			if (!yMoveDeep) deepDirection = -1;
			break;
		case 'i': // 往上走
		case 'I':
			yMoveFiren--;
			if (!xMoveFiren) firenDirection = -1;
			firenx = 3; // 回到站立
			fireny = 1;
			break;
		case 'k': // 往下走
		case 'K':
			yMoveFiren++;
			if (!xMoveFiren) firenDirection = -1;
			firenx = 3; // 回到站立
			fireny = 1;
			break;
		case 'j':
		case 'J':
			xMoveFiren++;
			if (firenController == 7) { // 上一個時刻向左跳
				firenController = 7; // 仍然向左
				is_move_when_jump_firen = 0;
			}
			else if (firenController == 3) { // 上一個時刻向右跳
				firenController = 7; // 改成向左
				is_move_when_jump_firen = 0;
			}
			else {
				firenController = 4; // 向左站立
				firenx = 1; // 回到站立
				fireny = 1;
			}
			if (!yMoveFiren) firenDirection = -1;

			break;
		case 'l':
		case 'L':
			xMoveFiren--;
			if (firenController == 7) { // 上一個時刻向左跳
				firenController = 3; // 改成向右
				is_move_when_jump_firen = 0;
			}
			else if (firenController == 3) { // 上一個時刻向右跳
				firenController = 3; // 仍然向右
				is_move_when_jump_firen = 0;
			}
			else {
				firenController = 0; // 向右站立
				firenx = 1; // 回到站立
				fireny = 1;
			}
			if (!yMoveFiren) firenDirection = -1;
			break;
		}
	}
	glutPostRedisplay();
}


void Firen_Timer(int val) {
	glutPostRedisplay();
	glutTimerFunc(firen_interval, Firen_Timer, val);
	firenTime += firen_interval * firenSpeed * 0.001f;

	//---------------------------------------------
	//腳色的連續圖(特殊動作)
	//---------------------------------------------
	if (firenImage == 2 && (fireny == 1 || fireny == 2)) { // 火焰攻擊(從第三張圖的第一個位置)

		// 移動圖片deep
		if (isLeftFiren == 0) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			offsetFirenSkill = translate(firen_interval * 0.001f, 0, 0) * offsetFirenSkill;
			if (firenPosX < 1) firenPosX += firen_interval * 0.0005f;
		}
		else if (isLeftFiren == 1) {
			offsetFirenSkill = translate(-firen_interval * 0.001f, 0, 0) * offsetFirenSkill;
			if (firenPosX > -1) firenPosX -= firen_interval * 0.0005f;
		}

		if (firenx == 2 && fireny == 2) {
			firenImage = 0; // 換回圖片0
			firenx = 1; // 回到靜止圖
			fireny = 1;
		}
		else if ((fireny == 1 && firenx == 10)) { // 往下一排走的條件
			fireny++;
		}
		else if ((fireny == 1 && firenx != 10)) { // deepx 往右走
			firenx++;
		}
		else if ((fireny == 2 && firenx != 1)) { // deepx 往左走
			firenx--;
		}


	}
	else if (firenImage == 2 && fireny == 4 && (firenx <= 5)) { // 一般攻擊(在連續圖的第四行)
		if (isLeftFiren == 0 && (firenTime - deltatimeFiren < 0.5)) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if (firenPosX < 1) firenPosX += firen_interval * 0.0005f;
		}
		else if (isLeftFiren == 1 && (firenTime - deltatimeFiren < 0.5)) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (firenPosX > -1) firenPosX -= firen_interval * 0.0005f;
		}

		if (firenx == 5) {
			if (firenTime - deltatimeFiren > 1.5) {
				firenImage = 0; // 換回圖片0
				firenx = 1; // 回到靜止圖
				fireny = 1;
			}
		}
		else if (firenx != 5) { // deepx 往右走
			firenx++;
		}
		//printf("firenx = %d fireny = %d\n", firenx, fireny);
	}
	else if (firenImage == 2 && (fireny == 4 || fireny == 5) && (firenx >= 6)) { // 丟東西(火柱)
		if (isLeftFiren == 0) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if (firenPosX < 1)if (firenPosX > -1) firenPosX += firen_interval * 0.0005f;
		}
		else if (isLeftFiren == 1) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (firenPosX > -1) firenPosX -= firen_interval * 0.0005f;
		}

		if (firenx == 9 && fireny == 5) {
			firenImage = 0; // 換回圖片0
			firenx = 1; // 回到靜止圖
			fireny = 1;
		}
		else if (firenx != 10 && fireny == 4) { // deepx 往右走
			firenx++;
		}
		else if (firenx == 10 && fireny == 4) {
			fireny++;
		}
		else if (firenx != 1 && fireny == 5) {
			firenx--;
		}

	}
	else if (firenImage == 0 && fireny == 4 && isFirenDie == 0) { // 扣血
		if (isLeftFiren == 0 && deltatimeFiren + 0.2 >= firenTime) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (firenPosX > -1) firenPosX -= firen_interval * 0.0005f;
		}
		else if (isLeftFiren == 1 && deltatimeFiren + 0.2 >= firenTime) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if (firenPosX < 1) firenPosX += firen_interval * 0.0005f;
		}

		if (firenx == 6) {
			if (offsetFirenBloodLength <= 0.0) { // 已死亡，跳出這個被打到的動畫
				isFirenDie = 1;
			}
			else if (offsetFirenBloodLength >= 0.0 && deltatimeFiren + 1.2 <= firenTime) { // 還沒死，但要躺在地上一下
				firenImage = 0; // 換回圖片0
				firenx = 1; // 回到靜止圖
				fireny = 1;

			}
		}
		else if (firenx != 6) { // deepx 往右走
			firenx++;
		}
	}
	//---------------------------------------------
	//技能特效連續圖
	//---------------------------------------------
	if (drawSkillFiren == 1 && firenSkillImage == 0) { // 藍色火焰播放連續圖

		if (isLeftFiren == 0) {

			offsetFirenSkill = translate((currentTimeFiren - deltatimeFiren) * 10 * firen_interval * 0.0001f, 0, 0) * offsetFirenSkill;
		}
		else if (isLeftFiren == 1) {

			offsetFirenSkill = translate((currentTimeFiren - deltatimeFiren) * 10 * -firen_interval * 0.0001f, 0, 0) * offsetFirenSkill;
		}

		// skill 連續圖動畫
		if (firenSkillx == 6 && firenSkilly == 3) {
			firenSkillx = 1; // 回到靜止圖
			firenSkilly = 1;
			drawSkillFiren = 0;
		}
		else if (firenSkillx != 6) {
			firenSkillx++;
		}
		else if (firenSkillx == 6) {
			firenSkilly++;
			firenSkillx = 1;
		}

	}
	else if (drawSkillFiren == 1 && firenSkillImage == 1) { // 噴火播放連續圖

		if (isLeftFiren == 0) {

			offsetFirenSkill = translate(firen_interval * 0.002f, 0, 0) * offsetFirenSkill;
		}
		else if (isLeftFiren == 1) {

			offsetFirenSkill = translate(-firen_interval * 0.002f, 0, 0) * offsetFirenSkill;
		}

		// skill 連續圖動畫
		if (firenSkillx == 4 && firenSkilly == 4) {
			firenSkillx = 1; // 回到靜止圖
			firenSkilly = 1;
			drawSkillFiren = 0;
		}
		else if (firenSkillx != 4) { // x : 1 -> 2
			firenSkillx++;
		}
		else if (firenSkillx == 4) { // y : 1 -> 2 ； x : 1 
			firenSkilly++;
			firenSkillx = 1;
		}
	}
	else if (drawSkillFiren == 1 && firenSkillImage == 2) { // 朱利安柱子連續圖
		if (isLeftFiren) offsetFirenSkill = translate(-0.2f, 0, 0) * offsetFiren;
		else offsetFirenSkill = translate(-0.2f, 0, 0) * offsetFiren;

		// skill 連續圖動畫
		if (firenSkillx == 6) {
			firenSkillx = 1; // 回到靜止圖
			firenSkilly = 1;
			drawSkillFiren = 0;
		}
		else if (firenSkillx != 6) { // x : 1 -> 2
			firenSkillx++;
		}

	}

	//---------------------------------------------
	//血條動畫
	//---------------------------------------------
	offsetFirenBlood = translate(0, 0.16f, 0) * offsetFiren; // 讓血條一直待在deep上方

}
void JumpFiren_Timer(int val) {

	glutPostRedisplay();
	glutTimerFunc(jumpFiren_interval, JumpFiren_Timer, val);
	currentTimeFiren += jumpFiren_interval * 0.001f;

	//----------------------------------------
	// 跳起的位移和動畫連續圖
	//----------------------------------------
	// jump_interval 為 30 時，最接近正常每秒的速度
	if (firenController == 3 || firenController == 7) { // 跳起到著地的時間
		if ((currentTimeFiren - deltatimeFiren) >= time_for_a_jump_firen) { // 要著地了，看向哪一方
			deltatimeFiren = 0.0f;
			is_move_when_jump_firen = 0;
			if (firenController == 3) {
				firenController = 0; // 著地後，看向右方
			}
			else if (firenController == 7) {
				firenController = 4; // 著地後，看向左方
			}

			firenx = 1;
			fireny = 1;
			firenImage = 0;
		}
		else if ((currentTimeFiren - deltatimeFiren) < time_for_a_jump_firen) {
			float radian = DOR((currentTimeFiren - deltatimeFiren) * (180 / (time_for_a_jump_firen + 0.017f)));
			// currentTime - deltatime : 0 到 time_for_a_jump(1.2)
			// 180/(time_for_a_jump+0.015) : 讓currentTime - deltatime從0到180(度)，0.015是誤差，計時器和正常時間有偏差
			if (is_move_when_jump_firen == 1) { // 邊跳邊向右
				//offset = translate(jump_interval * 0.0001f, 0, 0) * offset;
				firenPosX += firen_interval * 0.0005f;
			}
			else if (is_move_when_jump_firen == 2) { // 邊跳邊向左
				//offset = translate(-jump_interval * 0.0001f, 0, 0) * offset;
				firenPosX -= firen_interval * 0.0005f;
			}
			//offset = translate(0, cos( radian )*0.07f, 0) * offset; // 跳躍的矩陣
			firenPosY += cos(radian)*0.07f;

			if (firenx != 4) {
				firenx++;
			}
		}
	}
	else if (firenDirection != -1) {//normal move
		//offset = translate(jump_interval * 0.001f* xMove, jump_interval * 0.001f* yMove, 0) * offset;
		if (firenPosX < 1 && firenPosX > -1) firenPosX += jumpFiren_interval * 0.001f* xMoveFiren;
		if (firenPosY < 0 && firenPosY > -1) firenPosY += jumpFiren_interval * 0.001f* yMoveFiren;
		firenx++;
		if (firenPosY > 0 && firenController != 7) firenPosY = -0.01f;
		if (firenx == 8) firenx = 3;
	}

	//------------------------------------
	//update pos and set the boundary
	//------------------------------------
	if (firenPosX > 1) firenPosX = 0.99f;
	if (firenPosX < -1) firenPosX = -0.99f;
	if (firenPosY < -0.89) firenPosY = -0.89f;
	offsetFiren = translate(firenPosX, firenPosY, 0);
	cout << "firen Position : " << firenPosX << " , " << firenPosY << endl;


}

bool isEnemyHitByDeep(float enemyPosX, float enemyPosY,float skillRange) {
	if (abs(enemyPosY - deepPosY) < 0.2) {//y in range
		if (isLeftDeep && deepPosX - enemyPosX < skillRange) return true;//x in range & left
		if (!isLeftDeep && enemyPosX - deepPosX < skillRange) return true;//x in range & right
	}
	return false;
}

bool isEnemyHitByFiren(float enemyPosX, float enemyPosY, float skillRange) {
	if (abs(enemyPosY - firenPosY) < 0.2) {//y in range
		if (isLeftFiren && firenPosX - enemyPosX < skillRange) return true;//x in range & left
		if (!isLeftFiren && enemyPosX - firenPosX < skillRange) return true;//x in range & right
	}
	return false;
}


void init() {
	
	initDeep();
	initFiren();

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
	particleLife = 1.0f;
	particleSpeed = 0.1f;

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


	//--------------------------
	//particle system(rain--by learn opengl)
	//--------------------------
	// debug，記得改shader

	rainLife = 1;
	rainSpeed = 1;
	ShaderInfo particleShaderRain2[] = {
		{ GL_VERTEX_SHADER, "particle_rain.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "particle_rain.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programParticleRain2 = LoadShaders(particleShaderRain2);//讀取shader

	glUseProgram(programParticleRain2);//uniform參數數值前必須先use shader

	glGenVertexArrays(1, &VAOpr2);
	glGenBuffers(1, &VBOpr2);
	glGenBuffers(1, &EBOpr2);
	glBindVertexArray(VAOpr2);

	glBindBuffer(GL_ARRAY_BUFFER, VBOpr2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOpr2);
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

	glUseProgram(programParticleRain2);

	particleRainImg = loadTexture("sys/star.png");

	glUniform1i(glGetUniformLocation(programParticleRain2, "sprite"), 0);

	offsetParticleRainID = glGetUniformLocation(programParticleRain2, "offset"); // particle的位置
	colorParticleRainID = glGetUniformLocation(programParticleRain2, "color");
	particleTimeRainID = glGetUniformLocation(programParticleRain2, "time");
	particleLifeRainID = glGetUniformLocation(programParticleRain2, "life");

	for (GLuint i = 0; i < particleRainNum; ++i)
		particleRain.push_back(Particle());



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

	frameColorID = glGetUniformLocation(programFrame, "color");

	//--------------------------------------------------------
	//point light zone
	//--------------------------------------------------------
	ShaderInfo shaderlight[] = {
	{ GL_VERTEX_SHADER, "light.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "light.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programLight = LoadShaders(shaderlight);//讀取shader

	glUseProgram(programLight);//uniform參數數值前必須先use shader

	lightPosID = glGetUniformLocation(programLight, "lightPos");
	lightImg = loadTexture("sys/star.png");
	glUniform1i(glGetUniformLocation(programLight, "sprite"), 0);
}

void initDeep() {
	//-----------------------
	// deep-setting
	//-----------------------
	deepPosX = -0.8f;
	deepPosY = -0.2f;

	deepController = 0; // 向右站立
	offsetDeep = translate(-0.9f, 0, 0); // 初始化矩陣，只有腳色的矩陣translate到螢幕左邊
	deepx = 1; // 貼圖座標移動矩陣
	deepy = 1;
	xMoveDeep = 0;
	yMoveDeep = 0;

	is_move_when_jump_deep = 0;

	isLeftDeep = 0;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(deepVertices) + sizeof(deepTexCoord2), deepVertices, GL_STATIC_DRAW);
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

	deepcontrollerID = glGetUniformLocation(programDeep, "mario_controller");
	timeDeepID = glGetUniformLocation(programDeep, "time");
	offsetDeepID = glGetUniformLocation(programDeep, "offset"); // 少了這行，讓offset沒有傳入，使得position*offset未知，圖跑不出來
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
	offsetDeepSkill = scale(1, 1, 1);
	deepSkillx = 1;
	deepSkilly = 1;
	deepSkillImage = 0;
	drawSkillDeep = 0;

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
	for (int x = -6; x <= 6; x += 3)
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
	// 一般的 VAO VBO EBO
	// --------------------------------------
	glGenVertexArrays(1, &VAOskill);
	glGenBuffers(1, &VBOskill);
	glGenBuffers(1, &EBOskill);

	glBindVertexArray(VAOskill);

	glBindBuffer(GL_ARRAY_BUFFER, VBOskill);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skillVertices) + sizeof(skillVertices2), skillVertices, GL_STATIC_DRAW); // A+B的空間中先綁定A
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

	for (int spriteID = 0; spriteID < objectCount; ++spriteID)
	{
		deepSkillSheets[spriteID] = new Sprite2D();
	}
	deepSkillSheets[0]->Init("sys/twinsflame.png", 2, 4, 8);
	deepSkillSheets[1]->Init("sys/fire_dragon.png", 5, 2, 8);
	deepSkillSheets[2]->Init("sys/julianColumn.png", 2, 4, 8);

	glUniform1i(glGetUniformLocation(programSkill, "twinsflame"), 0);
	glUniform1i(glGetUniformLocation(programSkill, "firedragon"), 1);
	glUniform1i(glGetUniformLocation(programSkill, "julianColumn"), 2);



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
}

void initFiren() {
	//-----------------------
	// deep-setting
	//-----------------------
	firenPosX = 0.8f;
	firenPosY = -0.2f;

	firenController = 0; // 向右站立
	offsetFiren = translate(-0.9f, 0, 0); // 初始化矩陣，只有腳色的矩陣translate到螢幕左邊
	firenx = 1; // 貼圖座標移動矩陣
	fireny = 1;
	xMoveFiren = 0;
	yMoveFiren = 0;

	is_move_when_jump_firen = 0;

	isLeftFiren = 0;
	firenDirection = -1;
	firenImage = 0;

	ShaderInfo firenShader[] = {
		{ GL_VERTEX_SHADER, "firen.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "firen.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programFiren = LoadShaders(firenShader);//讀取shader

	glUseProgram(programFiren);//uniform參數數值前必須先use shader

	glGenVertexArrays(1, &VAOf);
	glGenBuffers(1, &VBOf);
	glGenBuffers(1, &EBOf);

	glBindVertexArray(VAOf);

	glBindBuffer(GL_ARRAY_BUFFER, VBOf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firenVertices), firenVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firenIndices), firenIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute(for image1)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glUseProgram(programFiren);

	for (int spriteID = 0; spriteID < objectCountFiren; ++spriteID)
	{
		firenSheets[spriteID] = new Sprite2D();
	}
	firenSheets[0]->Init("sys/firen_0.png", 7, 10, 50);
	firenSheets[1]->Init("sys/firen_1.png", 7, 10, 50);
	firenSheets[2]->Init("sys/firen_2.png", 5, 10, 50);
	for (int spriteID = 0; spriteID < objectCountFiren; ++spriteID)
	{
		firenNormalSheets[spriteID] = new Sprite2D();
	}
	firenNormalSheets[0]->Init("sys/firen_0_normal.png", 7, 10, 50);
	firenNormalSheets[1]->Init("sys/firen_1_normal.png", 7, 10, 50);
	firenNormalSheets[2]->Init("sys/firen_2_normal.png", 5, 10, 50);

	firencontrollerID = glGetUniformLocation(programFiren, "mario_controller");
	timeFirenID = glGetUniformLocation(programFiren, "time");
	offsetFirenID = glGetUniformLocation(programFiren, "offset"); // 少了這行，讓offset沒有傳入，使得position*offset未知，圖跑不出來
	firenxID = glGetUniformLocation(programFiren, "deepx");
	firenyID = glGetUniformLocation(programFiren, "deepy");
	isLeftFirenID = glGetUniformLocation(programFiren, "isLeft");
	firenImageID = glGetUniformLocation(programFiren, "deepImage");
	projectionFirenID = glGetUniformLocation(programFiren, "projection");
	viewFirenID = glGetUniformLocation(programFiren, "view");
	viewPosFirenID = glGetUniformLocation(programFiren, "viewPos");
	lightPosFirenID = glGetUniformLocation(programFiren, "lightPos");

	//-----------------------
	// skill-setting
	//-----------------------
	offsetFirenSkill = scale(1, 1, 1);
	firenSkillx = 1;
	firenSkilly = 1;
	firenSkillImage = 0;
	drawSkillFiren = 0;

	ShaderInfo skillShader[] = {
		{ GL_VERTEX_SHADER, "firenskill.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "firenskill.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programFirenSkill = LoadShaders(skillShader);//讀取shader

	glUseProgram(programFirenSkill);//uniform參數數值前必須先use shader

	//---------------------------------
	//dragon instance initialize(五個龍的位置offset)
	//---------------------------------
	int index = 0;
	for (int y = -2; y <= 2; y++)
	{
		glm::vec2 translation;
		translation.x = abs(y * 0.1f);
		translation.y = y * 0.1f;
		fireOffset[index++] = translation;
	}
	// --------------------------------------
	// store instance data in an array buffer(一次生成五龍，該VBO放置算好的偏差值)
	// --------------------------------------
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 5, &fireOffset[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//---------------------------------
	//dragon instance initialize(五個柱子的位置offset)
	//---------------------------------
	int index2 = 0;
	for (int x = -6; x <= 6; x += 3)
	{
		glm::vec2 translation;
		translation.x = x * 0.1f;
		translation.y = 0;
		fireColumnOffset[index2++] = translation;
	}
	// --------------------------------------
	// store instance data in an array buffer(一次生成五龍，該VBO放置算好的偏差值)
	// --------------------------------------
	unsigned int instanceVBO2;
	glGenBuffers(1, &instanceVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 5, &fireColumnOffset[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// --------------------------------------
	// 一般的 VAO VBO EBO
	// --------------------------------------
	glGenVertexArrays(1, &VAOfskill);
	glGenBuffers(1, &VBOfskill);
	glGenBuffers(1, &EBOfskill);

	glBindVertexArray(VAOfskill);

	glBindBuffer(GL_ARRAY_BUFFER, VBOfskill);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skillFirenVertices) + sizeof(skillFirenVertices2), skillFirenVertices, GL_STATIC_DRAW); // A+B的空間中先綁定A
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(skillFirenVertices), sizeof(skillFirenVertices2), skillFirenVertices2); // 再綁定B，才不會圖片有問題

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOfskill);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firenIndices), firenIndices, GL_STATIC_DRAW);

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


	glUseProgram(programFirenSkill);

	for (int spriteID = 0; spriteID < objectCountFiren; ++spriteID)
	{
		firenSkillSheets[spriteID] = new Sprite2D();
	}
	firenSkillSheets[0]->Init("sys/blue_flame.png", 3, 6, 18);
	firenSkillSheets[1]->Init("sys/firen_grd.png", 4, 4, 18);
	firenSkillSheets[2]->Init("sys/firen_column.png", 1, 6, 18);

	glUniform1i(glGetUniformLocation(programFirenSkill, "twinsflame"), 0);
	glUniform1i(glGetUniformLocation(programFirenSkill, "firedragon"), 1);
	glUniform1i(glGetUniformLocation(programFirenSkill, "julianColumn"), 2);



	offsetSkillFirenID = glGetUniformLocation(programFirenSkill, "offset"); // 少了這行，讓offset沒有傳入，使得position*offset未知，圖跑不出來
	firenSkillxID = glGetUniformLocation(programFirenSkill, "skillx");
	firenSkillyID = glGetUniformLocation(programFirenSkill, "skilly");
	isLeftSkillFirenID = glGetUniformLocation(programFirenSkill, "isLeft");
	firenSkillImageID = glGetUniformLocation(programFirenSkill, "skillImage");
	firenSkillTimeID = glGetUniformLocation(programFirenSkill, "time");


	//----------------------------
	//deepblood setting
	//---------------------------
	offsetFirenBlood = translate(0, 0.16f, 0);
	offsetFirenBloodLength = 1;

	ShaderInfo deepBloodShader[] = {
		{ GL_VERTEX_SHADER, "firenblood.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "firenblood.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programFirenBlood = LoadShaders(deepBloodShader);//讀取shader

	glUseProgram(programFirenBlood);//uniform參數數值前必須先use shader

	glGenVertexArrays(1, &VAOfb);
	glGenBuffers(1, &VBOfb);
	glGenBuffers(1, &EBOfb);

	glBindVertexArray(VAOfb);

	glBindBuffer(GL_ARRAY_BUFFER, VBOfb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firenBloodVertices), firenBloodVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOfb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firenBloodIndices), firenBloodIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute(for image1)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glUseProgram(programFirenBlood);

	firenBloodImg = loadTexture("sys/brightred.jpg");

	glUniform1i(glGetUniformLocation(programFirenBlood, "deepBlood"), 0);

	offsetFirenBloodID = glGetUniformLocation(programFirenBlood, "offset"); // 少了這行，讓offset沒有傳入，使得position*offset未知，圖跑不出來
	offsetFirenBloodLengthID = glGetUniformLocation(programFirenBlood, "offsetBlood");
}

void display() {


	//---------------------------------
	//先畫在我的frambuffer上
	//---------------------------------
	
	if (isDeepDie == 1 || isFirenDie == 1) {
		if (frameColor.x > 0.0) {
			frameColor.x -= 0.005f;
			frameColor.y -= 0.005f;
			frameColor.z -= 0.005f;
		}
		printf("frameColor.x = %f\n", frameColor.x);
		
		glUniform4fv(frameColorID, 1, &frameColor[0]);
	}
	else {
		glUniform4fv(frameColorID, 1, &frameColor[0]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);




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

	//-----------------------------------
	//light position draw
	//-----------------------------------
	glUseProgram(programLight);
	
	glUniform3fv(lightPosID, 1, &lightPosForLight[0]);

	glEnable(GL_POINT_SPRITE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBindVertexArray(VAOpr);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightImg);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glDrawArrays(GL_POINTS, 0, 1);

	glDisable(GL_POINT_SPRITE);
	glDisable(GL_BLEND);
	glDisable(GL_PROGRAM_POINT_SIZE);






	// -----------------------
	// rain-particle draw
	// -----------------------
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glUseProgram(programParticleRain2);
	glBindVertexArray(VAOpr2);
	for (unsigned int i = 0; i < particleRainNum; i++) {
		if (particleRain[i].Life > 0.0f) {
			glUniform2fv(offsetParticleRainID, 1, &particleRain[i].Position[0]);
			glUniform4fv(colorParticleRainID, 1, &particleRain[i].Color[0]);
			glUniform1f(particleTimeRainID, currentTime);
			glUniform1f(particleLifeRainID, particleRain[i].Life);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, particleRainImg);
			glBindVertexArray(VAOpr2);
			glUseProgram(programParticleRain2);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
	// Don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	//-----------------------
	// deepskill-draw
	//-----------------------
	// 在deep之後畫skill，skill才能跟deep一起移動
	glUseProgram(programSkill);
	glBindVertexArray(VAOskill);
	glUniformMatrix4fv(offsetSkillID, 1, false, &offsetDeepSkill[0][0]);
	glUniform1i(skillxID, deepSkillx);
	glUniform1i(skillyID, deepSkilly);
	glUniform1i(isLeftSkillID, isLeftDeep);
	glUniform1i(skillImageID, deepSkillImage);
	glUniform1f(skillTimeID, currentTime - deltatime);
	// bind textures on corresponding texture units

	glActiveTexture(GL_TEXTURE0);
	deepSkillSheets[deepSkillImage]->Enable();

	// render container
	glUseProgram(programSkill);
	glBindVertexArray(VAOskill);


	if (drawSkillDeep == 1) {
		if (deepSkillImage == 0) {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else if (deepSkillImage == 1) {
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 5);
			glBindVertexArray(0);
		}
		else if (deepSkillImage == 2) {
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 5);
			glBindVertexArray(0);
		}
	}
	deepSkillSheets[deepImage]->Disable();



	// -----------------------
	//deep-particle draw
	// -----------------------
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glUseProgram(programParticle);
	glBindVertexArray(VAOp);
	
	for (unsigned int i = 0; i < particleNum; i++) {
		if (particles[i].Life > 0.0f) {
			glUniform2fv(offsetParticleID, 1, &particles[i].Position[0]);
			glUniform4fv(colorParticleID, 1, &particles[i].Color[0]);
			glUniformMatrix4fv(projectionID, 1, false, &projection[0][0]);
			glUniformMatrix4fv(offsetParticleMatrixID, 1, false, &offsetParticle[0][0]);
			glUniform1f(particleTimeID, currentTime);
			glUniform1f(particleLifeID, particles[i].Life);
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
	glUniform1f(timeDeepID, deepTime);
	glUniformMatrix4fv(offsetDeepID, 1, false, &offsetDeep[0][0]);
	glUniform1i(deepxID, deepx);
	glUniform1i(deepyID, deepy);
	glUniform1i(isLeftID, isLeftDeep);
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


	//--------------------------------------------------------
	//========================================================
	//-----------------------
	// skill-draw
	//-----------------------
	// 在deep之後畫skill，skill才能跟deep一起移動
	glUseProgram(programFirenSkill);
	glBindVertexArray(VAOfskill);
	glUniformMatrix4fv(offsetSkillFirenID, 1, false, &offsetFirenSkill[0][0]);
	glUniform1i(firenSkillxID, firenSkillx);
	glUniform1i(firenSkillyID, firenSkilly);
	glUniform1i(isLeftSkillFirenID, isLeftFiren);
	glUniform1i(firenSkillImageID, firenSkillImage);
	glUniform1f(firenSkillTimeID, currentTimeFiren - deltatimeFiren);
	// bind textures on corresponding texture units

	glActiveTexture(GL_TEXTURE0);
	firenSkillSheets[firenSkillImage]->Enable();

	// render container
	glUseProgram(programFirenSkill);
	glBindVertexArray(VAOfskill);


	if (drawSkillFiren == 1) {
		if (firenSkillImage == 0) {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else if (firenSkillImage == 1) {
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 5);
			glBindVertexArray(0);
		}
		else if (firenSkillImage == 2) {
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 5);
			glBindVertexArray(0);
		}
	}
	firenSkillSheets[firenImage]->Disable();





	//-----------------------
	// firen-draw
	//-----------------------
	glUseProgram(programFiren); // 記得要useprogram 和 vao，不然東西會綁到上一個skillProgram中
	glBindVertexArray(VAOf);

	glUniform1i(firencontrollerID, firenController);
	glUniform1f(timeFirenID, firenTime);
	glUniformMatrix4fv(offsetFirenID, 1, false, &offsetFiren[0][0]);
	glUniform1i(firenxID, firenx);
	glUniform1i(firenyID, fireny);
	glUniform1i(isLeftFirenID, isLeftFiren);
	glUniform1i(firenImageID, firenImage);
	// bind textures on corresponding texture units
	//glm::mat4 projection2 = glm::ortho(0.0f, SCR_WIDTH, SCR_HEIGHT, 0.0f, -1.0f, 1.0f);
	//glm::mat4 view2 = camera.GetViewMatrix();
	glUseProgram(programFiren);
	glUniformMatrix4fv(projectionFirenID, 1, false, &projection[0][0]);
	glUniformMatrix4fv(viewFirenID, 1, false, &view[0][0]);

	glUniform3fv(viewPosFirenID, 1, &camera.Position[0]);
	glUniform3fv(lightPosFirenID, 1, &lightPos[0]);


	glActiveTexture(GL_TEXTURE0);
	firenSheets[firenImage]->Enable();
	glActiveTexture(GL_TEXTURE1);
	firenNormalSheets[firenImage]->Enable();


	// render container
	glUseProgram(programFiren);
	glBindVertexArray(VAOf);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	firenSheets[firenImage]->Disable();
	firenNormalSheets[firenImage]->Disable();

	//-----------------------
	// firenblood-draw
	//-----------------------
	glUseProgram(programFirenBlood); // 記得要useprogram 和 vao，不然東西會綁到上一個skillProgram中
	glBindVertexArray(VAOfb);

	glUniformMatrix4fv(offsetFirenBloodID, 1, false, &offsetFirenBlood[0][0]);
	glUniform1f(offsetFirenBloodLengthID, offsetFirenBloodLength);

	// bind textures on corresponding texture units

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, firenBloodImg);

	// render container
	glUseProgram(programFirenBlood);
	glBindVertexArray(VAOfb);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//==============================================================================
	//------------------------------------------------------------------------------





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

// learn-opengl tail particle
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
	particle.Velocity = vec2(particle.Velocity.x * particleDir.x, particle.Velocity.y * particleDir.y);
	
	//printf("%d , %d\n", particle.Position.x, particle.Position.y);
}

GLuint FirstUnusedParticleRain()
{
	// Search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticleRain; i < particleRainNum; ++i) {
		if (particleRain[i].Life <= 0.0f) {
			lastUsedParticleRain = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticleRain; ++i) {
		if (particleRain[i].Life <= 0.0f) {
			lastUsedParticleRain = i;
			return i;
		}
	}
	// Override first particle if all others are alive
	lastUsedParticleRain = 0;
	return 0;
}

void RespawnParticleRain(Particle &particle)
{
	
	particle.Position = vec2(random_float() * 2 - 1, random_float()); // -1~1 (起始位置)
	particle.Color = vec4(1.0);
	particle.Life = (random_float() * 2 + 2) * rainLife; // 2-4(生命週期)
	particle.Velocity = vec2(-dt, -dt) * rainSpeed; // (速度和方向)
	particle.Velocity = vec2(particle.Velocity.x * rainDir.x, particle.Velocity.y * rainDir.y);
}


// yao-chih-yuan rain
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
		particleRainNum = 2000;
		break;
	case 1:
		particleNum = 50;
		particleRainNum = 200;
		break;
	case 2:
		particleNum = 10;
		particleRainNum = 40;
		break;
	}
}
void ParticleSpeedMenuEvents(int option) {
	switch (option) {
	case 0:
		particleSpeed = 0.1f;
		rainSpeed = 1;
		break;
	case 1:
		particleSpeed = 0.05f;
		rainSpeed = 0.1f;
		break;
	case 2:
		particleSpeed = 0.5f;
		rainSpeed = 5;
		break;
	case 3:
		particleSpeed = 1.0f;
		rainSpeed = 10;
		break;
	}
	cout << "particleSpeed : " << particleSpeed << endl;
}
void ParticleDirectionMenuEvents(int option) {
	switch (option) {
	case 0:
		particleDir = vec2(1.0, 1.0);
		rainDir = vec2(1.0, 1.0);
		break;
	case 1:
		particleDir = vec2(1.0, -1.0);
		rainDir = vec2(1.0, -1.0);
		break;
	case 2:
		particleDir = vec2(-1.0, -1.0);
		rainDir = vec2(-1.0, -1.0);
		break;
	case 3:
		particleDir = vec2(-1.0, 1.0);
		rainDir = vec2(-1.0, 1.0);
		break;
	}
}
void ParticleLifeMenuEvents(int option) {
	switch (option) {
	case 0:
		particleLife = 1.0f;
		rainLife = 1.0f;
		break;
	case 1:
		particleLife = 0.1f;
		rainLife = 0.1f;
		break;
	case 2:
		particleLife = 3.0f;
		rainLife = 3.0f;
		break;
	case 3:
		particleLife = 5.0f;
		rainLife = 10.0f;
		break;
	}
	cout << "particleLife : " << particleLife << endl;
}