#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "Shader.h"


//PostProcessor为Breakout Game提供所有后期处理效果。 
//它在纹理四边形上渲染游戏，然后通过启用Confuse，Chaos或Shake布尔值来启用特定效果。 
//在呈现游戏效果之前需要调用BeginRender（），然后在渲染游戏工作之后调用EndRender（）。
class PostProcessor
{
public:
	// 状态
	Shader PostProcessingShader;
	Texture2D Texture;
	GLuint Width, Height;
	// 特效效果选项，反转，混沌，抖动
	GLboolean Confuse, Chaos, Shake;
	// 构造函数
	PostProcessor(Shader shader, GLuint width, GLuint height);
	// 在渲染游戏之前准备处理器的帧缓冲操作
	void BeginRender();
	// 应该在渲染游戏后调用，因此它将所有渲染的数据存储到纹理对象中
	void EndRender();
	// Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
	void Render(GLfloat time);
private:
	// 渲染状态
	GLuint MSFBO, FBO; // MSFBO =多重采样FBO。 FBO是常规的，用于将MS颜色缓冲区布局到纹理
	GLuint RBO; // RBO用于多重采样颜色缓冲区
	GLuint VAO;
	// 初始化四边形以渲染后期纹理
	void initRenderData();
};

#endif
