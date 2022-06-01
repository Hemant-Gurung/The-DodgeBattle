#pragma once
class ModelTestScene:public GameScene
{

public:
	ModelTestScene():GameScene(L"ModelTextScene"){};
	~ModelTestScene() = default;

	ModelTestScene(const ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	GameObject* m_pChair{};
};

