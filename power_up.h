#ifndef POWER_UP_H
#define POWER_UP_H
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game_object.h"
/********************************************************************************
** ����Ϊһ�������࣬�̳�GameObject
** PowerUp�������һ����һЩ����״̬��GameObject
**  ���ߣ�
	Speed: ����С��20%���ٶ�
	Sticky: ��С������ҵ���Ӵ�ʱ��С��ᱣ��ճ�ڵ����ϵ�״ֱ̬���ٴΰ��¿ո�����������������ͷ�С��ǰ�ҵ������ʵ�λ��
	Pass-Through: ��ʵ��ש�����ײ�������ã�ʹС����Դ������ݻٶ��ש��
	Pad-Size-Increase: ������ҵ���50���صĿ��
	Confuse: ��ʱ���ڼ���confuse������Ч���Ի����
	Chaos: ��ʱ���ڼ���chaos������Ч��ʹ�����ʧ����
****************************************************************************************/

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);
class PowerUp : public GameObject
{
public:
	// ��������
	std::string Type;
	GLfloat     Duration;
	GLboolean   Activated;
	// ���캯��
	PowerUp(std::string type, glm::vec3 color, GLfloat duration,
		glm::vec2 position, Texture2D texture)
		: GameObject(position, SIZE, texture, color, VELOCITY),
		Type(type), Duration(duration), Activated()
	{ }
};
#endif
