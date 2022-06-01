#include "stdafx.h"
#include "CharacterScene.h"

#include "Prefabs/Character.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Prefabs/SpherePrefab.h"
#include "Player.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

CharacterScene::~CharacterScene()
{
	delete m_pCharacter1;
	delete m_pCharacter2;
	delete m_Ball;
	delete pAnimator;
}

void CharacterScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	m_GenerateBall = false;
	m_accumulatedTime = 0;
	m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 1.0f, 0.0f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//Camera
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(0.f, /*m_CharacterDesc.controller.height **/ 280.5f, -230.f);
	pFixedCamera->GetTransform()->Rotate(50, 0, 0, true);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());
	//const auto pCamera = new FixedCamera();
	//AddChild(pCamera);

	
	//Simple Level Base
	const auto pLevelObject = AddChild(new GameObject());
	const auto pLevelMeshBase = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Level_Base_1.ovm"));
	pLevelMeshBase->SetMaterial(MaterialManager::Get()->CreateMaterial<ColorMaterial>());

	const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Level_Base_1.ovpt");
	
	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pGroundMaterial->SetDiffuseTexture(L"Textures/SoilBeach.jpg");

	//const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	//pGroundMaterial->SetDiffuseTexture(L"Textures/SoilBeach.jpg");

	pLevelMeshBase->SetMaterial(pGroundMaterial);

	//SIDE BAR
	const auto pLevelObjectBar = AddChild(new GameObject());
	const auto pLevelMeshBar = pLevelObjectBar->AddComponent(new ModelComponent(L"Meshes/Level_SideBar_1.ovm"));
	pLevelMeshBar->SetMaterial(MaterialManager::Get()->CreateMaterial<ColorMaterial>());

	const auto pLevelActorBar = pLevelObjectBar->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMeshBar = ContentManager::Load<PxTriangleMesh>(L"Meshes/Level_SideBar_1.ovpt");

	//GET MATERIAL FOR THE SIDE BAR
	const auto pBarMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pBarMaterial->SetDiffuseTexture(L"Textures/Mix.jpg");
	pLevelMeshBar->SetMaterial(pBarMaterial);
	////Ground Mesh
	////***********
	//const auto pGroundObj = new GameObject();
	//const auto pGroundModel = new ModelComponent(L"Meshes/UnitPlane.ovm");
	//pGroundModel->SetMaterial(pGroundMaterial);
	pLevelActorBar->AddCollider(PxTriangleMeshGeometry(pPxTriangleMeshBar, PxMeshScale({ 10.5,10.5,10.5f })), *pDefaultMaterial);
	pLevelActorBar->GetTransform()->Scale(10.5f, 10.5f, 10.5f);
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ 10.5f, 10.5f, 10.5f })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale(10.5f, 10.5f, 10.5f);
	//pCamera->GetTransform()->Translate(0.f, /*m_CharacterDesc.controller.height **/ 700.5f, 70.f);
	pLevelActor->SetCollisionGroup(CollisionGroup::Group2);
	for (int i = 0; i < 5; ++i)
	{
		GenerateBall();
	}


	m_pCharacter1 = new Player();
	//m_pCharacter1->SetTag(L"Player1");
	//m_pCharacter1->GetTransform()->Translate(0.f, -7.f, 0.f);

	//m_pCharacter1->GetTransform()->Translate(0.f, 15.f, 0 /*+(m_PlayerIndx * 10.f)*/);

	AddChild(m_pCharacter1);
	

	m_pCharacter2 = new Player();
	//m_pCharacter2->SetTag(L"Player2");
	//m_pCharacter2->GetTransform()->Translate(0.f, -7.f, 0.f);

	//m_pCharacter1->GetTransform()->Translate(10.f, 15.f, 0 /*+(m_PlayerIndx * 10.f)*/);

	AddChild(m_pCharacter2);
	
	

}

void CharacterScene::Update()
{
	GameScene::Update();

	// remove if the element in the vector is null
	m_balls.erase(std::remove_if(m_balls.begin(), m_balls.end(), ISNULL()), m_balls.end());

	//Shoot();
	if (!m_pCharacter1->GetIsDead() || !m_pCharacter2->GetIsDead())
	{
		for (int i = 0; i < m_balls.size(); ++i)
		{
			if (m_balls[i] != nullptr)
			{
				if (!m_pCharacter1->hasABall() && IsOverlapping(m_pCharacter1->GetCurrentPosition(), m_balls[i]->GetTransform()->GetPosition()))
				{
					m_pCharacter1->SetHasABall(true);

					//set the ball possession of the player
					//auto a = m_balls[i]->get
					if (m_balls[i]->GetParent())
						m_pCharacter1->RemoveChild(m_balls[i]);
					else
					{
						RemoveChild(m_balls[i], true);
						// std::cout << a;
					}
					m_balls[i] = nullptr;

					//m_balls.resize(m_balls.size() - 1);
				}

				//check if the ball is taken by the first player
				if (m_balls[i] != nullptr)
				{

					if (!m_pCharacter2->hasABall() && IsOverlapping(m_pCharacter2->GetCurrentPosition(), m_balls[i]->GetTransform()->GetPosition()))
					{


						//set the ball possession of the player
						m_pCharacter2->SetHasABall(true);
						if (m_balls[i]->GetParent())
							m_pCharacter2->RemoveChild(m_balls[i]);
						else
						{
							RemoveChild(m_balls[i], true);
							// std::cout << a;
						}
						m_balls[i] = nullptr;
						//m_balls.resize(m_balls.size() - 1);
					}
				}
			}
		}

		//Update the ball which are being shot
		if (m_pCharacter1->GetBall() != nullptr)
		{
			// add the ball to the scene as child which are made in the player class
			if (!std::count(m_balls.begin(), m_balls.end(), m_pCharacter1->GetBall()))
			{
				//add teh ball as the child of this scene
				AddChild(m_pCharacter1->GetBall());
				AddBall(m_pCharacter1->GetBall());
			}

		}
		if (m_pCharacter2->GetBall() != nullptr)
		{
			if (!std::count(m_balls.begin(), m_balls.end(), m_pCharacter2->GetBall()))
			{
				AddChild(m_pCharacter2->GetBall());
				AddBall(m_pCharacter2->GetBall());
			}
		}
	}
	
	//CHECK THE BALL COLLISION
	CheckIfPlayerIsShot();
	
}


void CharacterScene::GenerateBall()
{
	//m_balls.reserve(5);
	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 1.0f);
	m_Ball = AddChild(new SpherePrefab(5.f, 10, XMFLOAT4{ Colors::Green }));
	if(m_balls.size()>=5)
	{
		//std::replace(m_balls.begin(), m_balls.end()-1,nullptr,m_Ball);
	}
	else
	{
		m_balls.push_back(m_Ball);
	}
	auto pSphereActor = m_Ball->AddComponent(new RigidBodyComponent(false));

	pSphereActor->AddCollider(PxSphereGeometry(5.f), *pDefaultMaterial);
	//pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group1);
	float posx = float(-100 + (rand()) % int((100.f + 1.f) + 100));
	float posz = float(-100 + (rand()) % int((100.f + 1.f) + 100));
	pSphereActor->GetTransform()->Translate(XMFLOAT3{ posx,20,posz });
}

void CharacterScene::AddBall(GameObject* ball)
{
	if(ball!=nullptr)
	{
		m_balls.push_back(ball);
	}
}

void CharacterScene::Shoot()
{
	//auto a = m_pCharacter1->GrabBall(m_SceneContext);
	//RemoveChild(a, true);

}

bool CharacterScene::IsOverlapping(XMFLOAT3 playerPos, XMFLOAT3 targetPos)
{
	float distancex = playerPos.x - targetPos.x;
	float distancez = playerPos.z - targetPos.z;
	const int minimumDistance = 15;
	if(abs(distancex)< minimumDistance && abs(distancez) < minimumDistance)
	{
		
		return true;
	}
	return false;
}

bool CharacterScene::CheckIfPlayerIsShot()
{

	//chec is player is dead
	if (m_pCharacter1->GetIsDead())
	{
	//count before removing child
		m_accumulatedTime += 10.f * m_SceneContext.pGameTime->GetElapsed();
		if (m_accumulatedTime > 10.f)
		{
			m_accumulatedTime = 0;
			m_pCharacter1->SetHealth(5);
			m_pCharacter1->SetIsDead(false);
			SceneManager::Get()->SetActiveGameScene(L"StartScreen");

		}
	
	}
	if (m_pCharacter2->GetIsDead())
	{
		m_accumulatedTime += 10.f * m_SceneContext.pGameTime->GetElapsed();
		if (m_accumulatedTime > 10.f)
		{
			m_accumulatedTime = 0;
			m_pCharacter2->SetHealth(5);
			m_pCharacter2->SetIsDead(false);
			SceneManager::Get()->SetActiveGameScene(L"StartScreen");

		}
	}

	if (!m_pCharacter1->GetIsDead() || !m_pCharacter2->GetIsDead())
	{
		auto ballA = m_pCharacter1->GetBall();
		auto ballB = m_pCharacter2->GetBall();
		//Initialize both players variable isshot


		if (ballA != nullptr && m_pCharacter1->GetIsShooting())
		{

			// CHECK IF THE PLAYER IS SHOT
			if (IsOverlapping(ballA->GetTransform()->GetPosition(), m_pCharacter2->GetCurrentPosition()))
			{
				m_pCharacter2->SetIsShot(true);
				m_pCharacter1->SetIsShooting(false);
				
				//Sleep(10);
				return true;
			}
		}
		if (ballB != nullptr && m_pCharacter2->GetIsShooting())
		{
			//CHECK IF PLAYER B IS SHOT
			if (IsOverlapping(ballB->GetTransform()->GetPosition(), m_pCharacter1->GetCurrentPosition()))
			{
				m_pCharacter1->SetIsShot(true);
				m_pCharacter2->SetIsShooting(false);
				std::cout << "Character1" << " is shot\n";

				return true;
			}
		}

		//reset if the players are shooting or not
		
	}
	return false;
}


void CharacterScene::OnGUI()
{
	if (ImGui::Button(pAnimator->IsPlaying() ? "PAUSE" : "PLAY"))
	{
		if (pAnimator->IsPlaying())pAnimator->Pause();
		else pAnimator->Play();
	}

	if (ImGui::Button("RESET"))
	{
		pAnimator->Reset();
	}

	ImGui::Dummy({ 0,5 });

	bool reversed = pAnimator->IsReversed();
	if (ImGui::Checkbox("Play Reversed", &reversed))
	{
		pAnimator->SetPlayReversed(reversed);
	}

	if (ImGui::ListBox("Animation Clip", &m_AnimationClipId, m_ClipNames, m_ClipCount))
	{
		pAnimator->SetAnimation(m_AnimationClipId);
	}

	if (ImGui::SliderFloat("Animation Speed", &m_AnimationSpeed, 0.f, 4.f))
	{
		pAnimator->SetAnimationSpeed(m_AnimationSpeed);
	}
}
