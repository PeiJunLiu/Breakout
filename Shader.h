#ifndef SHADER_H
#define SHADER_H
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
	// 状态
	GLuint ID;
	// 构造函数
	Shader() { }
	// 将当前着色器设置为活动
	Shader  &Use();
	// 从给定的源代码编译着色器
	void    Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr); // Note: geometry source code is optional 
	// 实用功能
	//快速设置uniform值
	void    SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void    SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);
	void    SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void    SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	void    SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void    SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void    SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
	// 检查编译或链接是否失败，如果是，则打印错误日志
	void    checkCompileErrors(GLuint object, std::string type);
};

#endif

