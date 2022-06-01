#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length = 5.f)
	:m_pMaterial{pMaterial},
	m_Length{length},
     m_BindPose{}
{

}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(XMVECTOR{ m_Length,0,0 });
	this->AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	auto world = GetTransform()->GetWorld();
	auto wInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&world));
	XMStoreFloat4x4(&m_BindPose, wInv);

	auto a = GetChildren<BoneObject>();
	for (BoneObject* bone : GetChildren<BoneObject>())
	{
		if (bone != nullptr)
		{
			bone->CalculateBindPose();
		}
	}
}

void BoneObject::Initialize(const SceneContext& /*sceneContext*/)
{
	//CREATE GAME OBJECT
	GameObject* pEmpty = new GameObject();


	//ADD MODELCOMPONENT TO PEMPTY
	auto pModel = new ModelComponent(L"Meshes/Bone.ovm");

	//ASSIGN BONEOBJECTS MATERIAL TO PMODEL
	pModel->SetMaterial(m_pMaterial);

	//ADD MODEL TO THE GAMEOBJECT
	pEmpty->AddComponent(pModel);

	//ROTATE -90 DEGREES
	pEmpty->GetTransform()->Rotate(0, -90, 0);

	//UNIFORMLY SCALE PEMPTY TO MATCH THE BONE OBJECTS LENGTH
	pEmpty->GetTransform()->Scale(XMFLOAT3{ m_Length,m_Length,m_Length });

	//ADD pEmpty as child
	this->AddChild(pEmpty);
}
