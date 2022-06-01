#pragma once
//#include "C:\_GRAPHICS_PROG_2\ExamProjcet\OverlordEngine_x64\OverlordEngine\Scenegraph\GameObject.h"
class Ball :
    public GameObject
{
public:
	Ball();

	~Ball() = default;

	Ball(const Ball& other) = delete;
	Ball(Ball&& other) noexcept = delete;
	Ball& operator=(const Ball& other) = delete;
	Ball& operator=(Ball&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	GameObject* MakeBall();
	void ShootBall(XMFLOAT3 direction);
private:
	GameObject* m_Ball;
	RigidBodyComponent* m_pBallActor;

};

