#pragma once
//#include "C:\Users\hemt1\source\repos\PingPong\OverlordEngine_x64\OverlordEngine\Misc\Material.h"
class UberMaterial :
    public Material<UberMaterial>
{
public:
	UberMaterial();
	~UberMaterial() override = default;

	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;


	void SetUberMaterial(const std::wstring& assetfile);

	XMFLOAT3 m_DiffuseColor;
	bool m_UseOpacity;
protected:
	void InitializeEffectVariables() override;

private:
	
};

