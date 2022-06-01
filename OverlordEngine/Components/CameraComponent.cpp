#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true),
	m_winHeight{},
	m_winWidth{}
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}

void CameraComponent::Update(const SceneContext& sceneContext)
{
	m_winHeight = sceneContext.windowHeight;
	m_winWidth = sceneContext.windowWidth;
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active?this:nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups, XMFLOAT3 position) const
{
	TODO_W5(L"Implement Picking Logic")

		// btw -1 and 1
		//1.transform mouse pos to ndc space
		auto getMouse = m_pScene->GetSceneContext().pInput->GetMousePosition();

	
	//MOUSE POS IN XMFLOAT
	XMFLOAT3 mousePos = XMFLOAT3{(float) getMouse.x,(float)getMouse.y,0 };
	if (position.x != 0 && position.y != 0 && position.z != 0)
	{
		mousePos = position;
	}
	//GET WINDOW WIDTH AND HEIGHT

	//GET WINDOW SIZE
	XMFLOAT3 nearpoint{};
	XMFLOAT3 farPoint{};

	// MOUSE POSE IN NDC
	XMFLOAT2 mousePosNDC = XMFLOAT2{ (mousePos.x - m_winWidth / 2) / (m_winWidth / 2),((m_winHeight / 2) - mousePos.y) / (m_winHeight / 2) };
	//calculate near and far

	// VIEW PROJECTION (VP)
	auto tempnear = XMFLOAT3{ mousePosNDC.x, mousePosNDC.y, 0 };
	auto tempFar = XMFLOAT3{ mousePosNDC.x, mousePosNDC.y, 1 };
	// near and far plane=> create two coord for muse =>near SEE SLIDE
	
	auto nearPointTransformed = XMVector3TransformCoord(XMLoadFloat3(&tempnear), XMLoadFloat4x4(&m_ViewProjectionInverse));
	auto farPointTransformed = XMVector3TransformCoord(XMLoadFloat3(&tempFar), XMLoadFloat4x4(&m_ViewProjectionInverse));
	//raycast using near and far plane
	XMFLOAT3 direction;

	//stor nearpoint and far point
	XMStoreFloat3(&nearpoint, nearPointTransformed);
	XMStoreFloat3(&farPoint, farPointTransformed);

	//GET THE DIRECTION USING FAR POINT AND NEARPOINT
	XMStoreFloat3(&direction, farPointTransformed - nearPointTransformed);
	auto rayStart = PxVec3(nearpoint.x, nearpoint.y, nearpoint.z);
	auto rayDirection = PxVec3(direction.x, direction.y, direction.z);
	rayDirection.normalize();

	PxQueryFilterData filterData{};
	filterData.data.word0 = ~UINT(ignoreGroups);

	PxRaycastBuffer hit{};
	auto physProxy = GetScene()->GetPhysxProxy();
	if (physProxy->Raycast(rayStart, rayDirection, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData))
	{
		//if hit
		BaseComponent* component = nullptr;
		if(hit.hasBlock)
		{
			//if convert to base component
			component =(BaseComponent*)hit.block.actor->userData;

			//return the gameobject of the component
			return component->GetGameObject();
		}
	}
	//returns boolean//GetScene()->GetPhysxProxy()->Raycast(origin,normalized direction,distance=>maxfloat / dist(near,far),Raycastcall back hitinfo,hitflags,ignore certain actors)
	return nullptr;
}