#include "stdafx.h"
#include "ModelTestScene.h"

#include "Materials/DiffuseMaterial.h"


void ModelTestScene::Initialize()
{
	//bone rotation state
	
	//create ground plane
	GameSceneExt::CreatePhysXGroundPlane(*this);
	GetSceneSettings().drawPhysXDebug = false;
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawUserDebug = false;

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	
	//initialize the gameobject
	m_pChair = new GameObject();


	//DIFFUSE MATERIAL
	DiffuseMaterial* pDiffuse = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();

	//load model
	const auto chairModel = new ModelComponent(L"Meshes/chair.ovm");

	//set material to chairmodel
	pDiffuse->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	//set the diffuse material
	chairModel->SetMaterial(pDiffuse);

	//add chair material component to the chair
	m_pChair->AddComponent(chairModel);

	//create new instance of material

	auto chairMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/chair.ovpc");
	
	auto chairComponent = new RigidBodyComponent(false);

	//add collider
	chairComponent->AddCollider(PxConvexMeshGeometry(chairMesh), *pDefaultMaterial);

	//set color

	//add component to chair
	m_pChair->AddComponent(chairComponent);

	//transform
	m_pChair->GetTransform()->Translate(0, 4, 0);

	//add the chair gameobject to the gamescene
	GameScene::AddChild(m_pChair);
	
}

void ModelTestScene::Update()
{
	
}

void ModelTestScene::Draw()
{

}
