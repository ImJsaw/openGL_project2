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
	glutCreateWindow("OpenGL 4.3 - Robot");

	glewExperimental = GL_TRUE; //�m��glewInit()���e
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(deep_interval, Deep_Timer, 0);
	glutTimerFunc(jump_interval, Jump_Timer, 0);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboardup);

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
}

void Jump_Timer(int val) {

	glutPostRedisplay();
	glutTimerFunc(jump_interval, Jump_Timer, val);
	currentTime += jump_interval * 0.001; 
	// jump_interval �� 30 �ɡA�̱��񥿱`�C���t��

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
	// ���W�k�U���O�Ҽ{�A���U��deepDirection�O-1�F���k��V��isLeft�A�k0��1
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

//  deepController : ��7,��65,��4:��   �k:0��,12��,3��
// deepDirection : 0�� 1�W 2�k 3�U
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'x': // �������A
	case 'X':
		deepImage = 2;
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
		break;
	case 'S': // ���U��
	case 's':
		deepDirection = 3;
		break;
	case 'a':
	case 'A': // ������
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
void Keyboardup(unsigned char key, int x, int y) {
	switch (key) {
		
		case 'W': // ���W��
		case 'w':
			deepDirection = -1;
			deepx = 1; // �^�쯸��
			deepy = 1;
			break;
		case 'S': // ���U��
		case 's':
			deepDirection = -1;
			deepx = 1; // �^�쯸��
			deepy = 1;
			break;
		case 'a':
		case 'A':
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
			deepDirection = -1;
			
			break;
		case 'd':
		case 'D':
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
			deepDirection = -1;
			break;
	}
	glutPostRedisplay();
}

void init() {

	deepController = 0; // �V�k����
	offset = scale(1, 1, 1); // ��l�Ưx�}
	deepx = 1; // �K�Ϯy�в��ʯx�}
	deepy = 1;

	is_move_when_jump = 0;

	isLeft = 0;
	deepDirection = -1; 
	deepImage = 0;

	ShaderInfo shadersky[] = {
		{ GL_VERTEX_SHADER, "deep.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "deep.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programs = LoadShaders(shadersky);//Ū��shader

	glUseProgram(programs);//uniform�ѼƼƭȫe������use shader

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
	offsetID = glGetUniformLocation(programs, "offset");
	deepxID = glGetUniformLocation(programs, "deepx");
	deepyID = glGetUniformLocation(programs, "deepy");
	isLeftID = glGetUniformLocation(programs, "isLeft");
	deepImageID = glGetUniformLocation(programs, "deepImage");

}

void display() {
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

	glFlush();//�j�����W����OpenGL commands
	glutSwapBuffers();//�մ��e�x�M��xbuffer ,���Obuffer�e���M�e�xbuffer�洫�ϧڭ̬ݨ���
}

// ---------------------------------------------------
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