#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

void SoftwareSkinningScene_1::Initialize()
{
	//INITIALIZE BONESTATE
	rotateState = BoneRotation::up;
	//MATERIAL
	pColorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	GameObject* pRoot = new GameObject();
	m_SceneContext.settings.enableOnGUI = true;

	
	//create bone
	m_pBone = new BoneObject(pColorMat, 15.f);
	

	
	//ADD THIS BONE AS CHILD
	pRoot->AddChild(m_pBone);

	//SECOND TIME
	m_pBone1 = new BoneObject(pColorMat, 15.f);

	//add bone1 as a child of bone
	m_pBone->AddBone(m_pBone1);

	
	GameScene::AddChild(pRoot);
}

void SoftwareSkinningScene_1::Update()
{
	m_pBone->GetTransform()->Rotate(XMFLOAT3{ 0,0,m_BoneRotation });
	m_pBone1->GetTransform()->Rotate(XMFLOAT3{ 0,0,-m_BoneRotation * 2.f });

	switch (rotateState)
	{
	case SoftwareSkinningScene_1::BoneRotation::up:
		m_BoneRotation += speed * m_SceneContext.pGameTime->GetElapsed();
		if (m_BoneRotation > 45)
		{
			rotateState = BoneRotation::down;
		}
		break;
	case SoftwareSkinningScene_1::BoneRotation::down:
		m_BoneRotation -= speed * m_SceneContext.pGameTime->GetElapsed();
		if (m_BoneRotation <-45)
		{
			rotateState = BoneRotation::up;
		}
		break;
	default:
		break;
	}
	
	
}

void SoftwareSkinningScene_1::OnGUI()
{
	pColorMat->DrawImGui();
	
}
