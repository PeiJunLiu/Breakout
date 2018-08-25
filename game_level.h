#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


/// GameLevel将所有砖块作为Breakout级别的一部分保存，并托管从硬盘加载/渲染级别的功能。
class GameLevel
{
public:
	std::vector<GameObject> Bricks;

	GameLevel() { }
	// 从文件中加载关卡
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	// 渲染关卡
	void Draw(SpriteRenderer &renderer);
	// 检查一个关卡是否已完成 (所有非坚硬的瓷砖均被摧毁)
	GLboolean IsCompleted();
private:
	// 由砖块数据初始化关卡
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif