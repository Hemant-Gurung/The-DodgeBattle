#pragma once
struct CharacterDesc
{
	CharacterDesc(
		PxMaterial* pMaterial,
		float radius = .5f,
		float height = 2.f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
	}

	int characterIndex = -1;
	bool isDead = false;
	float maxMoveSpeed{ 15.f }; //Maximum Move
	float maxFallSpeed{ 15.f };

	float JumpSpeed{ 15.f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ .3f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 60.f };

	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Jump{ -1 };
	int actionID_Look{ -1 };
};

class Character : public GameObject
{
public:
	Character(const CharacterDesc& characterDesc);
	~Character();

	Character(const Character& other) = delete;
	Character(Character&& other) noexcept = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) noexcept = delete;

	void DrawImGui();
	void SetIsDead(bool dead);

	static int m_CharIndex;
	void Shoot();
	CharacterDesc m_CharacterDesc;
	ControllerComponent* m_pControllerComponent{};
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	//void GenerateBall();

	//std::vector<GameObject*>m_balls;
	CameraComponent* m_pCameraComponent{};
	
	bool m_IsDead;
	
	float m_TotalPitch{}, m_TotalYaw{}; //Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{}, //Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{}, //Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveVelocity{}; //MoveVelocity > Horizontal Movement = MoveDirection * MoveVelocity

	XMFLOAT3 m_TotalVelocity{}; //TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{}; //Current/Last Direction based on Camera forward/right (Stored for deacceleration)
	float m_PreviousAngle;
	int m_currentAnimation;
	bool playerHasABall{};
	float m_GamePadSensitivity;
	//Ball
	//GameObject* m_Ball;
	float rotateTest;
	float m_characterMoveSpeed;
	ModelAnimator* m_pAnimator;
	bool IsOverlapping(XMFLOAT3 playerPos, XMFLOAT3 targetPos);


	//Play animation time before removing the game object
	float m_accumulatedTime;

};

