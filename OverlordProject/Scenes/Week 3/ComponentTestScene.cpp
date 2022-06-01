#include "stdafx.h"
#include "ComponentTestScene.h"
#include "Prefabs/SpherePrefab.h"

void ComponentTestScene::Initialize()
{
	

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);
	
	
	// sphere 1
	auto pSphereObject =  AddChild(new SpherePrefab(1.f,10,XMFLOAT4{ Colors::Blue }));
	auto pSphereActor =pSphereObject->AddComponent(new RigidBodyComponent());
  /* UINT colloderId*/  pSphereActor->AddCollider(PxSphereGeometry(1.f),*pDefaultMaterial);
	//pSphereActor->GetCollider(colliderId)
	pSphereActor->GetTransform()->Translate(0.f, 10.f, 0.f);

	//collision group
	pSphereActor->SetCollisionGroup(CollisionGroup::Group1);

	// sphere 2  (Groupd 0)
	pSphereObject = AddChild(new SpherePrefab(1.f,10,XMFLOAT4{Colors::Green}));
	pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	 pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	//pSphereActor->GetCollider(colliderId)
	pSphereActor->GetTransform()->Translate(0.f, 40.f, 0.f);

	//ignore group
	pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group1);

	// sphere 1 Group 0
	 pSphereObject = AddChild(new SpherePrefab(1.f,10,XMFLOAT4{Colors::Red}));
	 pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	/* UINT colloderId*/  pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	//pSphereActor->GetCollider(colliderId)
	pSphereActor->GetTransform()->Translate(0.f, 50.f, 0.f);

	
}
