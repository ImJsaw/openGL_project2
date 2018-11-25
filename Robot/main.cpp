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

void init() {

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

	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}

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