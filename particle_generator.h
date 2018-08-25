#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "texture.h"
#include "game_object.h"

//单个粒子及其状态
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

//ParticleGenerator充当容器，通过反复生成和更新粒子并在给定的时间后杀死粒子来渲染大量粒子。
class ParticleGenerator
{
public:
	// 构造函数
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	// 更新所有粒子
	void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// 渲染所有粒子
	void Draw();
private:
	// 状态
	std::vector<Particle> particles;
	GLuint amount;  //粒子数量
	// 渲染状态
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	// 初始化缓冲区和顶点属性
	void init();

	//firstUnusedParticle()函数试图找到第一个消亡的粒子并且返回它的索引值给调用者.
	GLuint firstUnusedParticle();
	// 一旦粒子数组中第一个消亡的粒子被发现的时候,就通过调用RespawnParticle函数更新它的值,
	//函数接受一个Particle对象,一个GameObject对象和一个offset向量
	void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif

