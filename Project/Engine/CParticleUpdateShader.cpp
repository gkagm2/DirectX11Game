#include "pch.h"
#include "CParticleUpdateShader.h"
#include "CStructuredBuffer.h"
#include "CConstBuffer.h"

CParticleUpdateShader::CParticleUpdateShader() :
	CComputeShader(1024, 1, 1),
//	m_tParticleData{},
	m_pParticleBuffer(nullptr),
	m_pSharedBuffer(nullptr),
	m_fStartSpeed(0.f),
	m_fEndSpeed(0.f),
	m_fMinLifeTime(1.f),
	m_fMaxLifeTime(4.f),
	m_iSpawnCount(0),
	m_eShape(E_ParticleShape::Circle_Combine),
	m_bGravityEnable(false)
{
	m_pSharedBuffer = make_unique<CStructuredBuffer>();
	m_pSharedBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TSharedParticleData), 1, true, nullptr);
	}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

void CParticleUpdateShader::UpdateData()
{
	// ��ƼŬ ���� ���ε�
	UINT iRegisterNum = 0;
	m_pParticleBuffer->UpdateDataCS(iRegisterNum);

	// ���� ��ƼŬ ���� ���ε�
	TSharedParticleData tSharedData = {};
	tSharedData.m_iSpawnCount = m_iSpawnCount;
	//tSharedData.m_iCurActivedCount = 

	UINT iElementCnt = 1, iSharedBuffRegisterNum = 1;
	m_pSharedBuffer->SetData(&tSharedData, sizeof(TSharedParticleData) * iElementCnt);
	m_pSharedBuffer->UpdateDataCS(iSharedBuffRegisterNum);


	m_tInfo.iArr[0] = m_pParticleBuffer->GetElementCount(); // ��ƼŬ�� �ִ� ����
	m_tInfo.iArr[1] = (int)m_eShape;
	m_tInfo.iArr[2] = (int)m_bGravityEnable;

	m_tInfo.fArr[0] = m_fStartSpeed;
	m_tInfo.fArr[1] = m_fEndSpeed;
	m_tInfo.fArr[2] = m_fMinLifeTime;
	m_tInfo.fArr[3] = m_fMaxLifeTime;

	m_tInfo.v4Arr[0] = m_vObjectPos;
	m_tInfo.v4Arr[1] = m_vRadius; // Vector3->Vector4
}

void CParticleUpdateShader::Clear()
{
	m_pParticleBuffer->ClearRW();
	m_pSharedBuffer->ClearRW();
}

void CParticleUpdateShader::Excute()
{
	UINT iElementCnt = m_pParticleBuffer->GetElementCount();
	UINT iThreadX = (iElementCnt / GetGroupPerThreadX()) + 1;
	Dispatch(iThreadX, 1, 1);
}