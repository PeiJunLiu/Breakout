#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"


// ��������
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// ���ڿ�
const GLuint SCREEN_WIDTH = 800;
// ���ڸ�
const GLuint SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); //������һ���Բ���glewInit�������������������������������ǵ�Ӧ�ó���

	glfwSetKeyCallback(window, key_callback);

	// OpenGL����
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ��ʼ����Ϸ
	Breakout.Init();

	// ����ʱ�����
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// ��MENU״̬�¿�ʼ��Ϸ
	Breakout.State = GAME_MENU;

	while (!glfwWindowShouldClose(window))
	{
		// ��������ʱ��
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// deltaTime = 0.001f;
		//�����û�����
		Breakout.ProcessInput(deltaTime);

		// ������Ϸ״̬
		Breakout.Update(deltaTime);

		// ��Ⱦ
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.Render();

		glfwSwapBuffers(window);
	}

	// ɾ��ʹ����Դ���������ص�������Դ
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// ���û�����ESC��ʱ�����ǽ�WindowShouldClose��������Ϊtrue���ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
		{
			Breakout.Keys[key] = GL_FALSE;
			Breakout.KeysProcessed[key] = GL_FALSE;
		}
	}
}