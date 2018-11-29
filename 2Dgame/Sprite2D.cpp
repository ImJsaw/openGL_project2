#include "Sprite2D.h"

Sprite2D::Sprite2D()
{

}


Sprite2D::~Sprite2D()
{
}

bool Sprite2D::Init(std::string fileName, int rowCount, int colCount, int FPS)
{
	TextureData tdata = Common::Load_png(fileName.c_str());
	if (tdata.data != nullptr)
	{
		subWidth = tdata.width / colCount;
		subHeight = tdata.height / rowCount;
		spriteFPS = FPS;
		spriteCount = rowCount * colCount;
		spriteAspect = (float)subWidth / (float)subHeight;
		scaleMat = glm::scale(glm::vec3(spriteAspect, 1, 1));

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, subWidth, subHeight, spriteCount);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ROW_LENGTH, tdata.width);

		int layer = 0;
		for (int row = rowCount - 1; row >= 0; --row)
		{
			for (int col = 0; col < colCount; ++col)
			{
				GLubyte *data = tdata.data + (row * tdata.width * subHeight + col * subWidth) * 4;
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, subWidth, subHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
				++layer;

				if (layer >= spriteCount)
				{
					break;
				}
			}
		}

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		delete[] tdata.data;

		return true;
	}
	return false;
}

glm::mat4 Sprite2D::GetModelMat()
{
	return scaleMat;
}

float Sprite2D::GetAspect()
{
	return spriteAspect;
}

float Sprite2D::GetFPS()
{
	return spriteFPS;
}

int Sprite2D::GetCount()
{
	return spriteCount;
}

void Sprite2D::Enable()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
}

void Sprite2D::Disable()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
