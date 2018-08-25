#include "post_processor.h"


#include <iostream>

PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
: PostProcessingShader(shader), Texture(), Width(width), Height(height), Confuse(GL_FALSE), Chaos(GL_FALSE), Shake(GL_FALSE)
{
	// 初始化渲染缓存/帧缓存对象
	glGenFramebuffers(1, &this->MSFBO);
	glGenFramebuffers(1, &this->FBO);
	glGenRenderbuffers(1, &this->RBO);

	// 使用多重采样颜色缓冲区初始化渲染缓存（不需要深度/模板缓存）
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height); // 为渲染缓存对象分配存储空间
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); // 将多重采样渲染缓存对象附加到帧缓冲区
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

	// 同时将FBO/纹理 初始化为blit多重采样颜色缓存;用于着色器操作（用于后期处理）
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	this->Texture.Generate(width, height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0); // 将纹理附加到帧缓冲区作为其颜色附件
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 初始化渲染数据和 uniforms
	this->initRenderData();
	this->PostProcessingShader.SetInteger("scene", 0, GL_TRUE);
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
		{ -offset, offset },	// 左上
		{ 0.0f, offset },		// 中上
		{ offset, offset },		// 右上
		{ -offset, 0.0f },		// 左中
		{ 0.0f, 0.0f },			// 正中
		{ offset, 0.0f },		// 右中
		{ -offset, -offset },	// 左下
		{ 0.0f, -offset },		// 中下
		{ offset, -offset }		// 右下
	};
	glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (GLfloat*)offsets);
	GLint edge_kernel[9] = {
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(this->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
	GLfloat blur_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
	// 现在将多重采样颜色缓冲区解析为中间FBO以存储到纹理中
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //将READ和WRITE帧缓冲区绑定到默认帧缓冲区
}

void PostProcessor::Render(GLfloat time)
{
	// 设置 uniforms/options
	this->PostProcessingShader.Use();
	this->PostProcessingShader.SetFloat("time", time);
	this->PostProcessingShader.SetInteger("confuse", this->Confuse);
	this->PostProcessingShader.SetInteger("chaos", this->Chaos);
	this->PostProcessingShader.SetInteger("shake", this->Shake);
	// 渲染纹理方形
	glActiveTexture(GL_TEXTURE0);
	this->Texture.Bind();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
	// 配置 VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		// 位置        // 纹理
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

