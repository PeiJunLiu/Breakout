#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

//Texture2D�ܹ���OpenGL�д洢����������
//�����й�ʵ�ó������Ա��ڹ���
class Texture2D
{
public:
	// ������������ID����������������������ô�ϸ������
	GLuint ID;
	// ����ͼ��ߴ�
	GLuint Width, Height;	// ����ͼ��Ŀ�Ⱥ͸߶ȣ�������Ϊ��λ��
	// Texture Format
	GLuint Internal_Format; // �������ĸ�ʽ
	GLuint Image_Format;	// ����ͼ��ĸ�ʽ
	//������
	GLuint Wrap_S;			//S��
	GLuint Wrap_T;			//T��
	//�������
	GLuint Filter_Min;		// ����ģʽ�������������<��Ļ����
	GLuint Filter_Max;		// ����ģʽ�������������>��Ļ����
	// ���캯��������Ĭ������ģʽ��
	Texture2D();
	// ��ͼ��������������
	void Generate(GLuint width, GLuint height, unsigned char* data);
	// �������Ϊ��ǰ���GL_TEXTURE_2D�������
	void Bind() const;
};

#endif