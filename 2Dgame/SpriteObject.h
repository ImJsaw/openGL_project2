#pragma once

//#include "../../Include/Common.h"
#include <Common.h>
#include "Sprite2D.h"

class SpriteObject
{
public:
	SpriteObject();
	~SpriteObject();

	void AddSprite(Sprite2D *spriteTex);
	void SetOffsetPerFrame(glm::vec2 offset);
	void SetPosition(glm::vec2 pos);
	void SetScale(glm::vec3 scale);
	void SetStartFrame(int frame);

	glm::vec2 GetPosition();
	glm::mat4 GetScale();
	int GetCurrentFrame();

	void Update();

private:
	Sprite2D *sprite;

	glm::vec2 offsetPerFrame;
	glm::vec2 spritePosition;
	glm::mat4 spriteScale;

	float lastTime;
	float currentTime;
	float spritePlayTime;
	float spriteAspect;
	float spriteFPS;
	int spriteIndex;
	int startFrame;
};

