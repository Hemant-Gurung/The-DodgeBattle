#pragma once

class SpikyMaterial;

class SpikyScene:public GameScene
{
public:
	SpikyScene():GameScene(L"SpikyScene"){};
	~SpikyScene() = default;
	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
private:
	GameObject* m_pSpikySphere;
	float m_Rotate;
	SpikyMaterial* m_pMaterial;

};

