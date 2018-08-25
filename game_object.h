/***************************************************************************
** 首先我们需要先定义什么是一个砖块。
** 创建一个被称为游戏对象的组件作为一个游戏内物体的基本表示。
** 这样的游戏对象持有一些状态数据，如其位置、大小与速率。它还持有颜色、旋转、
** 是否坚硬(不可被摧毁)、是否被摧毁的属性，除此之外，它还存储了一个Texture2D变量作为
** 其精灵(Sprite)。
**********************************************************************************/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"


//用于保存与单个相关的所有状态的容器对象
//游戏对象实体。 游戏中的每个对象都可能需要
// GameObject中描述的最小状态。
class GameObject
{
public:
	// 对象状态
	glm::vec2   Position, Size, Velocity;
	glm::vec3   Color;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	GLboolean   Destroyed;
	// 精灵
	Texture2D   Sprite;
	// 构造函数
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	// 绘制精灵
	virtual void Draw(SpriteRenderer &renderer);
};

#endif
