#include "stdafx.h"
#include "Solar.h"
#include "Texture.h"

#define TEXTURE_LOAD_ERROR	_T("Failed to load texture!")

//构造
CTextures::CTextures()
{

}

//析构
CTextures::~CTextures()
{

}

//加载纹理
int CTextures::load_texture ( char *file_name, int width, int height, int depth, GLenum colour_type, GLenum filter_type )
{
	GLubyte *raw_bitmap ;
	FILE *file;

	//fopen_s打开文件成功返回0
	if ((fopen_s(&file, file_name, "rb") == 0)==NULL )
	{
		return 1;
	}

	raw_bitmap = (GLubyte *)malloc(width * height * depth * (sizeof(GLubyte)));

	if ( raw_bitmap == NULL )
	{
		fclose ( file );
		return 2;
	}

	fread  ( raw_bitmap , width * height * depth, 1 , file );
	fclose ( file);

	//  设置过滤类型
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_type );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_type );

	//  设置纹理环境
	glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	gluBuild2DMipmaps ( GL_TEXTURE_2D, colour_type, width, height, colour_type,
		GL_UNSIGNED_BYTE, raw_bitmap );

	free ( raw_bitmap );

	return 0;
}

//加载星球图像
void CTextures::LoadTextures(GLuint *texture_id, int MaxNrOfTextures)
{
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(MaxNrOfTextures, texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	if (load_texture("mercury.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}

	glBindTexture(GL_TEXTURE_2D, texture_id[1]);
	if (load_texture("venus.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}

	glBindTexture(GL_TEXTURE_2D, texture_id[2]);
	if (load_texture("earth.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}

	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
	if (load_texture("mars.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}

	glBindTexture(GL_TEXTURE_2D, texture_id[4]);
	if (load_texture("jupiter.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}

	glBindTexture(GL_TEXTURE_2D, texture_id[5] );
	if (load_texture( "saturn.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}

	glBindTexture(GL_TEXTURE_2D, texture_id[6]);
	if (load_texture("uranus.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}

	glBindTexture(GL_TEXTURE_2D, texture_id[7]);
	if (load_texture("neptune.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}

	glBindTexture(GL_TEXTURE_2D, texture_id[8]);
	if (load_texture("pluto.raw", 640, 320, 3, GL_RGB, GL_NEAREST))
	{
		MessageBox(NULL, TEXTURE_LOAD_ERROR, _T("Error"), MB_OK);
		exit(1);
	}
}