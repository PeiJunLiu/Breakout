#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

struct Character {
	GLuint     TextureID;  // 字形纹理的ID
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	GLuint     Advance;    // 原点距下一个字形原点的距离
};

class TextRenderer
{
public:
	// 保存预编译字符列表
	std::map<GLchar, Character> Characters;
	// 着色器用于文本渲染
	Shader TextShader;
	// 构造函数
	TextRenderer(GLuint width, GLuint height);
	// 预编译给定字体的字符列表
	void Load(std::string font, GLuint fontSize);
	// 使用预编译的字符列表呈现一串文本
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	// 渲染状态
	GLuint VAO, VBO;
};

#endif

