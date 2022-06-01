#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial()
	:Material<UberMaterial>(L"Effects/UberShader.fx")
   ,m_UseOpacity{false}
{
	
}

void UberMaterial::SetUberMaterial(const std::wstring&)
{
	InitializeEffectVariables();
}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gColorAmbient", {0.2f,0.2f,0.2f});
	SetVariable_Vector(L"gLightDirection", { -0.577f,-0.577f,0.577f });
	SetVariable_Vector(L"gColorDiffuse", m_DiffuseColor= { 0.520f,0.020f,0.020f });
	SetVariable_Scalar(L"gTextureOpacityIntensity", m_UseOpacity);
	SetVariable_Texture(L"gTextureDiffuse", ContentManager::Load<TextureData>(L"Textures/Skulls_Diffusemap.tga"));
	SetVariable_Texture(L"gTextureNormal", ContentManager::Load<TextureData>(L"Textures/Skulls_Normalmap.tga"));
	//SetVariable_Texture(L"gTextureOpacity", ContentManager::Load<TextureData>(L"Textures/Skulls_Heightmap.tga"));
	SetVariable_Texture(L"gCubeEnvironment", ContentManager::Load<TextureData>(L"Textures/Sunol_Cubemap.dds"));
	SetVariable_Texture(L"gTextureSpecularIntensity", ContentManager::Load<TextureData>(L"Textures/Skulls_Heightmap.tga"));
	//gTextureSpecularIntensity	
}
