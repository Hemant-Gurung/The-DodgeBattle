#pragma once

class ColorMaterial;
class BoneObject;
class MeshDrawComponent;
class SoftwareSkinningScene_2 :
    public GameScene
{
public:
	SoftwareSkinningScene_2() :GameScene(L"SoftwareSkinningScene_2") {}
	~SoftwareSkinningScene_2() override = default;

	SoftwareSkinningScene_2(const SoftwareSkinningScene_2 & other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2 && other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2 & other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2 && other) noexcept = delete;

	enum class BoneRotation
	{
		up,
		down
	};
	BoneRotation rotateState;


protected:

	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:

	
	BoneObject* m_pBone0{}, * m_pBone1{};
	ColorMaterial* pColorMat{};
	float m_BoneRotation{};
	int m_RotationSign{ 1 };
	const float speed = 30.f;
	XMFLOAT4X4 bindpose0;
	XMFLOAT4X4 bindpose1;

//	GameObject* pBoxDrawer;
	/*XMFLOAT3 m_BoneRotation{}, m_Bone1Rotation{};
	bool m_AutoRotate{ true };*/

	// STRUCT
	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color):
			transformedVertex{position,normal,color},
			originalVertex{position,normal,color}{}

			VertexPosNormCol transformedVertex{};
			VertexPosNormCol originalVertex{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};


};

