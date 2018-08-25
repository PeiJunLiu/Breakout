#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "power_up.h"


/*********************************************************************************
** ����Ҫ�а뾶��һ������ֵ���ò���ֵ����ָʾ�򱻹̶�(stuck)����ҵ����ϻ��Ǳ����������˶���״̬��
** ����Ϸ��ʼʱ���򱻳�ʼ�̶�����ҵ����ϣ�ֱ����Ұ����������ʼ��Ϸ��
*********************************************************************************/
class BallObject:public GameObject
{
public:
	// ���״̬ 
	GLfloat   Radius;	//�뾶
	GLboolean Stuck;	//�̶�

	GLboolean Sticky, PassThrough;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	//�ƶ��򣬽��������ڴ��ڷ�Χ�ڣ��ױ߳��⣩; �ع��µ�λ��
	glm::vec2 Move(GLfloat dt, GLuint window_width);
	//ʹ�ø�����λ�ú��ٶȽ�������Ϊԭʼ״̬
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif