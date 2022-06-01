#pragma once
class ColorMaterial;
class BoneObject;
class SoftwareSkinningScene_1 :
    public GameScene
{
public:
	SoftwareSkinningScene_1():GameScene(L"SoftwareSkinningScene_1"){}
	~SoftwareSkinningScene_1() override = default;

	SoftwareSkinningScene_1(const SoftwareSkinningScene_1 & other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1 && other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1 & other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1 && other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;
private:
	BoneObject* m_pBone{},* m_pBone1{};
	ColorMaterial* pColorMat{};
	float m_BoneRotation{};
	int m_RotationSign{ 1 };
	const float speed =30.f;

	enum class BoneRotation
	{
		up,
		down
	};
	BoneRotation rotateState;
};

