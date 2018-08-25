#include "Shader.h"
#include <iostream>

Shader &Shader::Use()
{
	//如果成功的完成了程序的链接，就可以调用glUseProgram()来运行着色器代码
	//并且参数设置为程序对象的句柄
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
	GLuint sVertex, sFragment, gShader;
	// Vertex Shader（顶点着色器）
	//创建着色器对象
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	//将着色器源代码关联到sVertex
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	//编译着色器对象的源代码
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");

	// Fragment Shader（片段着色器）
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// If geometry shader source code is given, also compile geometry shader
	//几何着色器
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
	// Shader Program
	//创建着色器程序
	this->ID = glCreateProgram();
	//关联着色器对象
	glAttachShader(this->ID, sVertex);
	glAttachShader(this->ID, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(this->ID, gShader);
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessery
	//当着色器对象任务完成后，将它删除
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}


/*****************************************************************
* glGetUniformLocation查询uniform name的位置值。
* 我们为查询函数提供着色器程序和uniform的名字（这是我们希望获得的位置值的来源）。
* 如果glGetUniformLocation返回 - 1就代表没有找到这个位置值。
* 最后，我们可以通过glUniform*函数设置uniform值。
* 注意，查询uniform地址不要求你之前使用过着色器程序，但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，
* 因为它是在当前激活的着色器程序中设置uniform的。
*****************************************************************/

void Shader::SetFloat(const GLchar *name, GLfloat value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetInteger(const GLchar *name, GLint value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}
