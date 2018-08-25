#include <algorithm>
#include <sstream>
#include <irrKlang\irrKlang.h>
using namespace irrklang;


#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "text_renderer.h"


//加载、播放音频
ISoundEngine *SoundEngine = createIrrKlangDevice();

//精灵渲染
SpriteRenderer	  *Renderer;
// 挡板
GameObject		  *Player;
//球
BallObject		  *Ball;
//粒子
ParticleGenerator *Particles;
//后期处理
PostProcessor     *Effects;
GLfloat            ShakeTime = 0.0f;
//字体
TextRenderer      *Text;

Game::Game(GLuint width, GLuint height) 
: State(GAME_MENU), Keys(), Width(width), Height(height), Lives(3)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
	delete Text;
	SoundEngine->drop();
}

void Game::Init()
{
    // 加载着色器
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
    ResourceManager::LoadShader("shaders/post_processing.vs", "shaders/post_processing.fs", nullptr, "postprocessing");
    //设置着色器
	//因为这个游戏完全是2D的，所以一个正射投影矩阵(Orthographic Projection Matrix)就可以了。
	//由于正射投影矩阵几乎直接变换所有的坐标至裁剪空间，我们可以定义如下的投影矩阵指定世界坐标为屏幕坐标：
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // 加载纹理
    ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
    ResourceManager::LoadTexture("textures/particle.png", GL_TRUE, "particle");
    ResourceManager::LoadTexture("textures/powerup_speed.png", GL_TRUE, "powerup_speed");
    ResourceManager::LoadTexture("textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
    ResourceManager::LoadTexture("textures/powerup_increase.png", GL_TRUE, "powerup_increase");
    ResourceManager::LoadTexture("textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
    ResourceManager::LoadTexture("textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
    ResourceManager::LoadTexture("textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");
    // 设置特定渲染的控件
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	//字体
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("fonts/OCRAEXT.TTF", 24);
    // 加载关卡
    GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
    GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
    GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
    GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // Configure game objects
	//挡板对象拥有位置、大小、渲染纹理等属性，所以我们理所当然地将其定义为一个GameObject。
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
	//音频
	SoundEngine->play2D("audio/breakout.mp3", GL_TRUE);
}


void Game::Update(GLfloat dt)
{
	// 更新对象，Move函数来更新球的位置
	Ball->Move(dt, this->Width);
	// 检测碰撞
	this->DoCollisions();
	// 更新粒子
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
	// 更新道具
	this->UpdatePowerUps(dt);
	// 减少ShakeTime变量的值直到其为0.0，并停用shake特效。
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = GL_FALSE;
	}
	if (Ball->Position.y >= this->Height) // 球是否接触底部边界？
	{
		--this->Lives;
        // 玩家是否已失去所有生命值? : 游戏结束
        if (this->Lives == 0)
        {
            this->ResetLevel();
            this->State = GAME_MENU;
        }
        this->ResetPlayer();
	}
	//获胜条件
	if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Chaos = GL_TRUE;
		this->State = GAME_WIN;
	}
}



//ProcessInput函数，使得当玩家按下A和D时，挡板可以水平移动。
//由于球初始是固定在挡板上的，我们必须让玩家能够从固定的位置重新移动它。我们选择使用空格键来从挡板释放球。
void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;
			this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
		}
	}
	
	if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			Effects->Chaos = GL_FALSE;
			this->State = GAME_MENU;
		}
	}

	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// 移动挡板
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		//按空格释放球
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
}
void Game::Render()
{
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN)
	{
		//开始渲染后期处理
		Effects->BeginRender();
		// 绘制背景
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
			);
		// 绘制关卡
		this->Levels[this->Level].Draw(*Renderer);
		// 绘制挡板
		Player->Draw(*Renderer);
		// 绘制道具
		for (PowerUp &powerUp : this->PowerUps)
		if (!powerUp.Destroyed)
			powerUp.Draw(*Renderer);
		// 绘制粒子 
		Particles->Draw();
		//绘制球
		Ball->Draw(*Renderer);
		// 结束渲染后期处理
		Effects->EndRender();
		//后期渲染
		Effects->Render(glfwGetTime());

		std::stringstream ss; ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}

	if (this->State == GAME_MENU)
	{
		Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
	}

	if (this->State == GAME_WIN)
	{
		Text->RenderText(
			"You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
			);
		Text->RenderText(
			"Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
			);
	}
}

//重置关卡
void Game::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
	//重置生命值
	this->Lives = 3;
}
//重置游戏
void Game::ResetPlayer()
{
	//重置游戏与球的状态
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
	// 同时禁用所有活动的道具
	Effects->Chaos = Effects->Confuse = GL_FALSE;
	Ball->PassThrough = Ball->Sticky = GL_FALSE;
	Player->Color = glm::vec3(1.0f);
	Ball->Color = glm::vec3(1.0f);
}

// PowerUps
GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

void Game::UpdatePowerUps(GLfloat dt)
{
	for (PowerUp &powerUp : this->PowerUps)
	{
		powerUp.Position += powerUp.Velocity * dt;
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f)
			{
				// 之后会将这个道具移除
				powerUp.Activated = GL_FALSE;
				// // 停用效果
				if (powerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{	//仅当没有其他sticky效果处于激活状态时重置
						Ball->Sticky = GL_FALSE;
						Player->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "pass-through")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
					{	// 仅当没有其他pass-through效果处于活动状态时才重置
						Ball->PassThrough = GL_FALSE;
						Ball->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{	// 仅当没有其他confuse效果处于活动状态时才重置
						Effects->Confuse = GL_FALSE;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{	// 仅当没有其他chaos效果处于活动状态时才重置
						Effects->Chaos = GL_FALSE;
					}
				}
			}
		}
	}
	// 通过遍历PowerUps容器，若一个道具被销毁切被停用，则移除它。
	//我们在算法开头使用remove_if函数，通过给定的lamda表达式消除这些对象。
	//remove_if函数将lamda表达式为true的元素移动至容器的末尾并返回一个迭代器指向应被移除的元素范围的开始部分。
	//容器的erase函数接着擦除这个迭代器指向的元素与容器末尾元素之间的所有元素。
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp &powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
	), this->PowerUps.end());
}

GLboolean ShouldSpawn(GLuint chance)
{
    GLuint random = rand() % chance;
    return random == 0;
}

/*****************************************************************************************************
**  道具：
** Speed: 增加小球20%的速度
** Sticky: 当小球与玩家挡板接触时，小球会保持粘在挡板上的状态直到再次按下空格键，这可以让玩家在释放小球前找到更合适的位置
** Pass-Through: 非实心砖块的碰撞处理被禁用，使小球可以穿过并摧毁多个砖块
** Pad-Size-Increase: 增加玩家挡板50像素的宽度
** Confuse: 短时间内激活confuse后期特效，迷惑玩家
** Chaos: 短时间内激活chaos后期特效，使玩家迷失方向
**	
** SpawnPowerUps函数以一定几率（1/75普通道具，1/15负面道具）生成一个新的PowerUp对象，并设置其属性。
** 每种道具有特殊的颜色使它们更具有辨识度，同时根据类型决定其持续时间的秒数，若值为0.0f则表示它持续无限长的时间。
** 除此之外，每个道具初始化时传入被摧毁砖块的位置与对应纹理
******************************************************************************************************/
void Game::SpawnPowerUps(GameObject &block)
{
    if (ShouldSpawn(75)) // 1/75概率
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, 
		block.Position, ResourceManager::GetTexture("powerup_speed")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, 
		block.Position, ResourceManager::GetTexture("powerup_sticky")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, 
		block.Position, ResourceManager::GetTexture("powerup_passthrough")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, 
		block.Position, ResourceManager::GetTexture("powerup_increase")));
    if (ShouldSpawn(15)) // Negative powerups should spawn more often
        this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, 
		block.Position, ResourceManager::GetTexture("powerup_confuse")));
    if (ShouldSpawn(45))
        this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 5.0f, 
		block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

//我们检查道具的类型并相应地改变游戏状态。
//对于Sticky和Pass-through效果，我们也相应地改变了挡板和小球的颜色来给玩家一些当前被激活了哪种效果的反馈。
void ActivatePowerUp(PowerUp &powerUp)
{
	// 根据道具类型发动道具
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = GL_TRUE;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->PassThrough = GL_TRUE;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = GL_TRUE; // 只在chaos未激活时生效，chaos同理
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = GL_TRUE;
	}
}


GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
	// 检查相同类型的另一个PowerUp是否仍处于活动状态
	//在这种情况下，我们不会禁用它的效果
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return GL_TRUE;
    }
    return GL_FALSE;
}


// 碰撞检测
//AABB-AABB碰撞检测
GLboolean CheckCollision(GameObject &one, GameObject &two);
//AABB - 圆碰撞检测
Collision CheckCollision(BallObject &one, GameObject &two);
//碰撞方向
Direction VectorDirection(glm::vec2 closest);

//DoCollision函数不仅仅只检测是否出现了碰撞，而且在碰撞发生时会有适当的动作。
//此函数现在会计算碰撞侵入的程度,并且基于碰撞方向使球的位置矢量与其相加或相减。
void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // 如果collision 是 true
            {
                // 如果不是实心的砖块则摧毁
                if (!box.IsSolid)
                {
                    box.Destroyed = GL_TRUE;
                    this->SpawnPowerUps(box);
					//小球撞击非实心砖块时的音效
					SoundEngine->play2D("audio/bleep.mp3", GL_FALSE);
                }
                else
                {   // 如果是实心的砖块则激活shake特效
                    ShakeTime = 0.05f;
                    Effects->Shake = GL_TRUE;
					//小球撞击实心砖块时的音效
					SoundEngine->play2D("audio/solid.wav", GL_FALSE);
                }
                // 碰撞处理
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
				// 为了实现Pass-through效果的类似小改动。当小球的PassThrough属性被设置为true时，不对非实心砖块做碰撞处理操作。
                if (!(Ball->PassThrough && !box.IsSolid)) 
                {
                    if (dir == LEFT || dir == RIGHT) // 水平方向碰撞
                    {
                        Ball->Velocity.x = -Ball->Velocity.x; // 反转水平速度
                        // // 重定位
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->Position.x += penetration; // 将球右移
                        else
                            Ball->Position.x -= penetration; // 将球左移
                    }
                    else // 垂直方向碰撞
                    {
                        Ball->Velocity.y = -Ball->Velocity.y; // 反转垂直速度
                        // 重定位
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                        if (dir == UP)
                            Ball->Position.y -= penetration; // 将球上移
                        else
                            Ball->Position.y += penetration; // 将球下移
                    }
                }
            }
        }    
    }

    // 检查PowerUps上的碰撞，如果是，激活它们
    for (PowerUp &powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            // 首先检查道具是到达底部边缘，如果是：不激活并销毁
            if (powerUp.Position.y >= this->Height)
                powerUp.Destroyed = GL_TRUE;

            if (CheckCollision(*Player, powerUp))
            {	// 道具与挡板接触，激活它！
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = GL_TRUE;
                powerUp.Activated = GL_TRUE;
				//获得道具时的音效
				SoundEngine->play2D("audio/powerup.wav", GL_FALSE);
            }
        }
    }

    // And finally check collisions for player pad (unless stuck)
	//球和玩家之间的碰撞与上面的碰撞稍有不同，因为这里应当基于撞击挡板的点与（挡板）中心的距离来改变球的水平速度。
	//撞击点距离挡板的中心点越远，则水平方向的速度就会越大。
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // C检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);
        // 依据结果移动
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); 
        // 解决粘板问题
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y);

        // 如果激活Sticky powerup，一旦计算出新的速度矢量，也会将球粘到挡板上
        Ball->Stuck = Ball->Sticky;
		//小球在挡板上反弹时的音效
		SoundEngine->play2D("audio/bleep.wav", GL_FALSE);
    }
}

//AABB-AABB碰撞检测
GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	// x轴方向碰撞？
    GLboolean collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
	// y轴方向碰撞？
    GLboolean collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
	// 只有两个轴向都有碰撞时才碰撞
    return collisionX && collisionY;
}

//AABB - 圆碰撞检测
Collision CheckCollision(BallObject &one, GameObject &two) 
{
	// 获取圆的中心 
    glm::vec2 center(one.Position + one.Radius);
	//计算AABB的信息（中心、半边长）
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
	// 获取两个中心的差矢量
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
	glm::vec2 closest = aabb_center + clamped;
    //  获得圆心center和最近点closest的矢量并判断是否 length <= radius
    difference = closest - center;
    if (glm::length(difference) < one.Radius) 
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

// 碰撞方向
/**************************************************************************************
对于Breakout我们使用以下规则来改变球的速度：
如果球撞击AABB的右侧或左侧，它的水平速度（x）将会反转。
如果球撞击AABB的上侧或下侧，它的垂直速度（y）将会反转。
我们定义指向北、南、西和东的四个矢量，然后计算它们和给定矢量的夹角,
这四个方向矢量和给定的矢量点乘积的结果中的最高值（点乘积的最大值为1.0f，代表0度角）即是矢量的方向。
***************************************************************************************/
Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// 上
        glm::vec2(1.0f, 0.0f),	// 右
        glm::vec2(0.0f, -1.0f),	// 下
        glm::vec2(-1.0f, 0.0f)	// 左
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}   