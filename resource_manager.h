#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <GL/glew.h>

#include "texture.h"
#include "Shader.h"

/***********************************************************************************
* ��Ϊ��ɫ����Ҫ��һ���ֽ������һЩ�ַ������������ǡ����ǿ��Ժ����׽��ļ����ش���Ƕ�뵽�����Լ������У�
* ������΢�е�Υ���˵�һ����ԭ��(Single Responsibility Principle)��
* ����������Ӧ���ֱ������ע���������ɫ���������������ǵ��ļ����ػ��ơ�
***********************************************************************************
* ���������ԭ��
* �����ﴴ��һ����ν��Դ��������ʵ�壬ר�ż�����Ϸ��ص���Դ��
* ����һ����Դ�������ж��ַ���������������ѡ��ʹ��һ������(Singleton)�ľ�̬��Դ��������
* ����������̬�ı��ʣ��������������ж�����ʹ�ã������װ���е��Ѽ�����Դ�Լ�һЩ��صļ��ع��ܡ�
* ʹ��һ�����о�̬���Եĵ������кܶ��ŵ�Ҳ�кܶ�ȱ�㡣����Ҫ��ȱ�������������ʧOOP���ԣ�����ɥʧ�����������Ŀ��ơ�
* ������������������С��Ŀ��˵����Щ����Ҳ�Ǻ����״���ġ�
*************************************************************************************/
class ResourceManager
{
public:
	// ��Դ�洢
	static std::map<std::string, Shader>    Shaders;
	static std::map<std::string, Texture2D> Textures;
	// ���ļ��м��ض��㣬Ƭ�Σ��ͼ��Σ���ɫ����Դ���루�����ɣ���ɫ������ ���gShaderFile����nullptr����������ؼ�����ɫ��
	static Shader   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	// �����洢��shader
	static Shader   GetShader(std::string name);
	// ���ļ����أ������ɣ�����
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	// �����洢������
	static Texture2D GetTexture(std::string name);
	// ɾ�������Ѽ�����Դ�ķ���
	static void      Clear();
private:
	// ˽�л����캯���������ǲ���Ҫ�κ�ʵ�ʵ���Դ�������������Ա�����ͺ����ǹ����ģ���̬�ģ���
	ResourceManager() { }
	// ���ļ����ز�������ɫ��
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	// ���ļ���������
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};
#endif

