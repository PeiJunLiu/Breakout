#include <iostream>
#include "texture.h"


Texture2D::Texture2D()
: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	//纹理也是使用ID引用的。让我们来创建一个纹理
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	// Create Texture
	//我们需要绑定它，让之后任何的纹理指令都可以配置当前绑定的纹理.
	glBindTexture(GL_TEXTURE_2D, this->ID);

	/***************************************************************************************************
	* 第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理
	*（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
	* 第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
	* 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
	* 第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
	* 下个参数应该总是被设为0（历史遗留的问题）。
	* 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
	* 最后一个参数是真正的图像数据。
	****************************************************************************************************/
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	
	// Set Texture wrap and filter modes
	/***************************************************************************************************
	* 第一个参数指定了纹理目标；我们使用的是2D纹理，因此纹理目标是GL_TEXTURE_2D。
	* 第二个参数需要我们指定设置的选项与应用的纹理轴。我们打算配置的是WRAP选项，并且指定S和T轴。
	* 最后一个参数需要我们传递一个环绕方式
	***************************************************************************************************/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	//过滤方式，进行放大(Magnify)和缩小(Minify)操作的时候可以设置纹理过滤的选项
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
