#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CAnimator2D::CAnimator2D() :
	CComponent(E_ComponentType::Animator2D),
	m_pCurAnimation(nullptr),
	m_eAnimationState(E_AnimationState::Once)
{
}

CAnimator2D::~CAnimator2D()
{
	Safe_Delete_UnorderedMap(m_unmapAnim);
}

void CAnimator2D::LateUpdate()
{
	if (nullptr == m_pCurAnimation)
		return;

	if (E_AnimationState::Loop == GetAnimationState() && m_pCurAnimation->IsFinished())
		m_pCurAnimation->Reset();
	m_pCurAnimation->LateUpdate();
}

void CAnimator2D::UpdateData()
{
	if (m_pCurAnimation)
		m_pCurAnimation->UpdateData();
}

void CAnimator2D::Play(const tstring& _strAnimName, E_AnimationState _eAnimationState)
{
	m_eAnimationState = _eAnimationState;
	m_pCurAnimation = FindAnimation(_strAnimName);
	assert(m_pCurAnimation);
}

void CAnimator2D::LoadAnimation(const tstring& _strRelativeFilePath)
{
	CAnimation2D* pAnim = new CAnimation2D;
	pAnim->Load(_strRelativeFilePath);
	m_unmapAnim.insert(std::make_pair(pAnim->GetName(), pAnim));
}

CAnimation2D* CAnimator2D::FindAnimation(const tstring& _strName)
{
	unordered_map<tstring, CAnimation2D*>::iterator iter = m_unmapAnim.find(_strName);

	if (m_unmapAnim.end() == iter)
		return nullptr;
	return iter->second;
}

void CAnimator2D::CreateAnimation(TAnimation2DDesc& _tAnimation2DDesc)
{
	CAnimation2D* pAnim = FindAnimation(_tAnimation2DDesc.strName);
	assert(!pAnim && _T("애니메이션이 이미 존재함")); // 존재 시 오류

	pAnim = new CAnimation2D;
	pAnim->SetName(_tAnimation2DDesc.strName);
	pAnim->Create(_tAnimation2DDesc);

	m_unmapAnim.insert(std::make_pair(_tAnimation2DDesc.strName, pAnim));
}

void CAnimator2D::Clear()
{
	static const CConstBuffer* pBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Animation2D_Data);
	TAnimation2DData tData = {};
	pBuffer->SetData(&tData);
	pBuffer->UpdateData();
}