/***************************************************************************
** ����������Ҫ�ȶ���ʲô��һ��ש�顣
** ����һ������Ϊ��Ϸ����������Ϊһ����Ϸ������Ļ�����ʾ��
** ��������Ϸ�������һЩ״̬���ݣ�����λ�á���С�����ʡ�����������ɫ����ת��
** �Ƿ��Ӳ(���ɱ��ݻ�)���Ƿ񱻴ݻٵ����ԣ�����֮�⣬�����洢��һ��Texture2D������Ϊ
** �侫��(Sprite)��
**********************************************************************************/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"


//���ڱ����뵥����ص�����״̬����������
//��Ϸ����ʵ�塣 ��Ϸ�е�ÿ�����󶼿�����Ҫ
// GameObject����������С״̬��
class GameObject
{
public:
	// ����״̬
	glm::vec2   Position, Size, Velocity;
	glm::vec3   Color;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	GLboolean   Destroyed;
	// ����
	Texture2D   Sprite;
	// ���캯��
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	// ���ƾ���
	virtual void Draw(SpriteRenderer &renderer);
};

#endif
