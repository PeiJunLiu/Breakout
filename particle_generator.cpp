#include "particle_generator.h"


ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
: shader(shader), texture(texture), amount(amount)
{
	this->init();
}



/******************************************************************************************************
** ��һ��ѭ��
** ��Ϊ���ӻ�����ʱ������,���Ǿ�����ÿһ֡�������`newParticles`��������.
** ����һ��ʼ��֪�����ܵ�����������`count`,���Բ��ܼ򵥵�������������������µ�����.
** ����Ļ��ܿ�ͻ�õ�һ��װ����ǧ��������ӵ�����,���ǵ������������������ʵֻ��һС���������Ǵ���,������̫�˷�Ч����.
** ����Ҫ���ľ����ҵ���һ������������Ȼ����һ���²�����������������.
** ����`firstUnusedParticle()`������ͼ�ҵ���һ�����������Ӳ��ҷ�����������ֵ��������.
******************************************************************************************************/
void ParticleGenerator::Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset)
{
	// ���������
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	// ������������
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= dt; // ������������
		if (p.Life > 0.0f)
		{	// �����ǻ��ŵģ���˸���
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

// ��Ⱦ��������
void ParticleGenerator::Draw()
{
	//ʹ����Ӽ����ʹ����С����⡱Ч��
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
	// ��Ҫ��������ΪĬ�ϻ��ģʽ
	//��Ҫ��Ⱦ��Щ���ӵ�ʱ��,����ʹ��GL_ONE�滻Ĭ�ϵ�Ŀ������ģʽGL_ONE_MINUS_SRC_ALPHA,
	//����,��Щ���ӵ�����һ���ʱ��ͻ����һЩƽ���ķ���Ч��
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

	//����this-> amountĬ������ʵ��
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

// �洢���ʹ�õ����ӵ����������ڿ��ٷ�����һ���������ӣ�
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{

	//�ҵ�����һ�����������ӵ�����ֵ,������һ����������������ֵ��������һ����������������ֵ���ұ�
	for (GLuint i = lastUsedParticle; i < this->amount; ++i){
		if (this->particles[i].Life <= 0.0f){
			lastUsedParticle = i;
			return i;
		}
	}
	//�������û���κ�����������,���Ǿͼ򵥵���һ�����Բ���
	for (GLuint i = 0; i < lastUsedParticle; ++i){
		if (this->particles[i].Life <= 0.0f){
			lastUsedParticle = i;
			return i;
		}
	}
	//���û�����������ͷ�������ֵ0,������ǵ�һ�����ӱ�����.	
	lastUsedParticle = 0;
	return 0;
}

//�����򵥵�����������ӵ�����ֵΪ1.0f,����ĸ�һ������0.5����ɫֵ(������ɫ����)����(��������Χ)����һ��λ�ú��ٶȻ�����Ϸ�������.
void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}