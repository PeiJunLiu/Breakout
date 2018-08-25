#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "Shader.h"
class SpriteRenderer
{
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	//一个着色器对象，一个顶点数组对象以及一个渲染和初始化函数。
	Shader shader;
	GLuint quadVAO;

	void initRenderData();
};

#endif

