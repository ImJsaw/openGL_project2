#include "main.h"

vec3 camera = vec3(0,0,20);

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4,3);//�HOpenGL version4.3���������
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//�O�_�V�U�ۮe,GLUT_FORWARD_COMPATIBLE���䴩(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
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
	deepTime += deep_interval * deepSpeed * 0.001;
	
	if (deepImage == 2) { // ���K����

		// ���ʹϤ�deep
		if (isLeft == 0) {
			offset = translate(deep_interval * 0.0005, 0, 0) * offset;
			offsetSkill = translate(deep_interval * 0.001, 0, 0) * offsetSkill;
		}
		else if (isLeft == 1) {
			offset = translate(-deep_interval * 0.0005, 0, 0) * offset;
			offsetSkill = translate(-deep_interval * 0.001, 0, 0) * offsetSkill;
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
	else if (deepImage == 1) { // �@�����
		if (isLeft == 0) {
			offset = translate(deep_interval * 0.0005, 0, 0) * offset;
		}
		else if (isLeft == 1) {
			offset = translate(-deep_interval * 0.0005, 0, 0) * offset;
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

	if (drawSkill == 1 && skillImage == 0) { 
		// ���ʨ�i�Ϥ��Askill�Mdeep
		if (isLeft == 0) {
			
			offsetSkill = translate(deep_interval * 0.0001, 0, 0) * offsetSkill;
		}
		else if (isLeft == 1) {
			
			offsetSkill = translate(-deep_interval * 0.0001, 0, 0) * offsetSkill;
		}

		// skill �s��ϰʵe
		if (skillx == 4 && skilly == 2) {
			skillx = 1; // �^���R���
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
}

void Jump_Timer(int val) {

	glutPostRedisplay();
	glutTimerFunc(jump_interval, Jump_Timer, val);
	currentTime += jump_interval * 0.001; 
	// jump_interval �� 30 �ɡA�̱��񥿱`�C�����t��
	if (deepController == 3 || deepController == 7) { // ���_��ۦa���ɶ�
		if ((currentTime - deltatime) >= time_for_a_jump) { // �n�ۦa�F�A�ݦV���@��
			deltatime = 0.0f;
			is_move_when_jump = 0;
			if (deepController == 3) {
				deepController = 0; // �ۦa��A�ݦV�k��
			}
			else if (deepController == 7) {
				deepController = 4; // �ۦa��A�ݦV����
			}
		}
		else if ((currentTime - deltatime) < time_for_a_jump) {
			float radian = DOR((currentTime - deltatime) * (180 / (time_for_a_jump+0.017)));
			// currentTime - deltatime : 0 �� time_for_a_jump(1.2)
			// 180/(time_for_a_jump+0.015) : ��currentTime - deltatime�q0��180(��)�A0.015�O�~�t�A�p�ɾ��M���`�ɶ������t
			if (is_move_when_jump == 1) { // �����V�k
				offset = translate(jump_interval * 0.001, 0, 0) * offset;
			}
			else if (is_move_when_jump == 2) { // �����V��
				offset = translate(-jump_interval * 0.001, 0, 0) * offset;
			}
			offset = translate(0, cos( radian )*0.05, 0) * offset; // ���D���x�}
		}
	}
	else if(deepDirection != -1){//normal move
		offset = translate(jump_interval * 0.001* xMove, jump_interval * 0.001* yMove, 0) * offset;
		deepx++;
		if (deepx == 8) deepx = 1;
	}

}

//  deepController : ��7,��65,��4:��   �k:0��,12��,3��
// deepDirection : 0�� 1�W 2�k 3�U
void Keyboard(unsigned char key, int x, int y) { // �U�ث��s���U�h������
	switch (key) {
	case 'z': // �@��������A
	case 'Z':
		deepImage = 1;
		deepy = 4;
		break;
	case 'x': // ���K�������A
	case 'X':
		deepImage = 2;
		skillImage = 0;
		offsetSkill = offset;
		drawSkill = 1;
		break;
	case 'c': // ���D
	case 'C':
		if (deepController == 1 || deepController == 0) { // �W�@�Ӯɨ譱�V�k��
			
			if (deepController == 1) {
				is_move_when_jump = 1;
			}
			deltatime = currentTime;
			deepController = 3; // �����V�k���D����
		}
		else if(deepController == 5 || deepController == 4){ // �W�@�Ӯɨ譱�V����
			
			if (deepController == 5) {
				is_move_when_jump = 2;
			}
			deltatime = currentTime;
			deepController = 7; // �����V�����D����
		}
		 // ���D
		break;
	case 'W': // ���W��
	case 'w':
		deepDirection = 1;
		yMove++;
		break;
	case 'S': // ���U��
	case 's':
		deepDirection = 3;
		yMove--;
		break;
	case 'a':
	case 'A': // ������
		xMove--;
		if (deepController == 7) { // �W�@�Ӯɨ�V����
			deepController = 7; // ���M�V��
			is_move_when_jump = 2; // �����V������
		}
		else if (deepController == 3) { // �W�@�Ӯɨ�V�k��
			deepController = 7; // �令�V��
			is_move_when_jump = 2; // �����V������
		}
		else {
			deepController = 5; // �V����
			isLeft = 1;
		}
		deepDirection = 0;
		break;
	case 'd':
	case 'D':
		xMove++;
		if (deepController == 7) { // �W�@�Ӯɨ�V����
			deepController = 3; // �令�V�k
			is_move_when_jump = 1; // �����V�k����
		}
		else if (deepController == 3) { // �W�@�Ӯɨ�V�k��
			deepController = 3; // ���M�V�k
			is_move_when_jump = 1; // �����V�k����
		}
		else {
			deepController = 1; // �V�k��
			isLeft = 0;
		}
		deepDirection = 2;
		break;
	}
	glutPostRedisplay();
}
void Keyboardup(unsigned char key, int x, int y) { // �@�먫�����s��}�Y����
	switch (key) {
		
		case 'W': // ���W��
		case 'w':
			yMove--;
			if(!xMove) deepDirection = -1;
			deepx = 1; // �^�쯸��
			deepy = 1;
			break;
		case 'S': // ���U��
		case 's':
			yMove++;
			if (!xMove) deepDirection = -1;
			deepx = 1; // �^�쯸��
			deepy = 1;
			break;
		case 'a':
		case 'A':
			xMove++;
			if (deepController == 7) { // �W�@�Ӯɨ�V����
				deepController = 7; // ���M�V��
				is_move_when_jump = 0;
			}
			else if (deepController == 3) { // �W�@�Ӯɨ�V�k��
				deepController = 7; // �令�V��
				is_move_when_jump = 0;
			}
			else {
				deepController = 4; // �V������
				deepx = 1; // �^�쯸��
				deepy = 1;
			}
			if (!yMove) deepDirection = -1;
			
			break;
		case 'd':
		case 'D':
			xMove--;
			if (deepController == 7) { // �W�@�Ӯɨ�V����
				deepController = 3; // �令�V�k
				is_move_when_jump = 0;
			}
			else if (deepController == 3) { // �W�@�Ӯɨ�V�k��
				deepController = 3; // ���M�V�k
				is_move_when_jump = 0;
			}
			else {
				deepController = 0; // �V�k����
				deepx = 1; // �^�쯸��
				deepy = 1;
			}
			if (!yMove) deepDirection = -1;
			break;
	}
	glutPostRedisplay();
}

void init() {

	//-----------------------
	// deep-setting
	//-----------------------
	deepController = 0; // �V�k����
	offset = scale(1, 1, 1); // ��l�Ưx�}
	deepx = 1; // �K�Ϯy�в��ʯx�}
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
	programDeep = LoadShaders(deepShader);//Ū��shader

	glUseProgram(programDeep);//uniform�ѼƼƭȫe������use shader

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
	
	deep_0 = loadTexture("sys/deep_0.png");
	deep_1 = loadTexture("sys/deep_1.png");
	deep_2 = loadTexture("sys/deep_2.png");
	
	glUniform1i(glGetUniformLocation(programDeep, "deep_0"), 0);
	glUniform1i(glGetUniformLocation(programDeep, "deep_1"), 1);
	glUniform1i(glGetUniformLocation(programDeep, "deep_2"), 2);

	deepcontrollerID = glGetUniformLocation(programDeep, "mario_controller");
	timeID = glGetUniformLocation(programDeep, "time");
	offsetID = glGetUniformLocation(programDeep, "offset"); // �֤F�o��A��offset�S���ǤJ�A�ϱoposition*offset�����A�϶]���X��
	deepxID = glGetUniformLocation(programDeep, "deepx");
	deepyID = glGetUniformLocation(programDeep, "deepy");
	isLeftID = glGetUniformLocation(programDeep, "isLeft");
	deepImageID = glGetUniformLocation(programDeep, "deepImage");

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
	programSkill = LoadShaders(skillShader);//Ū��shader

	glUseProgram(programSkill);//uniform�ѼƼƭȫe������use shader

	glGenVertexArrays(1, &VAOskill);
	glGenBuffers(1, &VBOskill);
	glGenBuffers(1, &EBOskill);

	glBindVertexArray(VAOskill);

	glBindBuffer(GL_ARRAY_BUFFER, VBOskill);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skillVertices), skillVertices, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(deepVertices), sizeof(deepTexCoord2), deepTexCoord2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOskill);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deepIndices), deepIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute(for image-�M�y)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glUseProgram(programSkill);

	twinsflame = loadTexture("sys/twinsflame.png");
	//deep_1 = loadTexture("sys/deep_1.png");
	//deep_2 = loadTexture("sys/deep_2.png");

	glUniform1i(glGetUniformLocation(programSkill, "deep_0"), 0);
	//glUniform1i(glGetUniformLocation(programs, "deep_1"), 1);
	//glUniform1i(glGetUniformLocation(programs, "deep_2"), 2);

	
	offsetSkillID = glGetUniformLocation(programSkill, "offset"); // �֤F�o��A��offset�S���ǤJ�A�ϱoposition*offset�����A�϶]���X��
	skillxID = glGetUniformLocation(programSkill, "skillx");
	skillyID = glGetUniformLocation(programSkill, "skilly");
	isLeftSkillID = glGetUniformLocation(programSkill, "isLeft");
	skillImageID = glGetUniformLocation(programSkill, "skillImage");
}

void display() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	

	
	
	
	
	
	
	//-----------------------
	// deep-draw
	//-----------------------
	glUseProgram(programDeep); // �O�o�nuseprogram �M vao�A���M�F��|�j��W�@��skillProgram��
	glBindVertexArray(VAO);

	glUniform1i(deepcontrollerID, deepController);
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
	glUseProgram(programDeep);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	//-----------------------
	// skill-setting
	//-----------------------
	// �bdeep����eskill�Askill�~���deep�@�_����
	glUseProgram(programSkill);
	glBindVertexArray(VAOskill);
	glUniformMatrix4fv(offsetSkillID, 1, false, &offsetSkill[0][0]);
	glUniform1i(skillxID, skillx);
	glUniform1i(skillyID, skilly);
	glUniform1i(isLeftSkillID, isLeft);
	glUniform1i(skillImageID, skillImage);
	// bind textures on corresponding texture units

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, twinsflame);
	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deep_1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, deep_2);*/
	// render container
	glUseProgram(programSkill);
	glBindVertexArray(VAOskill);
	if (drawSkill == 1) {
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	

	glFlush();//�j�����W����OpenGL commands
	glutSwapBuffers();//�մ��e�x�M��xbuffer ,����Obuffer�e���M�e�xbuffer�洫�ϧڭ̬ݨ���
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	stbi_set_flip_vertically_on_load(true); // ��Ӥ��ॿ

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