#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game_object.h"
#include "game_level.h"
#include "power_up.h"
//代表当前游戏状态
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

//计算碰撞处理所需的数值我们要从碰撞的函数中获取更多的信息而不只只是一个true或false，
//因此我们要返回一个包含更多信息的tuple，这些信息即是碰撞发生时的方向及差矢量（R¯）。
typedef std::tuple<GLboolean, Direction, glm::vec2>  Collision;


// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);
// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;


class Game
{
public:
	// 游戏状态
	GameState  State;
	GLboolean  Keys[1024];
	GLboolean KeysProcessed[1024];
	GLuint     Width, Height;
	//在Breakout游戏中支持多个关卡，因此我们将在Game类中添加一个持有GameLevel变量的容器。
	//同时我们还将存储当前的游戏关卡。
	std::vector<GameLevel> Levels;
	GLuint                 Level;
	//道具
	std::vector<PowerUp>  PowerUps;
	//玩家生命
	GLuint Lives;
	// 构造函数/析构函数
	Game(GLuint width, GLuint height);
	~Game();
	// 初始化游戏状态（加载所有的着色器/纹理/关卡）
	void Init();
	// 游戏循环

	//ProcessInput函数，使用存储在Keys数组里的数据来处理输入。
	void ProcessInput(GLfloat dt);
	//Update函数里面我们可以更新游戏设置状态（比如玩家/球的移动）
	void Update(GLfloat dt);
	//Render函数来对游戏进行渲染
	void Render();
	//碰撞检测
	void DoCollisions();

	// 重置游戏
	void ResetLevel();
	void ResetPlayer();
	//SpawnPowerUps在给定的砖块位置生成一个道具，
	//UpdatePowerUps管理所有当前被激活的道具。

	void SpawnPowerUps(GameObject &block);
	void UpdatePowerUps(GLfloat dt);	
};
#endif
