#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

//Texture2D能够在OpenGL中存储和配置纹理。
//它还托管实用程序功能以便于管理。
class Texture2D
{
public:
	// 保存纹理对象的ID，用于所有纹理操作以引用此细节纹理
	GLuint ID;
	// 纹理图像尺寸
	GLuint Width, Height;	// 加载图像的宽度和高度（以像素为单位）
	// Texture Format
	GLuint Internal_Format; // 纹理对象的格式
	GLuint Image_Format;	// 加载图像的格式
	//纹理环绕
	GLuint Wrap_S;			//S轴
	GLuint Wrap_T;			//T轴
	//纹理过滤
	GLuint Filter_Min;		// 过滤模式，如果纹理像素<屏幕像素
	GLuint Filter_Max;		// 过滤模式，如果纹理像素>屏幕像素
	// 构造函数（设置默认纹理模式）
	Texture2D();
	// 从图像数据生成纹理
	void Generate(GLuint width, GLuint height, unsigned char* data);
	// 将纹理绑定为当前活动的GL_TEXTURE_2D纹理对象
	void Bind() const;
};

#endif