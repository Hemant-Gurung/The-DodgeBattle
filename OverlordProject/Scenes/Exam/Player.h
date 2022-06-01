#pragma once
#include "Ball.h"
#include "Prefabs/Character.h"
#include "Score.h"
class Player :
    public GameObject
{
	static inline int m_CharacterCounter = { 0 };

public:
	Player();
	~Player();
	Player(const Player& other) = delete;
	Player(Player&& other) noexcept = delete;
	Player& operator=(const Player& other) = delete;
	Player& operator=(Player&& other) noexcept = delete;

	XMFLOAT3 GetCurrentPosition();
	bool hasABall();
	void SetHasABall(bool hasball);
	Character* GetCharacter();
	GameObject* GetBall();
	void SetIsShot(bool shot);
	void RemoveBall(Ball& ball);
	int GetPlayerLives() { return m_TotalHealth; }
	bool GetIsDead() { return m_IsDead; }
	void SetIsDead(bool dead);
	int GetIndex() { return m_PlayerIndx; }
	void SetHealth(int health) { m_TotalHealth = health; }
	void SetIsShooting(bool shooting) { m_IsShooting = shooting; }
	bool GetIsShooting() { return m_IsShooting; }
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	//Animations
	ModelAnimator* pAnimator{};

	int m_AnimationClipId{ 0 };
	float m_AnimationSpeed{ 1.f };

	char** m_ClipNames{};
	int m_ClipCount{};
	float m_ShootSpeed;
	bool IsShot;
	int m_PlayerIndx{0};
	Character* m_pCharacter;
	CharacterDesc m_CharacterDesc;
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveLeft1,
		CharacterMoveRight,
		CharacterMoveRight1,
		CharacterMoveForward,
		CharacterMoveForward1,
		CharacterMoveBackward,
		CharacterMoveBackward1,
		CharacterJump,
		CharacterJump1,
		CharacterLook,
		CharacterLook1

	};

	bool m_HasAball;
	int m_TotalHealth;
	float m_CounterLivesReset;
	bool m_IsDead;
	//Play animation time before removing the game object
	float m_accumulatedTime;

	//Player Score
	Score* m_Score;

	//Ball Method
	Ball* m_pBall;
	bool m_IsShooting;

	GameObject* GrabBall(const SceneContext&);
	void ResetIsShotVariable(const SceneContext&);
	void Shoot();
	void CheckIfPlayerIsDead();

	bool IsOverlapping(XMFLOAT3 playerPos, XMFLOAT3 targetPos);
	GameObject* m_Ball;

	std::vector<GameObject*> m_Balls;

	// private methodsd
	void InitializeScore(const SceneContext&);
	void InitializePlayerStats();
};






