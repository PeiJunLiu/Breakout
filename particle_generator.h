#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "texture.h"
#include "game_object.h"

//�������Ӽ���״̬
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

//ParticleGenerator�䵱������ͨ���������ɺ͸������Ӳ��ڸ�����ʱ���ɱ����������Ⱦ�������ӡ�
class ParticleGenerator
{
public:
	// ���캯��
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	// ������������
	void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// ��Ⱦ��������
	void Draw();
private:
	// ״̬
	std::vector<Particle> particles;
	GLuint amount;  //��������
	// ��Ⱦ״̬
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	// ��ʼ���������Ͷ�������
	void init();

	//firstUnusedParticle()������ͼ�ҵ���һ�����������Ӳ��ҷ�����������ֵ��������.
	GLuint firstUnusedParticle();
	// һ�����������е�һ�����������ӱ����ֵ�ʱ��,��ͨ������RespawnParticle������������ֵ,
	//��������һ��Particle����,һ��GameObject�����һ��offset����
	void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif

