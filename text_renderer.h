#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

struct Character {
	GLuint     TextureID;  // ���������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
};

class TextRenderer
{
public:
	// ����Ԥ�����ַ��б�
	std::map<GLchar, Character> Characters;
	// ��ɫ�������ı���Ⱦ
	Shader TextShader;
	// ���캯��
	TextRenderer(GLuint width, GLuint height);
	// Ԥ�������������ַ��б�
	void Load(std::string font, GLuint fontSize);
	// ʹ��Ԥ������ַ��б����һ���ı�
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	// ��Ⱦ״̬
	GLuint VAO, VBO;
};

#endif

