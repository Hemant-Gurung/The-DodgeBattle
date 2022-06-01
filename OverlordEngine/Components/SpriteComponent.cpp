#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color),
	m_Rotate(),
	m_Scale()
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
	
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Draw(const SceneContext& sceneContext)
{
	if (!m_pTexture)
		return;

	TODO_W4(L"Draw the sprite with SpriteRenderer::Draw")
		//append sprite
	
	m_Position= XMFLOAT2(m_pGameObject->GetTransform()->GetPosition().x,m_pGameObject->GetTransform()->GetPosition().y);
	//m_Pivot = XMFLOAT2(m_pGameObject->GetTransform()->GetPosition().x *m_Scale.x, m_pGameObject->GetTransform()->GetPosition().y*m_Scale.y);
	SpriteRenderer::Get()->AppendSprite(m_pTexture,m_Position, m_Color,m_Pivot,m_Scale,m_Rotation.z);
	
	//Here you need to draw the SpriteComponent using the Draw of the sprite renderer
	
	// The sprite renderer is a singleton

	//MathHelper::QuaternionToEuler();
	// you will need to position (X&Y should be in screenspace, Z contains the depth between [0,1]), the rotation and the scale from the owning GameObject
	
	SpriteRenderer::Get()->Draw(sceneContext);
	// You can use the MathHelper::QuaternionToEuler function to help you with the z rotation 
}

void SpriteComponent::Update(const SceneContext& /*scenecontext*/)
{
	m_Scale = XMFLOAT2(m_pGameObject->GetTransform()->GetScale().x, m_pGameObject->GetTransform()->GetScale().y);
	m_Rotation = MathHelper::QuaternionToEuler(m_pGameObject->GetTransform()->GetRotation());
	m_Color = GetColor();
	//m_Rotate += scenecontext.pGameTime->GetElapsed() *50;
	//m_pGameObject->GetTransform()->Rotate(m_Rotate,0, 0);
}
