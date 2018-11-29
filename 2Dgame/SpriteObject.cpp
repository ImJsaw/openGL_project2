#include "SpriteObject.h"
//#include "../../Include/Common.h"
#include <include/Common.h>

SpriteObject::SpriteObject()
{
}


SpriteObject::~SpriteObject()
{
}

void SpriteObject::AddSprite(Sprite2D *spriteTex)
{
	sprite = spriteTex;

	glm::vec3 scaleFactor(sprite->GetAspect(), 1, 1);
	SetScale(scaleFactor);
}

void SpriteObject::SetOffsetPerFrame(glm::vec2 offset)
{
	offsetPerFrame = offset;
}

void SpriteObject::SetPosition(glm::vec2 pos)
{
	spritePosition = pos;
}

void SpriteObject::SetScale(glm::vec3 scale)
{
	spriteScale = glm::scale(scale);
}

void SpriteObject::SetStartFrame(int frame)
{
	startFrame = frame;
}

void SpriteObject::Update()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	spritePlayTime += (currentTime - lastTime) / 1000.0f;
	spriteIndex = (int)(spritePlayTime * sprite->GetFPS() + startFrame) % sprite->GetCount();


	spritePosition += offsetPerFrame * (currentTime - lastTime) / 1000.0f;

	lastTime = currentTime;
}

glm::vec2 SpriteObject::GetPosition()
{
	return spritePosition;
}

glm::mat4 SpriteObject::GetScale()
{
	return spriteScale;
}

int SpriteObject::GetCurrentFrame()
{
	return spriteIndex;
}
