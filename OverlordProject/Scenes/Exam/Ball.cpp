#include "stdafx.h"
#include "Ball.h"

#include "Prefabs/SpherePrefab.h"

Ball::Ball()
	:m_Ball()
{
	//MakeBall();
	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.7f, 1.0f, 0.0f);
	m_Ball = AddChild(new SpherePrefab(5.f, 10, XMFLOAT4{ Colors::Green }));
	m_pBallActor = m_Ball->AddComponent(new RigidBodyComponent(false));
	m_pBallActor->AddCollider(PxSphereGeometry(5.f), *pDefaultMaterial);
}

//Ball::~Ball()
//{
//	delete m_Ball;
//}

void Ball::Initialize(const SceneContext& scene_context)
{
	GameObject::Initialize(scene_context);
}

void Ball::Update(const SceneContext& scene_context)
{
	GameObject::Update(scene_context);
}

GameObject* Ball::MakeBall()
{
	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.7f, 1.0f, 0.0f);
	m_Ball = AddChild(new SpherePrefab(5.f, 10, XMFLOAT4{ Colors::Green }));
	m_pBallActor = m_Ball->AddComponent(new RigidBodyComponent(false));
	m_pBallActor->AddCollider(PxSphereGeometry(5.f), *pDefaultMaterial);
	return m_Ball;
}

void Ball::ShootBall(XMFLOAT3 direction)
{
	m_Ball->GetComponent<RigidBodyComponent>()->AddForce(direction, PxForceMode::eIMPULSE);

}

