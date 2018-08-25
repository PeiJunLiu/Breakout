#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "text_renderer.h"
#include "resource_manager.h"

TextRenderer::TextRenderer(GLuint width, GLuint height)
{
	// 加载配置着色器
	this->TextShader = ResourceManager::LoadShader("shaders/text.vs", "shaders/text.fs", nullptr, "text");
	this->TextShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	this->TextShader.SetInteger("text", 0);
	// 为纹理四边形配置VAO/VBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, GLuint fontSize)
{
	// 首先清除以前加载的字符
	this->Characters.clear();
	// 然后初始化并加载FreeType库
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) // 发生错误时，所有函数都返回不同于0的值
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	// 将字体加载为面
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	// 设置大小以将字形加载为
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	// 禁用字节对齐限制
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// 然后对于前128个ASCII字符，预加载/编译它们的字符并存储它们
	for (GLubyte c = 0; c < 128; c++) 
	{
		// 加载字符字形
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// 生成纹理
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);
		// 设置纹理选项
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 储存字符供之后使用
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// 清理FreeType的资源
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	//  激活对应的渲染状态
	this->TextShader.Use();
	this->TextShader.SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	// 遍历文本中所有的字符
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// 对每个字符更新VBO
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 0.0 },
			{ xpos, ypos, 0.0, 0.0 },

			{ xpos, ypos + h, 0.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 1.0 },
			{ xpos + w, ypos, 1.0, 0.0 }
		};
		//  在四边形上绘制字形纹理
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// 更新VBO内存的内容
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // 一定要使用glBufferSubData而不是glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 绘制四边形
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6) * scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}