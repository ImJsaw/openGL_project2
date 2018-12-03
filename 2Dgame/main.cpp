#include "main.h"

//vec3 camera = vec3(0,0,20);
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPosForLight(0.0f, 0.0f, 0.0f);

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4,3);//�HOpenGL version4.3���������
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//�O�_�V�U�ۮe,GLUT_FORWARD_COMPATIBLE���䴩(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("OpenGL 4.3 - 2D Game - LF2");

	glewExperimental = GL_TRUE; //�m��glewInit()���e
	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;//c error
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);

	glutTimerFunc(deep_interval, Deep_Timer, 0);
	glutTimerFunc(jump_interval, Jump_Timer, 0);

	
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboardup);


	int ParticleNumMenu, ParticleSpeedMenu, ParticleDirectionMenu, ParticleLifeMenu;


	int ParticleMenu;

	ParticleNumMenu = glutCreateMenu(ParticleNumMenuEvents);//�إߥk����
	glutAddMenuEntry("500", 0);
	glutAddMenuEntry("50", 1);
	glutAddMenuEntry("10", 2);

	ParticleSpeedMenu = glutCreateMenu(ParticleSpeedMenuEvents);//�إߥk����
	glutAddMenuEntry("1", 0);
	glutAddMenuEntry("0.1", 1);
	glutAddMenuEntry("10", 2);
	glutAddMenuEntry("1000", 3);

	ParticleDirectionMenu = glutCreateMenu(ParticleDirectionMenuEvents);//�إߥk����
	glutAddMenuEntry("left-down", 0);// ���ɰw
	glutAddMenuEntry("left-up", 1);
	glutAddMenuEntry("right-up", 2);
	glutAddMenuEntry("right-down", 3);

	ParticleLifeMenu = glutCreateMenu(ParticleLifeMenuEvents);//�إߥk����
	glutAddMenuEntry("1", 0);
	glutAddMenuEntry("0.1", 1);
	glutAddMenuEntry("3", 2);
	glutAddMenuEntry("5", 3);


	ParticleMenu = glutCreateMenu(ParticleMenuEvents);//�إߥk����
												  //�[�J�k�䪫��
	glutAddSubMenu("number", ParticleNumMenu);
	glutAddSubMenu("speed", ParticleSpeedMenu);
	glutAddSubMenu("direction", ParticleDirectionMenu);
	glutAddSubMenu("life", ParticleLifeMenu);


	glutCreateMenu(MenuEvents);//�إߥk����A�D���
	glutAddSubMenu("particles", ParticleMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p



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
	
	
	lightPos[0] = sin(deepTime);//���O�����k����
	lightPos[2] = cos(deepTime);//���O���e�Ჾ��
	lightPosForLight[0] = cos(deepTime);//���O�����k����
	lightPosForLight[2] = sin(deepTime);//���O���e�Ჾ��
	//---------------------------------------------
	//�}�⪺�s���(�S��ʧ@)
	//---------------------------------------------
	if (deepImage == 2) { // ���K����

		// ���ʹϤ�deep
		if (isLeftDeep == 0) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			offsetDeepSkill = translate(deep_interval * 0.001f, 0, 0) * offsetDeepSkill;
			if (deepPosX < 1) deepPosX += deep_interval * 0.0005f;
		}
		else if (isLeftDeep == 1) {
			offsetDeepSkill = translate(-deep_interval * 0.001f, 0, 0) * offsetDeepSkill;
			 if (deepPosX > -1 ) deepPosX -= deep_interval * 0.0005f;
		}

		// deep�s��ϰʵe
		if (deepx == 1 && deepy == 4) {
			deepImage = 0; // ���^�Ϥ�0
			deepx = 1; // �^���R���
			deepy = 1;
		}
		else if ((deepy == 1 && deepx == 10) || (deepy == 3 && deepx == 10) || (deepy == 2 && deepx == 1) ) { // ���U�@�ƨ�������
			deepy++;
		}
		else if ((deepy == 1 && deepx != 10) || (deepy == 3 && deepx != 10)) { // deepx ���k��
			deepx++;
		}
		else if ((deepy == 2 && deepx != 1) || (deepy == 4 && deepx != 1)) { // deepx ������
			deepx--;
		}

		
	}
	else if (deepImage == 1 && deepy == 4) { // �@�����(�b�s��Ϫ��ĥ|��)
		if (isLeftDeep == 0) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX < 1) deepPosX += deep_interval * 0.0005f;
		}
		else if (isLeftDeep == 1) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX > -1) deepPosX -= deep_interval * 0.0005f;
		}

		if (deepx == 10) {
			deepImage = 0; // ���^�Ϥ�0
			deepx = 1; // �^���R���
			deepy = 1;
		}
		else if(deepx != 10){ // deepx ���k��
			deepx++;
		}
	}
	else if (deepImage == 1 && deepy == 2) { // ��F��(�Ϥ�1���ĤG��)
		if (isLeftDeep == 0) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if(deepPosX < 1)if (deepPosX > -1) deepPosX += deep_interval * 0.0005f;
		}
		else if (isLeftDeep == 1) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX > -1) deepPosX -= deep_interval * 0.0005f;
		}

		if (deepx == 8) {
			deepImage = 0; // ���^�Ϥ�0
			deepx = 1; // �^���R���
			deepy = 1;
		}
		else if (deepx != 10) { // deepx ���k��
			deepx++;
		}
	}
	else if (deepImage == 0 && deepy == 4 && isDeepDie == 0) { // ����
		if (isLeftDeep == 0 && deltatime + 0.2 >= deepTime) {
			//offset = translate(-deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX > -1) deepPosX -= deep_interval * 0.0005f;
		}
		else if (isLeftDeep == 1 && deltatime + 0.2 >= deepTime) {
			//offset = translate(deep_interval * 0.0005f, 0, 0) * offset;
			if (deepPosX < 1) deepPosX += deep_interval * 0.0005f;
		}

		if (deepx == 6) {
			if (offsetDeepBloodLength <= 0.0) { // �w���`�A���X�o�ӳQ���쪺�ʵe
				isDeepDie = 1;
			}
			else if(offsetDeepBloodLength >= 0.0 && deltatime + 1.2 <= deepTime){ // �٨S���A���n���b�a�W�@�U
				deepImage = 0; // ���^�Ϥ�0
				deepx = 1; // �^���R���
				deepy = 1;
				
			}
		}
		else if (deepx != 6) { // deepx ���k��
			deepx++;
		}
	}
	//---------------------------------------------
	//�ޯ�S�ĳs���
	//---------------------------------------------
	if (drawSkillDeep == 1 && deepSkillImage == 0) { // �B���C�𼽩�s���
		
		if (isLeftDeep == 0) {
			
			offsetDeepSkill = translate((currentTime-deltatime)*10 * deep_interval * 0.0001f, 0, 0) * offsetDeepSkill;
		}
		else if (isLeftDeep == 1) {
			
			offsetDeepSkill = translate((currentTime - deltatime) * 10 * -deep_interval * 0.0001f, 0, 0) * offsetDeepSkill;
		}

		// skill �s��ϰʵe
		if (deepSkillx == 4 && deepSkilly == 2) {
			deepSkillx = 1; // �^���R���
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
	else if (drawSkillDeep == 1 && deepSkillImage == 1) { // ���s����s���
		
		if (isLeftDeep == 0) {

			offsetDeepSkill = translate(deep_interval * 0.002f, 0, 0) * offsetDeepSkill;
		}
		else if (isLeftDeep == 1) {

			offsetDeepSkill = translate(-deep_interval * 0.002f, 0, 0) * offsetDeepSkill;
		}

		// skill �s��ϰʵe
		if (deepSkillx == 2 && deepSkilly == 5) {
			deepSkillx = 1; // �^���R���
			deepSkilly = 1;
			drawSkillDeep = 0;
		}
		else if (deepSkillx != 2) { // x : 1 -> 2
			deepSkillx++;
		}
		else if (deepSkillx == 2) { // y : 1 -> 2 �F x : 1 
			deepSkilly++;
			deepSkillx = 1;
		}
	}
	else if (drawSkillDeep == 1 && deepSkillImage == 2) { // ���Q�w�W�l�s���
		if (isLeftDeep) offsetDeepSkill = translate(-0.2f, 0, 0) * offsetDeep;
		else offsetDeepSkill = translate(-0.2f, 0, 0) * offsetDeep;

		// skill �s��ϰʵe
		if (deepSkillx == 4 && deepSkilly == 2) {
			deepSkillx = 1; // �^���R���
			deepSkilly = 1;
			drawSkillDeep = 0;
		}
		else if (deepSkillx != 4) { // x : 1 -> 2
			deepSkillx++;
		}
		else if (deepSkillx == 4) { // y : 1 -> 2 �F x : 1 
			deepSkilly++;
			deepSkillx = 1;
		}
	}

	//---------------------------------------------
	//����ʵe
	//---------------------------------------------
	offsetDeepBlood = translate(0, 0.16f, 0) * offsetDeep; // ������@���ݦbdeep�W��
	
}
void Jump_Timer(int val) {

	glutPostRedisplay();
	glutTimerFunc(jump_interval, Jump_Timer, val);
	currentTime += jump_interval * 0.001f; 

	//----------------------------------------
	// ���_���첾�M�ʵe�s���
	//----------------------------------------
	// jump_interval �� 30 �ɡA�̱��񥿱`�C���t��
	if (deepController == 3 || deepController == 7) { // ���_��ۦa���ɶ�
		if ((currentTime - deltatime) >= time_for_a_jump) { // �n�ۦa�F�A�ݦV���@��
			deltatime = 0.0f;
			is_move_when_jump_deep = 0;
			if (deepController == 3) {
				deepController = 0; // �ۦa��A�ݦV�k��
			}
			else if (deepController == 7) {
				deepController = 4; // �ۦa��A�ݦV����
			}

			deepx = 1;
			deepy = 1;
			deepImage = 0;
		}
		else if ((currentTime - deltatime) < time_for_a_jump) { 
			float radian = DOR((currentTime - deltatime) * (180 / (time_for_a_jump+0.017f)));
			// currentTime - deltatime : 0 �� time_for_a_jump(1.2)
			// 180/(time_for_a_jump+0.015) : ��currentTime - deltatime�q0��180(��)�A0.015�O�~�t�A�p�ɾ��M���`�ɶ������t
			if (is_move_when_jump_deep == 1) { // �����V�k
				//offset = translate(jump_interval * 0.0001f, 0, 0) * offset;
				deepPosX += deep_interval * 0.0005f;
			}
			else if (is_move_when_jump_deep == 2) { // �����V��
				//offset = translate(-jump_interval * 0.0001f, 0, 0) * offset;
				deepPosX -= deep_interval * 0.0005f;
			}
			//offset = translate(0, cos( radian )*0.07f, 0) * offset; // ���D���x�}
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
		if (deepPosY > 0 && deepController != 7) deepPosY = -0.01;
		if (deepx == 8) deepx = 3;
	}

	//------------------------------------
	//update pos and set the boundary
	//------------------------------------
	if (deepPosX > 1) deepPosX = 0.99;
	if (deepPosX < -1) deepPosX = -0.99;
	if (deepPosY < -0.89) deepPosY = -0.89;
	offsetDeep = translate(deepPosX, deepPosY, 0);
	cout << "deep Position : " << deepPosX << " , " << deepPosY << endl;

	//------------------------------
	//particle zone
	//-----------------------------
	// Add new particles
	for (GLuint i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		vec4 deepPos = offsetDeep * deepPosition; // deep����m
		//printf("deepPos = %d , %d\n", deepPos[0], deepPos[1]);
		RespawnParticle(particles[unusedParticle], vec2(deepPos[0], deepPos[1]), vec2(0.1, 0.1));
		//printf("Respawn...\n");
		// (�ͩR�������ɤl, �D����m, �D���b�|)
	}
	// Uupdate all particles
	for (GLuint i = 0; i < particleNum; ++i)
	{
		Particle &p = particles[i];
		// jump_interval * 0.001 �O �C�@�ժ��ɶ��t
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f){	// particle is alive, thus update
			p.Position -= p.Velocity;
			p.Color.a -= dt * 2.5;
			
			p.Color.a -= dt * 2.5f;
		}
	}

	//------------------------------
	//particle rain zone
	//-----------------------------
	// Add new particles
	for (GLuint i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticleRain(); // ���覺�`���B
		RespawnParticleRain(particleRain[unusedParticle]); // ��l�ơA���L�_���^��
	}
	// Uupdate all particles
	for (GLuint i = 0; i < particleRainNum; ++i)
	{
		Particle &p = particleRain[i];
		// jump_interval * 0.001 �O �C�@�ժ��ɶ��t
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f) {	// particle is alive, thus update
			//p.Position += p.Velocity * dt;
			p.Position += p.Velocity;
			p.Color.a -= dt * 1.5;
		}
	}

}

//  deepController : ��7,��65,��4:��   �k:0��,12��,3��
// deepDirection : 0�� 1�W 2�k 3�U
void Keyboard(unsigned char key, int x, int y) { // �U�ث��s���U�h������
	if (isDeepDie == 0) { // �p�G�٨S���A���F����~����
		switch (key) {
		case 'q': // ����
		case 'Q':
			deepImage = 0;
			deepy = 4;
			if (offsetDeepBloodLength > 0) offsetDeepBloodLength -= 0.1f;
			deltatime = deepTime; // �p�ɶ}�l�A��1.2���deep�~��A���_��
			break;
		case 'e': // ���Q�w�W
		case 'E':
			deepImage = 1;
			deepy = 2;
			deepSkillImage = 2; // ���Q�w�W�l�Ϥ�
			if (isLeftDeep) offsetDeepSkill = offsetDeep;
			else offsetDeepSkill = offsetDeep;
			deltatime = currentTime; // ���W�l���V�~�����Pı
			drawSkillDeep = 1;
			break;
		case 'z': // �@��������A�A���s�S�ıҰ�
		case 'Z':
			deepImage = 1;
			deepy = 4;

			deepSkillImage = 1; // �C��Ϥ�
			if (isLeftDeep) offsetDeepSkill = translate(-0.4f, 0, 0) * offsetDeep;
			else offsetDeepSkill = translate(0.4f, 0, 0) * offsetDeep;
			drawSkillDeep = 1;

			break;
		case 'x': // ���K�������A�A�C��S�ıҰ�
		case 'X':
			deepImage = 2;
			deepSkillImage = 0;
			if (isLeftDeep) offsetDeepSkill = translate(-0.1f, 0, 0) * offsetDeep;
			else offsetDeepSkill = translate(0.1f, 0, 0) * offsetDeep;
			drawSkillDeep = 1;

			deltatime = currentTime;

			break;
		case 'c': // ���D
		case 'C':
			if (deepController == 1 || deepController == 0) { // �W�@�Ӯɨ譱�V�k��

				if (deepController == 1) {
					is_move_when_jump_deep = 1;
				}
				deltatime = currentTime;
				deepController = 3; // �����V�k���D����
			}
			else if (deepController == 5 || deepController == 4) { // �W�@�Ӯɨ譱�V����

				if (deepController == 5) {
					is_move_when_jump_deep = 2;
				}
				deltatime = currentTime;
				deepController = 7; // �����V�����D����
			}
			deepx = 1;
			deepy = 7;
			deepImage = 0;

			break;
		case 'W': // ���W��
		case 'w':
			deepDirection = 1;
			yMoveDeep++;
			break;
		case 'S': // ���U��
		case 's':
			deepDirection = 3;
			yMoveDeep--;
			break;
		case 'a':
		case 'A': // ������
			xMoveDeep--;
			if (deepController == 7) { // �W�@�Ӯɨ�V����
				deepController = 7; // ���M�V��
				is_move_when_jump_deep = 2; // �����V������
			}
			else if (deepController == 3) { // �W�@�Ӯɨ�V�k��
				deepController = 7; // �令�V��
				is_move_when_jump_deep = 2; // �����V������
			}
			else {
				deepController = 5; // �V����
				isLeftDeep = 1;
			}
			deepDirection = 0;
			break;
		case 'd':
		case 'D':
			xMoveDeep++;
			if (deepController == 7) { // �W�@�Ӯɨ�V����
				deepController = 3; // �令�V�k
				is_move_when_jump_deep = 1; // �����V�k����
			}
			else if (deepController == 3) { // �W�@�Ӯɨ�V�k��
				deepController = 3; // ���M�V�k
				is_move_when_jump_deep = 1; // �����V�k����
			}
			else {
				deepController = 1; // �V�k��
				isLeftDeep = 0;
			}
			deepDirection = 2;
			break;
		}
	}
	
	glutPostRedisplay();
}
void Keyboardup(unsigned char key, int x, int y) { // �@�먫�����s��}�Y����

	if (isDeepDie == 0) {// �p�G�٨S���A��}����~����
		switch (key) {

		case 'W': // ���W��
		case 'w':
			yMoveDeep--;
			if (!xMoveDeep) deepDirection = -1;
			deepx = 3; // �^�쯸��
			deepy = 1;
			break;
		case 'S': // ���U��
		case 's':
			yMoveDeep++;
			if (!xMoveDeep) deepDirection = -1;
			deepx = 3; // �^�쯸��
			deepy = 1;
			break;
		case 'a':
		case 'A':
			xMoveDeep++;
			if (deepController == 7) { // �W�@�Ӯɨ�V����
				deepController = 7; // ���M�V��
				is_move_when_jump_deep = 0;
			}
			else if (deepController == 3) { // �W�@�Ӯɨ�V�k��
				deepController = 7; // �令�V��
				is_move_when_jump_deep = 0;
			}
			else {
				deepController = 4; // �V������
				deepx = 1; // �^�쯸��
				deepy = 1;
			}
			if (!yMoveDeep) deepDirection = -1;

			break;
		case 'd':
		case 'D':
			xMoveDeep--;
			if (deepController == 7) { // �W�@�Ӯɨ�V����
				deepController = 3; // �令�V�k
				is_move_when_jump_deep = 0;
			}
			else if (deepController == 3) { // �W�@�Ӯɨ�V�k��
				deepController = 3; // ���M�V�k
				is_move_when_jump_deep = 0;
			}
			else {
				deepController = 0; // �V�k����
				deepx = 1; // �^�쯸��
				deepy = 1;
			}
			if (!yMoveDeep) deepDirection = -1;
			break;
		}
	}
	glutPostRedisplay();
}


void init() {
	
	initDeep();


	//----------------------------
	//background setting
	//---------------------------

	ShaderInfo backShader[] = {
		{ GL_VERTEX_SHADER, "back.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "back.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programBack = LoadShaders(backShader);//Ū��shader

	glUseProgram(programBack);//uniform�ѼƼƭȫe������use shader

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

	// debug�A�O�o��shader
	ShaderInfo particleShader[] = {
		{ GL_VERTEX_SHADER, "particle.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "particle.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programParticle = LoadShaders(particleShader);//Ū��shader

	glUseProgram(programParticle);//uniform�ѼƼƭȫe������use shader
	
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

	offsetParticleID = glGetUniformLocation(programParticle, "offset"); // �֤F�o��A��offset�S���ǤJ�A�ϱoposition*offset�����A�϶]���X��
	colorParticleID = glGetUniformLocation(programParticle, "color");
	projectionID = glGetUniformLocation(programParticle, "projection");
	offsetParticleMatrixID = glGetUniformLocation(programParticle, "offsetMat");
	particleTimeID = glGetUniformLocation(programParticle, "time");
	particleLifeID = glGetUniformLocation(programParticle, "life");

	for (GLuint i = 0; i < particleNum; ++i)
		particles.push_back(Particle());


	/*//--------------------------
	//particle system(Rain)
	//--------------------------

	proj_matrix = scale(1, 1, 1);


	// debug�A�O�o��shader
	ShaderInfo particleRainShader[] = {
		{ GL_VERTEX_SHADER, "point_sprite.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "point_sprite.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programParticleRain = LoadShaders(particleRainShader);//Ū��shader

	glUseProgram(programParticleRain);//uniform�ѼƼƭȫe������use shader

	glGenVertexArrays(1, &VAOpr);
	glBindVertexArray(VAOpr);

	proj_location = glGetUniformLocation(programParticleRain, "proj_matrix");
	time_Loc = glGetUniformLocation(programParticleRain, "time");

	glGenVertexArrays(1, &VAOpr);
	glBindVertexArray(VAOpr);

	glGenBuffers(1, &VBOpr);
	glBindBuffer(GL_ARRAY_BUFFER, VBOpr);
	glBufferData(GL_ARRAY_BUFFER, NUM_STARS * sizeof(star_t), NULL, GL_STATIC_DRAW);

	star = (star_t *)glMapBufferRange(GL_ARRAY_BUFFER, 0, NUM_STARS * sizeof(star_t), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	int i;

	for (i = 0; i < NUM_STARS; i++)
	{
		star[i].position[0] = (random_float() * 2.0f);
		star[i].position[1] = (random_float() * 2.0f);
		printf("star = %f %f\n", star[i].position[0], star[i].position[1]);
		star[i].position[2] = 0.0f;
		star[i].color[0] = 0.8f + random_float() * 0.2f;
		star[i].color[1] = 0.8f + random_float() * 0.2f;
		star[i].color[2] = 0.8f + random_float() * 0.2f;
		star[i].life = (random_float() + 1); // 1-2
		
	}


	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), (void *)sizeof(glm::vec3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(star_t), (void *)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	TextureData tdata = Common::Load_png("sys/star.png");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);*/

	//--------------------------
	//particle system(rain--by learn opengl)
	//--------------------------
	// debug�A�O�o��shader

	rainLife = 1;
	rainSpeed = 1;
	ShaderInfo particleShaderRain2[] = {
		{ GL_VERTEX_SHADER, "particle_rain.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "particle_rain.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programParticleRain2 = LoadShaders(particleShaderRain2);//Ū��shader

	glUseProgram(programParticleRain2);//uniform�ѼƼƭȫe������use shader

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

	offsetParticleRainID = glGetUniformLocation(programParticleRain2, "offset"); // particle����m
	colorParticleRainID = glGetUniformLocation(programParticleRain2, "color");
	particleTimeRainID = glGetUniformLocation(programParticleRain2, "time");
	particleLifeRainID = glGetUniformLocation(programParticleRain2, "life");

	for (GLuint i = 0; i < particleRainNum; ++i)
		particleRain.push_back(Particle());



	//------------------------------------------------------------
	//framebuffer zone : �N�ù��e�����U�A�g�L��s�S�ġA�A�Ǧ^���覡
	//------------------------------------------------------------
	ShaderInfo shaderframe[] = {
		{ GL_VERTEX_SHADER, "framebuffer.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "framebuffer.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programFrame = LoadShaders(shaderframe);//Ū��shader

	glUseProgram(programFrame);//uniform�ѼƼƭȫe������use shader

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
	programLight = LoadShaders(shaderlight);//Ū��shader

	glUseProgram(programLight);//uniform�ѼƼƭȫe������use shader

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

	deepController = 0; // �V�k����
	offsetDeep = translate(-0.9f, 0, 0); // ��l�Ưx�}�A�u���}�⪺�x�}translate��ù�����
	deepx = 1; // �K�Ϯy�в��ʯx�}
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
	programDeep = LoadShaders(deepShader);//Ū��shader

	glUseProgram(programDeep);//uniform�ѼƼƭȫe������use shader

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
	offsetDeepID = glGetUniformLocation(programDeep, "offset"); // �֤F�o��A��offset�S���ǤJ�A�ϱoposition*offset�����A�϶]���X��
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
	programSkill = LoadShaders(skillShader);//Ū��shader

	glUseProgram(programSkill);//uniform�ѼƼƭȫe������use shader

	//---------------------------------
	//dragon instance initialize(�����s����moffset)
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
	// store instance data in an array buffer(�@���ͦ����s�A��VBO��m��n�����t��)
	// --------------------------------------
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 5, &dragonOffset[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//---------------------------------
	//dragon instance initialize(���ӬW�l����moffset)
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
	// store instance data in an array buffer(�@���ͦ����s�A��VBO��m��n�����t��)
	// --------------------------------------
	unsigned int instanceVBO2;
	glGenBuffers(1, &instanceVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 5, &columnOffset[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// --------------------------------------
	// �@�몺 VAO VBO EBO
	// --------------------------------------
	glGenVertexArrays(1, &VAOskill);
	glGenBuffers(1, &VBOskill);
	glGenBuffers(1, &EBOskill);

	glBindVertexArray(VAOskill);

	glBindBuffer(GL_ARRAY_BUFFER, VBOskill);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skillVertices) + sizeof(skillVertices2), skillVertices, GL_STATIC_DRAW); // A+B���Ŷ������j�wA
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(skillVertices), sizeof(skillVertices2), skillVertices2); // �A�j�wB�A�~���|�Ϥ������D

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

	// texture coord attribute(for image-�M�y)
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

	// �����s�����t�ȡA�t�~�ǤJ(�]���ϥΪ��O���P��VBO)
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // �qinstanceVBO�ǤJ��
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(5, 1); // tell OpenGL this is an instanced vertex attribute.
	//(index of attribpointer, update/indice)

	// ���Ӧ��Q�w�W�l�����t�ȡA�t�~�ǤJ(�]���ϥΪ��O���P��VBO)
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO2); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // �qinstanceVBO�ǤJ��
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



	offsetSkillID = glGetUniformLocation(programSkill, "offset"); // �֤F�o��A��offset�S���ǤJ�A�ϱoposition*offset�����A�϶]���X��
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
	programDeepBlood = LoadShaders(deepBloodShader);//Ū��shader

	glUseProgram(programDeepBlood);//uniform�ѼƼƭȫe������use shader

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

	offsetDeepBloodID = glGetUniformLocation(programDeepBlood, "offset"); // �֤F�o��A��offset�S���ǤJ�A�ϱoposition*offset�����A�϶]���X��
	offsetDeepBloodLengthID = glGetUniformLocation(programDeepBlood, "offsetBlood");
}



void display() {


	//---------------------------------
	//���e�b�ڪ�frambuffer�W
	//---------------------------------
	
	if (isDeepDie == 1) {
		if (frameColor.x > 0.0) {
			frameColor.x -= 0.005;
			frameColor.y -= 0.005;
			frameColor.z -= 0.005;
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
	//���߻氮�A���bframebuffer����
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





	/*//---------------------------------------
	//rain particle
	//---------------------------------------
	glUseProgram(programParticleRain);
	currentTimeStar = currentTime;
	deltaTimeStar = (currentTimeStar - lastTime) * 0.001;
	lastTime = currentTimeStar;

	glUseProgram(programParticleRain);
	
	float f_timer_cnt = glutGet(GLUT_ELAPSED_TIME);
	float currentTiming = f_timer_cnt * 0.001f;

	currentTiming *= 0.1f;
	//currentTiming -= floor(currentTiming); // 0, -0.9, -0.8, ..., -0.1, 0



	glUniform1f(time_Loc, currentTiming);
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, &proj_matrix[0][0]);

	glEnable(GL_POINT_SPRITE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(VAOpr);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glDrawArrays(GL_POINTS, 0, NUM_STARS);
	
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_BLEND);
	glDisable(GL_PROGRAM_POINT_SIZE);*/


	// -----------------------
	// rain-particle draw
	// -----------------------
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glUseProgram(programParticleRain2);
	glBindVertexArray(VAOpr2);
	for (int i = 0; i < particleRainNum; i++) {
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
	// skill-draw
	//-----------------------
	// �bdeep����eskill�Askill�~���deep�@�_����
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
	
	for (int i = 0; i < particleNum; i++) {
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
	glUseProgram(programDeep); // �O�o�nuseprogram �M vao�A���M�F��|�j��W�@��skillProgram��
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
	glUseProgram(programDeepBlood); // �O�o�nuseprogram �M vao�A���M�F��|�j��W�@��skillProgram��
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
	//framebufferobject--mainmap(�]���쥻�e�b�ۤv��framebuffer�A�^��쥻��buffer)
	//-----------------------------------


	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (really necessery actually, since we will be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT); // ���M�ťD�nbuffer���s��pixel�C��

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





	glFlush();//�j�����W����OpenGL commands
	glutSwapBuffers();//�մ��e�x�M��xbuffer ,���Obuffer�e���M�e�xbuffer�洫�ϧڭ̬ݨ���
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
	//particle.Position = charPos + random + offsett; // �D����m + �H�� + �D���b�|
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
	
	particle.Position = vec2(random_float() * 2 - 1, random_float()); // -1~1 (�_�l��m)
	particle.Color = vec4(1.0);
	particle.Life = (random_float() * 2 + 2) * rainLife; // 2-4(�ͩR�g��)
	particle.Velocity = vec2(-dt, -dt) * rainSpeed; // (�t�שM��V)
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
		rainSpeed = 0.1;
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