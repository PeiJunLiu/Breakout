#include <iostream>
#include "texture.h"


Texture2D::Texture2D()
: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	//����Ҳ��ʹ��ID���õġ�������������һ������
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	// Create Texture
	//������Ҫ��������֮���κε�����ָ��������õ�ǰ�󶨵�����.
	glBindTexture(GL_TEXTURE_2D, this->ID);

	/***************************************************************************************************
	* ��һ������ָ��������Ŀ��(Target)������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ�����
	*���κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
	* �ڶ�������Ϊ����ָ���༶��Զ����ļ��������ϣ�������ֶ�����ÿ���༶��Զ����ļ���Ļ�������������0��Ҳ���ǻ�������
	* ��������������OpenGL����ϣ����������Ϊ���ָ�ʽ�����ǵ�ͼ��ֻ��RGBֵ���������Ҳ��������ΪRGBֵ��
	* ���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶ȡ�����֮ǰ����ͼ���ʱ�򴢴������ǣ���������ʹ�ö�Ӧ�ı�����
	* �¸�����Ӧ�����Ǳ���Ϊ0����ʷ���������⣩��
	* ���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
	* ���һ��������������ͼ�����ݡ�
	****************************************************************************************************/
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	
	// Set Texture wrap and filter modes
	/***************************************************************************************************
	* ��һ������ָ��������Ŀ�ꣻ����ʹ�õ���2D�����������Ŀ����GL_TEXTURE_2D��
	* �ڶ���������Ҫ����ָ�����õ�ѡ����Ӧ�õ������ᡣ���Ǵ������õ���WRAPѡ�����ָ��S��T�ᡣ
	* ���һ��������Ҫ���Ǵ���һ�����Ʒ�ʽ
	***************************************************************************************************/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	//���˷�ʽ�����зŴ�(Magnify)����С(Minify)������ʱ���������������˵�ѡ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
