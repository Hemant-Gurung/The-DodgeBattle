#pragma once
//#include "C:\Users\hemt1\source\repos\Graphics\OverlordEngine_x64\OverlordEngine\Scenegraph\GameObject.h"
class BoneObject :
    public GameObject
{
public:
	BoneObject(BaseMaterial* pMaterial, float length );
	~BoneObject() override = default;

	BoneObject(const BoneObject & other) = delete;
	BoneObject(BoneObject && other) noexcept = delete;
	BoneObject& operator=(const BoneObject & other) = delete;
	BoneObject& operator=(BoneObject && other) noexcept = delete;

	void AddBone(BoneObject* PBone);

	const XMFLOAT4X4& GetBindPose() const { return m_BindPose; }
	void CalculateBindPose();
protected:
	void Initialize(const SceneContext&) override;
private:
	float m_Length{};
	BaseMaterial* m_pMaterial{};

	XMFLOAT4X4 m_BindPose{};

};

