#include "particle_generator.h"


ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
: shader(shader), texture(texture), amount(amount)
{
	this->init();
}



/******************************************************************************************************
** 第一个循环
** 因为粒子会随着时间消亡,我们就想在每一帧里面产生`newParticles`个新粒子.
** 但是一开始就知道了总的粒子数量是`count`,所以不能简单的往粒子数组里面添加新的粒子.
** 否则的话很快就会得到一个装满成千上万个粒子的数组,考虑到这个粒子数组里面其实只有一小部分粒子是存活的,这样就太浪费效率了.
** 我们要做的就是找到第一个消亡的粒子然后用一个新产生的粒子来更新它.
** 函数`firstUnusedParticle()`就是试图找到第一个消亡的粒子并且返回它的索引值给调用者.
******************************************************************************************************/
void ParticleGenerator::Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset)
{
	// 添加新粒子
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	// 更新所有粒子
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= dt; // 减少粒子生命
		if (p.Life > 0.0f)
		{	// 粒子是活着的，因此更新
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

// 渲染所有粒子
void ParticleGenerator::Draw()
{
	//使用添加剂混合使其具有“发光”效果
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			this->shader.SetVector2f("offset", particle.Position);
			this->shader.SetVector4f("color", particle.Color);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	// 不要忘记重置为默认混合模式
	//当要渲染这些粒子的时候,我们使用GL_ONE替换默认的目的因子模式GL_ONE_MINUS_SRC_ALPHA,
	//这样,这些粒子叠加在一起的时候就会产生一些平滑的发热效果
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{

	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	//创建this-> amount默认粒子实例
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

// 存储最后使用的粒子的索引（用于快速访问下一个死亡粒子）
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{

	//找到的上一个消亡的粒子的索引值,由于下一个消亡的粒子索引值总是在上一个消亡的粒子索引值的右边
	for (GLuint i = lastUsedParticle; i < this->amount; ++i){
		if (this->particles[i].Life <= 0.0f){
			lastUsedParticle = i;
			return i;
		}
	}
	//如果我们没有任何消亡的粒子,我们就简单的做一个线性查找
	for (GLuint i = 0; i < lastUsedParticle; ++i){
		if (this->particles[i].Life <= 0.0f){
			lastUsedParticle = i;
			return i;
		}
	}
	//如果没有粒子消亡就返回索引值0,结果就是第一个粒子被覆盖.	
	lastUsedParticle = 0;
	return 0;
}

//函数简单的重置这个粒子的生命值为1.0f,随机的给一个大于0.5的颜色值(经过颜色向量)并且(在物体周围)分配一个位置和速度基于游戏里的物体.
void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}