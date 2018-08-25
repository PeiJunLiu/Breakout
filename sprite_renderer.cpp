#include "sprite_renderer.h"



SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}


//使用精灵渲染器的着色器，配置一个模型矩阵并且设置相关的uniform。这里最重要的就是变换的顺序：
//当试图在一个场景中用旋转矩阵和缩放矩阵放置一个对象的时候，建议是首先做缩放变换，再旋转，最后才是位移变换。
//因为矩阵乘法是从右向左执行的，所以我们变换的矩阵顺序是相反的：移动，旋转，缩放。
void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	// Prepare transformations
	this->shader.Use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

	this->shader.SetMatrix4("model", model);

	// Render textured quad
	this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	// 配置 VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		// 位置     // 纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

