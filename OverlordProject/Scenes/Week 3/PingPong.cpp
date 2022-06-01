#include "stdafx.h"
#include "PingPong.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"



void PingPong::Initialize()
{
	m_IsGameOn = false;
	//auto& physx = PxGetPhysics();
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(0, 80, 0);
	pFixedCamera->GetTransform()->Rotate(90, 0, 0, true);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 1.f);

	//set scene settings

	GetSceneSettings().clearColor = XMFLOAT4{ Colors::Black };
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;

	//m_Ball = AddChild(new SpherePrefab(2.f, 10.f, XMFLOAT4{ Colors::Red }));

	m_Ball = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Red }));

	auto pSphereActor = m_Ball->AddComponent(new RigidBodyComponent(false));
	pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	/* UINT colloderId*/  //pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	pSphereActor->GetTransform()->Translate(0.f, 0.f, 0.f);
	pSphereActor->SetConstraint(RigidBodyConstraint::RotY, false);
	pSphereActor->SetConstraint(RigidBodyConstraint::TransY, false);

	//


	//right wall
	m_RightWall = AddChild(new CubePrefab(2, 5, 25, XMFLOAT4{ Colors::White }));
	auto RightActor = m_RightWall->AddComponent(new RigidBodyComponent(true));
	RightActor->AddCollider(PxBoxGeometry(1, 2.5, 10), *pDefaultMaterial);
	RightActor->GetTransform()->Translate(-50.f, 0.f, 0.f);


	// UP
	InputAction up = InputAction(int(InputID::up), InputState::down, VK_UP, -1, 0, GamepadIndex::playerOne);
	GetSceneContext().pInput->AddInputAction(up);

	InputAction down = InputAction(int(InputID::down), InputState::down, VK_DOWN, -1, 0, GamepadIndex::playerOne);
	GetSceneContext().pInput->AddInputAction(down);
	//InputAction{ int(InputIds::Up),InputTriggerState::down,VK_UP,-1,XINPUT_GAMEPAD_DPAD_UP });


//right wall
	m_LeftWall = AddChild(new CubePrefab(2, 5, 25, XMFLOAT4{ Colors::White }));
	auto LeftActor = m_LeftWall->AddComponent(new RigidBodyComponent(true));
	LeftActor->AddCollider(PxBoxGeometry(1, 2.5, 10), *pDefaultMaterial);
	LeftActor->GetTransform()->Translate(50.f, 0.f, 0.f);

	// UP Left player
	InputAction up2 = InputAction(int(InputID::up1), InputState::down, 'Q', -1, 0, GamepadIndex::playerOne);
	GetSceneContext().pInput->AddInputAction(up2);

	InputAction down2 = InputAction(int(InputID::down1), InputState::down, 'A', -1, 0, GamepadIndex::playerOne);
	GetSceneContext().pInput->AddInputAction(down2);
	//GetSceneContext().pCamera->GetTransform()->Translate(0, 27, 0);
	//GetSceneContext().pCamera->GetTransform()->Rotate(90, 0,0,true);

	InputAction space = InputAction(int(InputID::space), InputState::down, VK_SPACE, -1, 0, GamepadIndex::playerOne);
	GetSceneContext().pInput->AddInputAction(space);



	//up invisible wall
	GameObject* UpWallBeyond = AddChild(new CubePrefab(250.f, 50.f, 10, XMFLOAT4{ Colors::Black }));
	auto UpWallBeyondActor = UpWallBeyond->AddComponent(new RigidBodyComponent(true));
	UpWallBeyondActor->AddCollider(PxBoxGeometry(120.f, 25.f, 5.f), *pDefaultMaterial);
	UpWallBeyondActor->GetTransform()->Translate(-50.f, 0, 30.f);

	//down wall
	GameObject* downWallBeyond = AddChild(new CubePrefab(250.f, 50.f, 10, XMFLOAT4{ Colors::Black }));
	auto downWallBeyondActor = downWallBeyond->AddComponent(new RigidBodyComponent(true));
	downWallBeyondActor->AddCollider(PxBoxGeometry(120.f, 25.f, 5.f), *pDefaultMaterial);
	downWallBeyondActor->GetTransform()->Translate(-50.f, 0, -30.f);


	//left and right
	GameObject* leftWallBeyond = AddChild(new CubePrefab(50.f, 50.f, 100, XMFLOAT4{ Colors::Black }));
	auto leftWallBeyondActor = leftWallBeyond->AddComponent(new RigidBodyComponent(true));
	auto colliderid = leftWallBeyondActor->AddCollider(PxBoxGeometry(25.f, 25.f, 50.f), *pDefaultMaterial);
	leftWallBeyondActor->GetTransform()->Translate(-80.f, 0, 0.f);

	//wall at tight side
	GameObject* rightWallBeyond = AddChild(new CubePrefab(50.f, 50.f, 100, XMFLOAT4{ Colors::Black }));

	//add rigid body component to the gameobject
	auto rightWallBeyondActor = rightWallBeyond->AddComponent(new RigidBodyComponent(true));

	//add collider to the actor and get the collider_id
	auto colliderid2 = rightWallBeyondActor->AddCollider(PxBoxGeometry(25.f, 25.f, 50.f), *pDefaultMaterial);
	rightWallBeyondActor->GetTransform()->Translate(80.f, 0, 0.f);


	//trigger left and right wall
	auto colliderleft = leftWallBeyondActor->GetCollider(colliderid);
	// add trigger to the collider
	colliderleft.SetTrigger(true);

	// set on trigger call back
	leftWallBeyond->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction action)->void
		{
		//if it is triggered
			if (action == PxTriggerAction::ENTER)
			{
				// restart the game
				Restart();
			}
		}
	);

	//trigger right

	auto colliderright = rightWallBeyondActor->GetCollider(colliderid2);
	colliderright.SetTrigger(true);
	rightWallBeyond->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction action)->void
		{
			if (action == PxTriggerAction::ENTER)
			{
				Restart();
			}
		}
	);



	
	
}

void PingPong::Update()
{
	if (GetSceneContext().pInput->IsActionTriggered(int(InputID::up)))
	{
		auto currpos = m_LeftWall->GetTransform()->GetPosition();
		currpos.z += 20.f * GetSceneContext().pGameTime->GetElapsed();

		m_LeftWall->GetTransform()->Translate(currpos.x, currpos.y, currpos.z);

		//m_RightWall->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ 2,0,0 }, PxForceMode::eFORCE);
	}

	if (GetSceneContext().pInput->IsActionTriggered(int(InputID::down)))
	{
		auto currpos = m_LeftWall->GetTransform()->GetPosition();
		currpos.z -= 20.f * GetSceneContext().pGameTime->GetElapsed();

		m_LeftWall->GetTransform()->Translate(currpos.x, currpos.y, currpos.z);

		//m_RightWall->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ 2,0,0 }, PxForceMode::eFORCE);
	}

	if (GetSceneContext().pInput->IsActionTriggered(int(InputID::up1)))
	{
		auto currpos = m_RightWall->GetTransform()->GetPosition();
		currpos.z += 20.f * GetSceneContext().pGameTime->GetElapsed();

		m_RightWall->GetTransform()->Translate(currpos.x, currpos.y, currpos.z);

		//m_RightWall->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ 2,0,0 }, PxForceMode::eFORCE);
	}

	if (GetSceneContext().pInput->IsActionTriggered(int(InputID::down1)))
	{
		auto currpos = m_RightWall->GetTransform()->GetPosition();
		currpos.z -= 20.f * GetSceneContext().pGameTime->GetElapsed();

		m_RightWall->GetTransform()->Translate(currpos.x, currpos.y, currpos.z);

		//m_RightWall->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ 2,0,0 }, PxForceMode::eFORCE);
	}

	if (!m_IsGameOn && GetSceneContext().pInput->IsActionTriggered(int(InputID::space)))
	{
		m_IsGameOn = true;
		m_Ball->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ 20,0,3 }, PxForceMode::eIMPULSE);
	}



}

void PingPong::Restart()
{
	m_IsGameOn = false;
	m_Ball->GetTransform()->Translate(0, 0, 0);
}

void PingPong::Draw()
{

}


