#include "stdafx.h"
#include "Player.h"

#include "Materials/DiffuseMaterial_Skinned.h"
#include "Prefabs/Character.h"
#include "Prefabs/SpherePrefab.h"
#include "CharacterScene.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

Player::Player()
	:m_PlayerIndx(m_CharacterCounter),
	m_HasAball(false),
	m_ShootSpeed(200.f),
	m_Ball(nullptr),
	IsShot(false),
	m_TotalHealth(5),
	m_CounterLivesReset(0),
	m_IsDead(false),
	m_Score(nullptr),
	m_pBall(new Ball()),
	m_pCharacter(),
	m_CharacterDesc(nullptr),
	m_accumulatedTime(0),
	m_IsShooting(false)
{
	++m_CharacterCounter;
	m_Score = new Score();


}

Player::~Player()
{
	for (auto ball : m_Balls)
	{
		delete ball;
		ball = nullptr;
	}
}

XMFLOAT3 Player::GetCurrentPosition()
{
	return m_pCharacter->GetTransform()->GetPosition();
}

bool Player::hasABall()
{
	return m_HasAball;
}

void Player::SetHasABall(bool hasball)
{
	m_HasAball = hasball;

}

Character* Player::GetCharacter()
{
	return m_pCharacter;
}

GameObject* Player::GetBall()
{
	//if(m_HasAball)
	{
		return m_Ball;
	}


	//return nullptr;
}

void Player::SetIsShot(bool shot)
{
	if (!IsShot)
	{
		IsShot = shot;

		//DECREASE THE HEALTH
		//increase this only once per hit

		m_TotalHealth--;
		m_Score->SetText(std::to_string(m_TotalHealth));
		//std::cout << "TOTAL HEALTH" << this->GetCharacter() << " " << m_TotalHealth << std::endl;
	}

	//std::cout << this << " Shot";
}

void Player::RemoveBall(Ball& ball)
{
	RemoveChild(&ball, true);
}

void Player::SetIsDead(bool dead)
{
	m_IsDead = dead;
	m_pCharacter->SetIsDead(dead);
}

void Player::Initialize(const SceneContext& scene_context)
{

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(1.f, 0.5f, 0.5f);

	//Character1
	m_CharacterDesc = CharacterDesc{ pDefaultMaterial };
	m_CharacterDesc.actionId_MoveForward = CharacterMoveForward + m_PlayerIndx;
	m_CharacterDesc.actionId_MoveBackward = CharacterMoveBackward + m_PlayerIndx;
	m_CharacterDesc.actionId_MoveLeft = CharacterMoveLeft + m_PlayerIndx;
	m_CharacterDesc.actionId_MoveRight = CharacterMoveRight + m_PlayerIndx;
	m_CharacterDesc.actionId_Jump = CharacterJump + m_PlayerIndx;
	m_CharacterDesc.actionID_Look = CharacterLook + m_PlayerIndx;
	m_CharacterDesc.characterIndex = m_PlayerIndx;
	m_CharacterDesc.isDead = false;


	m_pCharacter = AddChild(new Character(m_CharacterDesc));
	//std::wstring tag;
	//tag = L"Player";
	//tag += std::to_string(m_PlayerIndx);
	m_pCharacter->SetTag(L"Player" + std::to_wstring(m_PlayerIndx));
	if (m_pCharacter->GetTag() == L"Player1")
	{
		m_pCharacter->GetTransform()->Translate(10.f, 10.f, 20 /*+(m_PlayerIndx * 10.f)*/);
	}

	if (m_pCharacter->GetTag() == L"Player0")
	{
		m_pCharacter->GetTransform()->Translate(10.f, 10.f, -20 /*+(m_PlayerIndx * 10.f)*/);
		//this->GetTransform()->Translate(0, -7, 0);
	}
	m_pCharacter->GetTransform()->Scale(0.2f);
	//INITIALIZE INPUTS

	// offset the player pos to fit in the character collider



	{
		//ADD INPUT ACTIONS
		scene_context.pInput->AddInputAction(InputAction(InputIds::CharacterMoveForward + m_PlayerIndx, InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_UP, static_cast<GamepadIndex>(m_PlayerIndx)));
		scene_context.pInput->AddInputAction(InputAction(InputIds::CharacterMoveBackward + m_PlayerIndx, InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, static_cast<GamepadIndex>(m_PlayerIndx)));
		scene_context.pInput->AddInputAction(InputAction(InputIds::CharacterMoveLeft + m_PlayerIndx, InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT, static_cast<GamepadIndex>(m_PlayerIndx)));
		scene_context.pInput->AddInputAction(InputAction(InputIds::CharacterMoveRight + m_PlayerIndx, InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT, static_cast<GamepadIndex>(m_PlayerIndx)));
		scene_context.pInput->AddInputAction(InputAction(InputIds::CharacterJump + m_PlayerIndx, InputState::pressed, -1, -1, XINPUT_GAMEPAD_RIGHT_SHOULDER, static_cast<GamepadIndex>(m_PlayerIndx)));
		scene_context.pInput->AddInputAction(InputAction(InputIds::CharacterLook + m_PlayerIndx, InputState::down, -1, -1, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, static_cast<GamepadIndex>(m_PlayerIndx)));


	}

	//const auto pSkinnedMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	//pSkinnedMaterial->SetDiffuseTexture(L"Textures/PeasantGirl_Diffuse.png");

	auto pSkinnedMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pSkinnedMaterial->SetDiffuseTexture(L"Textures/MyCharacter_Diffuse.png");


	auto pObject = AddChild(new GameObject);

	//pObject->GetTransform()->Rotate(0, 180, 0, true);
	auto pModel = new ModelComponent(L"Meshes/MyCharacter_Update_1.ovm");
	pModel->SetMaterial(pSkinnedMaterial);

	pObject->AddComponent(pModel);
	m_pCharacter->AddComponent(pModel);


	pAnimator = pModel->GetAnimator();
	pAnimator->SetAnimation(m_AnimationClipId);
	pAnimator->SetAnimationSpeed(m_AnimationSpeed);

	//Gather Clip Names
	m_ClipCount = pAnimator->GetClipCount();
	m_ClipNames = new char* [m_ClipCount];
	for (auto i{ 0 }; i < m_ClipCount; ++i)
	{
		auto clipName = StringUtil::utf8_encode(pAnimator->GetClip(i).name);
		m_ClipNames[i] = new char[clipName.size() + 1];
		strncpy_s(m_ClipNames[i], clipName.size() + 1, clipName.c_str(), clipName.size());
		m_ClipNames[i][clipName.size()] = '\0';
	}

	//INITIALIZE SCORE
	InitializeScore(scene_context);

}

void Player::Update(const SceneContext& scenecontext)
{
	if (!m_IsDead)
	{
		if (scenecontext.pInput->IsActionTriggered(m_pCharacter->m_CharacterDesc.actionId_Jump))
		{
			// check if player has a ball first
			std::cout << "player ball current situation " << m_HasAball << '/n';
			if (m_HasAball)
			{
				Shoot();

				//reset the possession of a ball
				m_HasAball = false;
			}
		}

		//RESET IS SHOT VARIABLE
		ResetIsShotVariable(scenecontext);
		CheckIfPlayerIsDead();
		m_Score->Update(scenecontext);
	}
	
	//IsOverlapping(m_pCharacter->GetTransform()->GetPosition(),)
	//GameObject::Update(scene_context);


}

void Player::Shoot()
{
	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.7f, 1.0f, 0.0f);

	m_Ball = AddChild(new SpherePrefab(5.f, 10, XMFLOAT4{ Colors::Green }));

	// current positon
	auto pSphereActor = m_Ball->AddComponent(new RigidBodyComponent(false));
	//pSphereActor->GetTransform()->Translate(0, -10, 0);
	pSphereActor->AddCollider(PxSphereGeometry(5.f), *pDefaultMaterial);
	//m_Ball->AddComponent(pSphereActor);
	//m_Ball = m_pBall->MakeBall();
	auto s = m_pCharacter->m_pControllerComponent->GetTransform()->GetForward();

	auto playerpos = m_pCharacter->GetTransform()->GetPosition();
	//auto characterrPos = m_pCharacter->GetTransform()->GetPosition();
	const float ballOffset = 20.f;
	auto ballfinalpos = XMFLOAT3(playerpos.x + -(s.x * ballOffset), playerpos.y, playerpos.z + -(s.z * ballOffset));

	m_Ball->GetTransform()->Translate(ballfinalpos);
	//std::cout <<"forwardDirection: "<<s.x<<" "<<s.y<< '\n';
	m_Ball->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ -s.x * m_ShootSpeed,0,-s.z * m_ShootSpeed }, PxForceMode::eIMPULSE);
	m_Balls.emplace_back(m_Ball);
	m_IsShooting = true;
}

void Player::CheckIfPlayerIsDead()
{

	if (m_TotalHealth <= 0)
	{
		m_TotalHealth = 0;

		m_IsDead = true;
		m_pCharacter->SetIsDead(true);


		std::cout << "Player " << this->GetCharacter() << " isDead\n";
	}
}

bool Player::IsOverlapping(XMFLOAT3 playerPos, XMFLOAT3 targetPos)
{
	if (m_IsShooting)
	{
		float distancex = playerPos.x - targetPos.x;
		float distancez = playerPos.z - targetPos.z;
		if (abs(distancex) < 5 && abs(distancez) < 5)
		{

			return true;
		}
	}
	return false;

}

void Player::InitializeScore(const SceneContext& scene_context)
{

	//SCORE CLASS
	if (m_pCharacter->GetTag() == L"Player0")
	{
		m_Score->SetTextPosition(XMFLOAT2{ 400,0 });
	}
	else if (m_pCharacter->GetTag() == L"Player1")
	{
		m_Score->SetTextPosition(XMFLOAT2{ 800,0 });
	}
	m_Score->Initialize(scene_context);

}

GameObject* Player::GrabBall(const SceneContext& /*scenecontext*/)
{



	return nullptr;
}

void Player::ResetIsShotVariable(const SceneContext& scenecontext)
{
	if (IsShot)
	{
		m_CounterLivesReset += 10.f * scenecontext.pGameTime->GetElapsed();
		if (m_CounterLivesReset >= 5.f)
		{
			IsShot = false;
			m_CounterLivesReset = 0;
		}
	}
}
