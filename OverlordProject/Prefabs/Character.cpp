#include "stdafx.h"
#include "Prefabs/SpherePrefab.h"
#include "Character.h"
#include <cmath>
int Character::m_CharIndex = 0;
Character::Character(const CharacterDesc& characterDesc) :
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime),
	m_currentAnimation(0),
	m_PreviousAngle(0),
	rotateTest(0),
	playerHasABall(false),
	m_characterMoveSpeed(2.5f),
	m_GamePadSensitivity(8.f),
	m_IsDead(false),
	m_accumulatedTime(0)

{}

Character::~Character()
{
	delete m_pAnimator;
	delete m_pControllerComponent;
	delete m_pCameraComponent;
}


void Character::Initialize(const SceneContext& /*sceneContext*/)
{
	//m_balls.reserve(5);
	/*m_pAnimator = GetComponent<ModelComponent>();
	m_pAnimator->GetAnimator()->Play();*/

	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));
	m_pControllerComponent->SetCollisionGroup(CollisionGroup::Group1);
	
	

}

void Character::Update(const SceneContext& sceneContext)
{

	m_pAnimator = GetComponent<ModelComponent>()->GetAnimator();
	
	m_pAnimator->Play();

	{

		//***************
		//HANDLE INPUT

		//## Input Gathering (move)
		XMFLOAT2 move{}; //Uncomment

		//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward)) move.y = m_characterMoveSpeed;
		else move.y = 0.0f;

		if (move.y == 0)
		{
			if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward)) move.y = -m_characterMoveSpeed;
			else move.y = 0.f;
		}
		//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement

		//move.x should contain a 1 (Right) or -1 (Left) based on the active input (check corresponding actionId in m_CharacterDesc)
		//if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft)) move.x = m_characterMoveSpeed;

		//if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight)) move.x = -m_characterMoveSpeed;

		//if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump))
		//{
		//	//GenerateBall();
		//	Shoot();
		//}


		//## Input Gathering (look)
		XMFLOAT2 look{ 0.f, 0.f }; //Uncomment
		//Only if the Left Mouse Button is Down >  //0x0002
		//get mouse movement
		//if (sceneContext.pInput->IsMouseButton(InputState::down, RI_MOUSE_LEFT_BUTTON_DOWN))
		//if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionID_Look))
		{
			{
				//get mouse movement according to the player index stored in the character description struct
				auto mouseMovement = sceneContext.pInput->GetThumbstickPosition(false, static_cast<GamepadIndex>(m_CharacterDesc.characterIndex));

					//set look
				if (mouseMovement.x > 0 || mouseMovement.y > 0)
				{
					look.x = static_cast<float>(mouseMovement.x * 20.f);
					look.y = static_cast<float>(mouseMovement.y * 20.f);
				}


				/*auto mouseMovement2 = sceneContext.pInput->GetThumbstickPosition(false, static_cast<GamepadIndex>(1));
				if (mouseMovement.x > 0 || mouseMovement.y > 0)
				{
					look.x = static_cast<float>(mouseMovement2.x * 20.f);
					look.y = static_cast<float>(mouseMovement2.y * 20.f);
				}*/

				//get current position
			}
		}
			//auto currPos = XMLoadFloat3(&GetTransform()->GetPosition());



			//GET FORWARD
			XMFLOAT3 forward = GetTransform()->GetForward();
			//GET RIGHT
			XMFLOAT3 right = GetTransform()->GetRight();

			// check if the move vecto is null
			if (!m_IsDead)
			{
				if (move.x != 0 || move.y != 0)
				{
					// check for the animation 
					if (m_currentAnimation != 0)
					{
						m_currentAnimation = 0;
						m_pAnimator->SetAnimation(m_currentAnimation);
					}

					// set the velocity
					m_TotalVelocity.x = (forward.x * move.y) + (right.x * move.x);
					m_TotalVelocity.y = (forward.z * move.y) + (right.z * move.x);

				}
				else
				{
					// if the move is not null set the othe animtion
					if (m_currentAnimation != 1)
					{
						m_currentAnimation = 1;
						m_pAnimator->SetAnimation(m_currentAnimation);
					}

					// set the total velocy to zero
					m_TotalVelocity.x = 0.f;
					m_TotalVelocity.y = 0.f;
				}
				// Store the MouseMovement in the local 'look' variable (cast is required)
			//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look
				if (!m_pControllerComponent->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN))
				{
					m_FallAcceleration = m_TotalVelocity.y * sceneContext.pGameTime->GetElapsed();
					m_TotalVelocity.z = 10.f;

				}
				else
				{
					m_TotalVelocity.z = 0;
				}
			}
			else
			{
				if (m_currentAnimation != 2)
				{
					m_currentAnimation = 2;
					m_pAnimator->SetAnimation(m_currentAnimation);
				}

				
			}

		//***************
		//CAMERA ROTATION

		//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable
			m_TotalYaw += look.x * m_GamePadSensitivity * sceneContext.pGameTime->GetElapsed();
			m_TotalPitch += look.y * m_GamePadSensitivity * sceneContext.pGameTime->GetElapsed();

			//********
			//MOVEMENT
			//## Horizontal Velocity (Forward/Backward/Right/Left)
			auto movetemp = XMFLOAT3(m_TotalVelocity.x * 15.f * sceneContext.pGameTime->GetElapsed()
				, m_TotalVelocity.z * -9.81f * sceneContext.pGameTime->GetElapsed()
				, m_TotalVelocity.y * 15.f * sceneContext.pGameTime->GetElapsed());
			m_pControllerComponent->Move(movetemp);
			
			m_MoveAcceleration *= sceneContext.pGameTime->GetElapsed();
			// implement the rotation	
			
			//call controller component to rotate the character
			m_pControllerComponent->GetTransform()->Rotate(0, m_TotalYaw, 0);
			
			
		//}
		//auto mouseMovement = sceneContext.pInput->GetThumbstickPosition(false, static_cast<GamepadIndex>(m_CharacterDesc.characterIndex));

		auto s = m_pControllerComponent->GetTransform()->GetForward();

		DebugRenderer::DrawLine(this->GetTransform()->GetPosition(), XMFLOAT4{Colors::Green}, XMFLOAT3{ -s.x * 500,s.y * 500,-s.z * 500 },XMFLOAT4{Colors::Red});
	}	
}
//bool Character::IsOverlapping(XMFLOAT3 playerPos, XMFLOAT3 targetPos)
//{
//	float distancex = playerPos.x - targetPos.x;
//	float distancez = playerPos.z - targetPos.z;
//	if (distancex < 1 && distancez < 1)
//	{
//		return true;
//	}
//	return false;
//}



void Character::Shoot()
{

	auto s = m_pControllerComponent->GetTransform()->GetForward();
	
}



void Character::DrawImGui()
{
	if (ImGui::CollapsingHeader("Character"))
	{
		//ImGui::Text(std::format("Move Speed: {:0.1f} m/s", 5).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if(ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}
}

void Character::SetIsDead(bool dead)
{
	m_IsDead = dead;
}
