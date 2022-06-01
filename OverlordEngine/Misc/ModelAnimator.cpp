#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter},
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	TODO_W7_();

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		auto elapsedSec = sceneContext.pGameTime->GetElapsed();
		auto durationInTicks =m_CurrentClip.duration;
		auto TicksPerSeconds = m_CurrentClip.ticksPerSecond;
		//m_TickCount = 100;
		//m_AnimationSpeed = 1.0f;
		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = elapsedSec * TicksPerSeconds * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = fmod(passedTicks, durationInTicks);
		//2. 
		//IF m_Reversed is true
		if (m_Reversed)
		{
			//	Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;
			//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if(m_TickCount<0)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		else
		{
			//ELSE
			//	Add passedTicks to m_TickCount
			m_TickCount += passedTicks;
			//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if(m_TickCount>durationInTicks)
			{
				m_TickCount -= durationInTicks;
			}
		}
		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//auto keyIterator = m_CurrentClip.keys.begin();

		for(auto i =0 ;i!=m_CurrentClip.keys.size();++i)
		{
			if(m_TickCount>m_CurrentClip.keys[i].tick)
			{
				keyA = m_CurrentClip.keys[i];
				keyB = m_CurrentClip.keys[(i + 1) % m_CurrentClip.keys.size()];
			}
		}

		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//BLEND
		float blend = (keyB.tick - m_TickCount) / (m_CurrentClip.duration/m_CurrentClip.keys.size());

		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (size_t i = 0; i < keyA.boneTransforms.size(); ++i)
		{
			//	Retrieve the transform from keyA (transformA)
			auto transformA = keyA.boneTransforms[i];
			auto transformMatA = XMLoadFloat4x4(&transformA);
			//	auto transformA = ...
			// 	Retrieve the transform from keyB (transformB)
			auto transformB = keyB.boneTransforms[i];
			auto transformMatB = XMLoadFloat4x4(&transformB);
			//	Decompose both transforms
			XMVECTOR translationA, rotA, scaleA;
			XMMatrixDecompose(&scaleA, &rotA, &translationA, transformMatA);
			XMVECTOR translationB, rotB, scaleB;
			XMMatrixDecompose(&scaleB, &rotB, &translationB, transformMatB);

			
			//	Lerp between all the transformations (Position, Scale, Rotation)
			auto lerpPos = XMVectorLerp(translationB, translationA, blend);
			auto lerpRot =	XMQuaternionSlerp(rotB, rotA, blend);
			auto lerpScale = XMVectorLerp(scaleB, scaleA, blend);
			//	Compose a transformation matrix with the lerp-results
			XMMATRIX result = 
				XMMatrixScalingFromVector(lerpScale) * 
				XMMatrixRotationQuaternion(lerpRot) *
				XMMatrixTranslationFromVector(lerpPos);
			//	Add the resulting matrix to the m_Transforms vector
			XMFLOAT4X4 finalMatrix;
			XMStoreFloat4x4(&finalMatrix, result);
			m_Transforms.push_back(finalMatrix);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	TODO_W7_()
		//Set m_ClipSet to false'
		m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for(auto& clip:m_pMeshFilter->m_AnimationClips)
	{
		//check clip name
		if(clip.name == clipName)
		{
			//call set animation clip
			SetAnimation(clip);
		}
		else
		{
			Reset();
			Logger::LogWarning(L"clip name not matching");
		}
	}
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	TODO_W7_()
		//Set m_ClipSet to false
		m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if(clipNumber<m_pMeshFilter->m_AnimationClips.size())
	{
		AnimationClip clip;
		clip = m_pMeshFilter->m_AnimationClips.at(clipNumber);
		SetAnimation(clip);
	}
	else
	{
		Reset();
		Logger::LogWarning(L"Animation clip size is smaller than the clipnumber.");
		return;
	}
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	TODO_W7_()
		//Set m_ClipSet to true
		m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;

	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	TODO_W7_()
		//If pause is true, set m_IsPlaying to false
		if (pause) m_IsPlaying = false;
	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;

	if(m_ClipSet)
	{
		//If m_ClipSet is true
		
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		auto transforms = m_CurrentClip.keys.begin()->boneTransforms;
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assi
		m_Transforms.assign(transforms.begin(), transforms.end());
	}
	else
	{
		//Else
	//	Create an IdentityMatrix
		XMFLOAT4X4 iMatrix =
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
		UINT count = m_pMeshFilter->m_BoneCount;
		m_Transforms.assign(count,iMatrix);
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
	}
	
}
