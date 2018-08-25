#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "power_up.h"


/*********************************************************************************
** 球还需要有半径和一个布尔值，该布尔值用于指示球被固定(stuck)在玩家挡板上还是被允许自由运动的状态。
** 当游戏开始时，球被初始固定在玩家挡板上，直到玩家按下任意键开始游戏。
*********************************************************************************/
class BallObject:public GameObject
{
public:
	// 球的状态 
	GLfloat   Radius;	//半径
	GLboolean Stuck;	//固定

	GLboolean Sticky, PassThrough;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	//移动球，将其限制在窗口范围内（底边除外）; 回归新的位置
	glm::vec2 Move(GLfloat dt, GLuint window_width);
	//使用给定的位置和速度将球重置为原始状态
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif