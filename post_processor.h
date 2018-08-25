#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "Shader.h"


//PostProcessorΪBreakout Game�ṩ���к��ڴ���Ч���� 
//���������ı�������Ⱦ��Ϸ��Ȼ��ͨ������Confuse��Chaos��Shake����ֵ�������ض�Ч���� 
//�ڳ�����ϷЧ��֮ǰ��Ҫ����BeginRender������Ȼ������Ⱦ��Ϸ����֮�����EndRender������
class PostProcessor
{
public:
	// ״̬
	Shader PostProcessingShader;
	Texture2D Texture;
	GLuint Width, Height;
	// ��ЧЧ��ѡ���ת�����磬����
	GLboolean Confuse, Chaos, Shake;
	// ���캯��
	PostProcessor(Shader shader, GLuint width, GLuint height);
	// ����Ⱦ��Ϸ֮ǰ׼����������֡�������
	void BeginRender();
	// Ӧ������Ⱦ��Ϸ����ã��������������Ⱦ�����ݴ洢�����������
	void EndRender();
	// Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
	void Render(GLfloat time);
private:
	// ��Ⱦ״̬
	GLuint MSFBO, FBO; // MSFBO =���ز���FBO�� FBO�ǳ���ģ����ڽ�MS��ɫ���������ֵ�����
	GLuint RBO; // RBO���ڶ��ز�����ɫ������
	GLuint VAO;
	// ��ʼ���ı�������Ⱦ��������
	void initRenderData();
};

#endif
