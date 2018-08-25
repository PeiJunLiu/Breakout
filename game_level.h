#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


/// GameLevel������ש����ΪBreakout�����һ���ֱ��棬���йܴ�Ӳ�̼���/��Ⱦ����Ĺ��ܡ�
class GameLevel
{
public:
	std::vector<GameObject> Bricks;

	GameLevel() { }
	// ���ļ��м��عؿ�
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	// ��Ⱦ�ؿ�
	void Draw(SpriteRenderer &renderer);
	// ���һ���ؿ��Ƿ������ (���зǼ�Ӳ�Ĵ�ש�����ݻ�)
	GLboolean IsCompleted();
private:
	// ��ש�����ݳ�ʼ���ؿ�
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif