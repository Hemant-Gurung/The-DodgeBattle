#pragma once
class UberMaterial;

class UberMaterialScene:public GameScene
{
public:

	UberMaterialScene() :GameScene(L"UberMaterialScene"){};
	~UberMaterialScene() = default;

	UberMaterialScene(const UberMaterialScene& other) = delete;
	UberMaterialScene(UberMaterialScene&& other) noexcept = delete;
	UberMaterialScene& operator=(const UberMaterialScene& other) = delete;
	UberMaterialScene& operator=(UberMaterialScene&& other) noexcept = delete;	

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
private:
	void InitializeEffectVariables() ;
	GameObject* m_pSphere;
	float m_rotate;
	UberMaterial* m_pMaterial;
};

