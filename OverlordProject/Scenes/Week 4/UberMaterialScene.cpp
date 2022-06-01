#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/ColorMaterial.h"
#include "Materials/UberMaterial.h"

void UberMaterialScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;
	//GetSceneSettings().enableOnGUI = true;
	m_SceneContext.settings.enableOnGUI = true;
	
	//create game object
	m_pSphere = new GameObject();

	//rotate
	m_rotate = 0;
	//auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	//get sphere model using model component
	auto sphereModel = new ModelComponent(L"Meshes/Sphere.ovm");

	//create color material
	m_pMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	//use this
	
	//auto chairMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Sphere.ovpt");
	/*pMaterial->m_DiffuseColor.x = color[0];
	pMaterial->m_DiffuseColor.y = color[1];
	pMaterial->m_DiffuseColor.z = color[2];*/
	//MaterialManager::CreateMaterial<>()
	//add material to the sphere
	m_pMaterial->SetUberMaterial(L"Effects/UberShader.fx");
	
	//set material
	sphereModel->SetMaterial(m_pMaterial);

	//add component to the model
	m_pSphere->AddComponent(sphereModel);

	//set initial positions
	m_pSphere->GetTransform()->Scale(20.f, 20.f, 20.f);
	m_pSphere->GetTransform()->Translate(0.f, 5.f, 0.f);

	
	
	//add the model to the scene
	GameScene::AddChild(m_pSphere);
}

void UberMaterialScene::Update()
{
	//
	m_rotate += 10.f* GetSceneContext().pGameTime->GetElapsed();
	m_pSphere->GetTransform()->Rotate(0,m_rotate, 0);
}

void UberMaterialScene::Draw()
{
	
}

void UberMaterialScene::OnGUI()
{
	m_pMaterial->DrawImGui();
}

void UberMaterialScene::InitializeEffectVariables()
{

}

