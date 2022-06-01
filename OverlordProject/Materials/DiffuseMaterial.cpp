#include "stdafx.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial():
	Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fx")
{
	m_pAssetfile = L"Effects / PosNormTex3D.fx";
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetfile)
{
	
	//get the Texture data using content manager
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetfile);

	//set the varaible in effect file gDiffuseMap
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);

	// initialize the effect variable optional for default technique
	InitializeEffectVariables();
}

void DiffuseMaterial::InitializeEffectVariables()
{
	SetTechnique(L"Default");

}