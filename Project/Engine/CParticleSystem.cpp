#include "pch.h"
#include "CParticleSystem.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"
#include "CMaterial.h"

#include "CTimeManager.h"

const int g_testParticleCnt = 30;
CParticleSystem::CParticleSystem() :
	CComponent(E_ComponentType::ParticleSystem),
	m_pParticleBuffer(nullptr),
	m_fStartSpeed(500.f),
	m_fEndSpeed(50.f),
	m_iSpawnCntPerSec(10),
	m_iMaxParticleCount(100),
	m_fAccTime(0.f)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_PointMesh);
	m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_ParticleMtrl);
	m_pUpdateShader = dynamic_cast<CParticleUpdateShader*>(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_ParticleUpdateShader).Get());

	assert(m_pMesh.Get());
	assert(m_pMaterial.Get());

	m_vStartScale = Vector4(100.f, 100.f, 1.f, 1.f);
	m_vEndScale = Vector4(5.f, 5.f, 0.f, 1.f);

	m_vStartColor = Vector4(0.1f, 0.1f, 0.1f, 1.f);
	m_vEndColor = Vector4(1.f, 1.f, 1.f, 1.f);
}

CParticleSystem::~CParticleSystem()
{
	if (m_pParticleBuffer)
		delete m_pParticleBuffer;
}

void CParticleSystem::Start()
{
	Transform()->FinalUpdate();

	TParticle arrParticle[g_testParticleCnt] = {};

	float fDist = 50.f;

	for (int i = 0; i < g_testParticleCnt; ++i) {
		float fTheta = (g_testParticleCnt - i + 1) * (XM_PI / 4.f);

		arrParticle[i].fCurTime = 0.f;
		arrParticle[i].fMaxTime = 1.5f;
		arrParticle[i].vWorldDir = Vector3(cosf(fTheta), sinf(fTheta), 0.f);
		arrParticle[i].iAlive = true;
		Vector3 vObjPos = Transform()->GetPosition();
		arrParticle[i].vWorldPos = vObjPos + Vector3((-((float)g_testParticleCnt - 1.f) / 2.f) * fDist + (float)i * fDist, 0.f, 0.f);
	}

	m_pParticleBuffer = new CStructuredBuffer;
	m_pParticleBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TParticle), g_testParticleCnt, arrParticle);
}

void CParticleSystem::Update()
{
}

void CParticleSystem::FinalUpdate()
{
	m_pUpdateShader->SetParticleBuffer(m_pParticleBuffer);
	UINT iSpawnCount = _CalculateSpawnCount();
	m_pUpdateShader->SetSpawnCount(iSpawnCount);
	m_pUpdateShader->SetStartSpeed(m_fStartSpeed);
	m_pUpdateShader->SetEndSpeed(m_fEndSpeed);
	m_pUpdateShader->Excute();
}

void CParticleSystem::Render()
{
	m_pMaterial->SetData(E_ShaderParam::Vector4_0, &m_vStartColor);
	m_pMaterial->SetData(E_ShaderParam::Vector4_1, &m_vEndColor);
	m_pMaterial->SetData(E_ShaderParam::Vector4_2, &m_vStartScale);
	m_pMaterial->SetData(E_ShaderParam::Vector4_3, &m_vEndScale);
	m_pMaterial->UpdateData();

	Transform()->UpdateData();
	m_pParticleBuffer->UpdateData(REGISTER_NUM_Particle, E_ShaderStage::All);

	m_pMesh->UpdateData();
	m_pMesh->Render_Particle(g_testParticleCnt);

	m_pMaterial->Clear();
	m_pParticleBuffer->Clear(E_ShaderStage::All);
}

UINT CParticleSystem::_CalculateSpawnCount()
{
	UINT iSpawnCount = 0;
	float OneParticleCreateTime = 1.f / (float)m_iSpawnCntPerSec; // 한개 생성하는데 걸리는 시간

	m_fAccTime += DeltaTime;

	// 누적시간이 넘어섰으면
	if (OneParticleCreateTime < m_fAccTime) {
		float fCreateValue = m_fAccTime / OneParticleCreateTime; // 생성해야될 개수

		// 이번 프레임에 생성할 파티클 개수
		iSpawnCount = (UINT)floorf(fCreateValue); // 정수 부분만 가져온다. (내림floorf 사용)

		// 남은 누적시간
		m_fAccTime = fmodf(m_fAccTime, OneParticleCreateTime); // 나머지(소수) 부분만 가져오기. (fmodf 사용)
	}

	return iSpawnCount;
}