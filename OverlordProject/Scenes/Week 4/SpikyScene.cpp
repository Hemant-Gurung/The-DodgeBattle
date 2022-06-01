#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

void SpikyScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;

	m_SceneContext.settings.enableOnGUI = true;

	//initialize rotate
	m_Rotate = 0;

	// create game object
	m_pSpikySphere = new GameObject();

	auto SpikyModel = new ModelComponent(L"Meshes/OctaSphere.ovm");

	//create material to add
	m_pMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();

	m_pMaterial->SetMaterial();
	//add material to the model
	SpikyModel->SetMaterial(m_pMaterial);

	//add component to the gameobject
	m_pSpikySphere->AddComponent(SpikyModel);


	//set initial positions
	m_pSpikySphere->GetTransform()->Scale(15.f, 15.f, 15.f);
	m_pSpikySphere->GetTransform()->Translate(0.f, 5.f, 0.f);

	GameScene::AddChild(m_pSpikySphere);
}

void SpikyScene::Update()
{
	m_Rotate += 10.f * GetSceneContext().pGameTime->GetElapsed();
	m_pSpikySphere->GetTransform()->Rotate(0, m_Rotate, 0);
}

void SpikyScene::Draw()
{
	
}

void SpikyScene::OnGUI()
{
	m_pMaterial->DrawImGui();
}
