#pragma once
//#include "C:\Users\hemt1\source\repos\Graphics\OverlordEngine_x64\OverlordEngine\Scenegraph\GameScene.h"
class ColorMaterial;
class BoneObject;
class MeshDrawComponent;
class SoftwareSkinningScene_3 :
	public GameScene
{

	

public:
	SoftwareSkinningScene_3() :GameScene(L"SoftwareSkinningScene_3") {}
	~SoftwareSkinningScene_3() override = default;

	SoftwareSkinningScene_3(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

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
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float B0, float B1) :
			transformedVertex{ position,normal,color },
			originalVertex{ position,normal,color },
			blendWeight0{B0},
			blendWeight1{B1}{}


		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
		float blendWeight0;
		float blendWeight1;
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};




};

