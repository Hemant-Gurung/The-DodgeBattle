#include "stdafx.h"
#include "DiffuseMaterial_Shadow.h"

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow.fx")
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow::OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* /*pModel*/) const
{
	/*
	 * TODO_W8
	 * Update The Shader Variables
	 * 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	 * 	LightWVP = model_world * light_viewprojection
	 * 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	 *
	 * 2. Update the ShadowMap texture
	 *
	 * 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	*/
	//Update Shadow Variables
	//const auto pShadowMapRenderer = ShadowMapRenderer::Get();
	////...
	//auto LightWVP = XMMatrixMultiply(XMLoadFloat4x4(&pModel->GetTransform()->GetWorld()), XMLoadFloat4x4(&pShadowMapRenderer->GetLightVP()));

	////uupdate light
	////SetVariable_MatrixArray(, LightWVP.r->m128_f32, )
	//XMFLOAT4X4 wvp;
	//XMStoreFloat4x4(&wvp, XMLoadFloat4x4(&pModel->GetTransform()->GetWorld()) * XMLoadFloat4x4(&pShadowMapRenderer->GetLightVP()));

	//SetVariable_Matrix(L"gWorldViewProj_Light", &wvp._11);

	//auto shadowMap = pShadowMapRenderer->GetShadowMap();

	//if (shadowMap != nullptr)
	//{
	//	SetVariable_Texture(L"gShadowMap", shadowMap);
	//}
	//Update Shadow Variables
	//const auto pShadowMapRenderer = ShadowMapRenderer::Get();
	//...
}
