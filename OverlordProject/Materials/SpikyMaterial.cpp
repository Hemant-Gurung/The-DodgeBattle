#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial()
	:Material<SpikyMaterial>(L"Effects/SpikyShader.fx")
{
	
}

void SpikyMaterial::SetMaterial()
{
	InitializeEffectVariables();
}


void SpikyMaterial::InitializeEffectVariables()
{
	m_SpikeLength = 0.5f;
	SetVariable_Scalar(L"gSpikeLength", m_SpikeLength);
	
}



