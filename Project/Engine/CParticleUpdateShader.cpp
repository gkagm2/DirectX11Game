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
	m_pSharedBuffer->Create(E_StructuredBufferType::Dual, sizeof(TSharedParticleData), 1);

	//m_tParticleData.fStartSpeed(0.f);
	//m_tParticleData.fEndSpeed(0.f);
	//m_tParticleData.fMinLifeTime(1.f);
	//m_tParticleData.fMaxLifeTime(4.f);
	//m_tParticleData.iSpawnCntPerSec

}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

void CParticleUpdateShader::UpdateData()
{
	// 파티클 정보 바인딩
	UINT iRegisterNum = 0;
	m_pParticleBuffer->UpdateDataRW(iRegisterNum);

	// 공유 파티클 정보 바인딩
	TSharedParticleData tSharedData = {};
	tSharedData.m_iSpawnCount = m_iSpawnCount;
	//tSharedData.m_iCurActivedCount = 

	UINT iElementCnt = 1, iSharedBuffRegisterNum = 1;
	m_pSharedBuffer->SetData(&tSharedData, iElementCnt);
	m_pSharedBuffer->UpdateDataRW(iSharedBuffRegisterNum);


	m_tInfo.iArr[0] = m_pParticleBuffer->GetElementCount(); // 파티클의 최대 개수
	m_tInfo.iArr[1] = (int)m_eShape;
	m_tInfo.iArr[2] = (int)m_bGravityEnable;

	m_tInfo.fArr[0] = m_fStartSpeed;
	m_tInfo.fArr[1] = m_fEndSpeed;
	m_tInfo.fArr[2] = m_fMinLifeTime;
	m_tInfo.fArr[3] = m_fMaxLifeTime;

	m_tInfo.v4Arr[0] = m_vObjectPos;
	m_tInfo.v4Arr[1] = m_vRadius; // Vector3->Vector4

	g_pMtrlBuffer->SetData(&m_tInfo);
	g_pMtrlBuffer->UpdateData(E_ShaderStage::Compute);
}

void CParticleUpdateShader::Clear()
{
	m_pParticleBuffer->ClearRW();
	m_pSharedBuffer->ClearRW();
	g_pMtrlBuffer->Clear(E_ShaderStage::Compute);
}

void CParticleUpdateShader::Excute()
{
	UINT iElementCnt = m_pParticleBuffer->GetElementCount();
	UINT iThreadX = (iElementCnt / GetThreadX()) + 1;
	Dispatch(iThreadX, 1, 1);
}