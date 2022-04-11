#include "pch.h"
#include "CAnimator3D.h"

#include "CTimeManager.h"
#include "CMeshRenderer.h"
#include "CStructuredBuffer.h"
#include "CResourceManager.h"
#include "CAnimation3DShader.h"
#include "CKeyManager.h"

CAnimator3D::CAnimator3D() :
	CComponent(E_ComponentType::Animator3D),
	m_iCurClip(0),
	m_dCurTime(0.),
	m_iFrameCount(30),
	m_pBoneFinalMatBuffer(nullptr),
	m_bFinalMatUpdate(false),
	m_iFrameIdx(0),
	m_iNextFrameIdx(0),
	m_fRatio(0.f)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMatBuffer);
}


void CAnimator3D::Update()
{
}

void CAnimator3D::LateUpdate()
{
}

void CAnimator3D::FinalUpdate()
{
	m_dCurTime = 0.f;
	// ���� ������� Clip �� �ð��� �����Ѵ�.
	m_vecClipUpdateTime[m_iCurClip] += DT;

	if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
		m_vecClipUpdateTime[m_iCurClip] = 0.f;

	m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// ���� ������ �ε���
	if (m_iFrameIdx >= m_pVecClip->at(0).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// ���̸� ���� �ε����� ����
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// �����Ӱ��� �ð��� ���� ������ �����ش�.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	// ��ǻƮ ���̴� ���꿩��
	m_bFinalMatUpdate = false;
}

bool CAnimator3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	// TODO (Jang) : ����
	return true;
}

bool CAnimator3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	// TODO (Jang) : ����
	return true;
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}

void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_Animation3DShader).Get();

		// Bone Data
		SharedPtr<CMesh> pMesh = MeshRenderer()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		// ������Ʈ ���̴� ����
		pUpdateShader->Excute();

		m_bFinalMatUpdate = true;
	}

	// t30 �������Ϳ� ������� ������(��������) ���ε�		
	m_pBoneFinalMatBuffer->UpdateData(30, E_ShaderStage::Vertex);
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRenderer()->GetMtrlCount();
	SharedPtr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i) {
		pMtrl = MeshRenderer()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;
		pMtrl->SetAnim3D(false); // Animation Mesh �˸���
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::check_mesh(SharedPtr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount) {
		m_pBoneFinalMatBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(Matrix), iBoneCount, false, nullptr);
	}
}