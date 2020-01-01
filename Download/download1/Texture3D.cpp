#include "Texture3D.h"



Texture3D::Texture3D()
{
	textureid = 0;
}

void Texture3D::UseTexture3d()
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, textureid);
}

void Texture3D::generate3dpattern()
{
	for (int x = 0; x < texwidth; x++)
	{
		for (int y = 0; y < texheight; y++)
		{
			for (int z = 0; z < texdepth; z++)
			{
				if ((y / 10) % 2 == 0)
				{
					tex3dpattern[x][y][z] = 0.0f;
				}
				else
				{
					tex3dpattern[x][y][z] = 1.0f;
				}
			}
		}
	}
}

void Texture3D::filldataarray(GLubyte data[])
{
	for (int i = 0; i < texwidth; i++)
	{
		for (int j = 0; j < texheight; j++)
		{
			for (int k = 0; k < texdepth; k++)
			{
				if (tex3dpattern[i][j][k] == 1.0f)
				{
					data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 0] = (GLubyte)255;
					data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 1] = (GLubyte)255;
					data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 2] = (GLubyte)0;
					data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 3] = (GLubyte)255;
				}
				else
				{
					data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 0] = (GLubyte)0;
					data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 1] = (GLubyte)0;
					data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 2] = (GLubyte)255;
					data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 3] = (GLubyte)255;
				}
			}
		}
	}
}

void Texture3D::filldataarrayflash(GLubyte data[])
{
	//check radius blue fades off quickly, green fades of slower, red stays same
	float radius;
	glm::vec3 texturecenter = glm::vec3(texwidth / 2, texheight / 2, texdepth / 2);
	
	for (int i = 0; i < texwidth; i++)
	{
		for (int j = 0; j < texheight; j++)
		{
			for (int k = 0; k < texdepth; k++)
			{
				//calculate radius
				glm::vec3 point = glm::vec3(i, j, k);
				radius = Utils::Distance3vec(texturecenter, point);
				//scale b and g depending on its radius
				int green = 255 * (.2 / radius);
				int blue = 255 * (1 / radius);
				//
				data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 0] = (GLubyte)255; //r
				data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 1] = (GLubyte)green; //g
				data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 2] = (GLubyte)blue; //b
				data[i*(texwidth*texheight * 4) + j * (texheight * 4) + k * 4 + 3] = (GLubyte)255; //a
			}
		}
	}
}

void Texture3D::load3dtexture()
{
	GLubyte* data = new GLubyte[texwidth*texheight*texdepth * 4];

	filldataarrayflash(data);

	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_3D, textureid);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, texwidth, texheight, texdepth);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, texwidth, texheight, texdepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);

	glBindTexture(GL_TEXTURE_3D, 0);
}

Texture3D::~Texture3D()
{
	glDeleteTextures(1, &textureid);
}
