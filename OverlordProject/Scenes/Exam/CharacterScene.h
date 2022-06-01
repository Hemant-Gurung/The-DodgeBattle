#pragma once
#include "Player.h"
#include "Ball.h"
class Character;


struct ISNULL
{
	bool operator()(GameObject* s)
	{
		if(s == nullptr )
		{
			return true;
		}
		return false;
	}
};

class CharacterScene : public GameScene
{
public:
	CharacterScene() :GameScene(L"CharacterScene") {}
	~CharacterScene();
	CharacterScene(const CharacterScene& other) = delete;
	CharacterScene(CharacterScene&& other) noexcept = delete;
	CharacterScene& operator=(const CharacterScene& other) = delete;
	CharacterScene& operator=(CharacterScene&& other) noexcept = delete;


	

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	/*enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,

		CharacterMoveLeft1,
		CharacterMoveRight1,
		CharacterMoveForward1,
		CharacterMoveBackward1,
		CharacterJump1

	};*/

	Player* m_pCharacter1{};
	Player* m_pCharacter2{};
	int m_playerIndex;

	//Animations
	ModelAnimator* pAnimator{};

	int m_AnimationClipId{ 0 };
	float m_AnimationSpeed{ 1.f };
	float m_accumulatedTime;
	char** m_ClipNames{};
	int m_ClipCount{};
	GameObject* m_Ball;
	bool m_GenerateBall;
	std::vector<GameObject*>m_balls;
	void GenerateBall();
	void AddBall(GameObject* ball);
	void Shoot();
	bool IsOverlapping(XMFLOAT3 playerPos,XMFLOAT3 targetPos);
	int m_CharIndex;
	bool CheckIfPlayerIsShot();
};


