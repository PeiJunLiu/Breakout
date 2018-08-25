#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <GL/glew.h>

#include "texture.h"
#include "Shader.h"

/***********************************************************************************
* 因为着色器需要有一个字节数组或一些字符串来调用它们。我们可以很容易将文件加载代码嵌入到它们自己的类中，
* 但这稍微有点违反了单一功能原则(Single Responsibility Principle)，
* 即这两个类应当分别仅仅关注纹理或者着色器本身，而不是它们的文件加载机制。
***********************************************************************************
* 出于上面的原因
* 在这里创建一个所谓资源管理器的实体，专门加载游戏相关的资源。
* 创建一个资源管理器有多种方法。在这里我们选择使用一个单例(Singleton)的静态资源管理器，
* （由于它静态的本质）它在整个工程中都可以使用，它会封装所有的已加载资源以及一些相关的加载功能。
* 使用一个具有静态属性的单例类有很多优点也有很多缺点。它主要的缺点就是这样会损失OOP属性，并且丧失构造与析构的控制。
* 不过，对于我们这种小项目来说是这些问题也是很容易处理的。
*************************************************************************************/
class ResourceManager
{
public:
	// 资源存储
	static std::map<std::string, Shader>    Shaders;
	static std::map<std::string, Texture2D> Textures;
	// 从文件中加载顶点，片段（和几何）着色器的源代码（并生成）着色器程序。 如果gShaderFile不是nullptr，它还会加载几何着色器
	static Shader   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	// 检索存储的shader
	static Shader   GetShader(std::string name);
	// 从文件加载（并生成）纹理
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	// 检索存储的纹理
	static Texture2D GetTexture(std::string name);
	// 删除所有已加载资源的分配
	static void      Clear();
private:
	// 私有化构造函数，即我们不需要任何实际的资源管理器对象。其成员变量和函数是公开的（静态的）。
	ResourceManager() { }
	// 从文件加载并生成着色器
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	// 从文件加载纹理
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};
#endif

