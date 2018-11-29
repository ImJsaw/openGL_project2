#pragma once
//#include "../../Include/Common.h"
#include <include/Common.h>
#include <string>

class Sprite2D
{
public:
	Sprite2D();
	~Sprite2D();

	bool Init(std::string fileName, int rowCount, int colCount, int FPS);
	void Enable();
	void Disable();

	float GetFPS();
	float GetAspect();
	int GetCount();
	int GetIndex();
	glm::mat4 GetModelMat();

private:
	float spriteAspect;
	float spriteFPS;
	int spriteCount;
	int subWidth;
	int subHeight;
	GLuint textureID;
	glm::mat4 scaleMat;
};


