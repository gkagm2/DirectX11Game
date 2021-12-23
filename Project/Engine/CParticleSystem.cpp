#include "pch.h"
#include "CParticleSystem.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"
#include "CMaterial.h"

#include "CTimeManager.h"

CParticleSystem::CParticleSystem() :
	CComponent(E_ComponentType::ParticleSystem),
	m_pParticleBuffer(nullptr),
	m_pParticleTex(nullptr),
	m_vStartScale(50.f, 50.f, 0.f, 0.f),
	m_vEndScale(20.f, 20, 0.f, 0.f),
	m_vStartColor(1.f, 0.0f, 0.0f, 1.f),
	m_vEndColor(0.f, 1.f, 0.f, 1.f),
	m_fStartSpeed(100),
	m_fEndSpeed(10.f),
	m_fMinLifeTime(1.f),
	m_fMaxLifeTime(4.f),
	m_iSpawnCntPerSec(100),
	m_iMaxParticleCount(100),
	m_fAccTime(0.f),
	m_vRadius(50.f, 50.f, 0.f),
	m_eShape(E_ParticleShape::Circle_Spread),
	m_bGravityEnable(false)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_PointMesh);
	m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_ParticleMtrl);
	m_pUpdateShader = dynamic_cast<CParticleUpdateShader*>(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_ParticleUpdateShader).Get());

	assert(m_pMesh.Get());
	assert(m_pMaterial.Get());
	assert(m_pUpdateShader.Get());

	m_pParticleBuffer = make_unique<CStructuredBuffer>();
	if (FAILED(m_pParticleBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TParticle), m_iMaxParticleCount, false)))
		assert(nullptr);

	// 파티클용 텍스쳐 로딩
	m_pParticleTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle1"), _T("texture\\particle\\AlphaCircle.png"));
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin) :
	CComponent(_origin),
	m_pMesh(nullptr),
	m_pMaterial(nullptr),
	m_pParticleBuffer(nullptr),
	m_pUpdateShader(nullptr),
	m_vStartScale(_origin.m_vStartScale),
	m_vEndScale(_origin.m_vEndScale),
	m_vStartColor(_origin.m_vStartColor),
	m_vEndColor(_origin.m_vEndColor),
	m_fStartSpeed(_origin.m_fStartSpeed),
	m_fEndSpeed(_origin.m_fEndSpeed),
	m_fMinLifeTime(_origin.m_fMinLifeTime),
	m_fMaxLifeTime(_origin.m_fMaxLifeTime),
	m_iSpawnCntPerSec(_origin.m_iSpawnCntPerSec),
	m_iMaxParticleCount(_origin.m_iMaxParticleCount),
	m_fAccTime(_origin.m_fAccTime),
	m_vRadius(_origin.m_vRadius),
	m_eShape(_origin.m_eShape),
	m_bGravityEnable(_origin.m_bGravityEnable)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_PointMesh);
	m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_ParticleMtrl);
	m_pUpdateShader = dynamic_cast<CParticleUpdateShader*>(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_ParticleUpdateShader).Get());

	assert(m_pMesh.Get());
	assert(m_pMaterial.Get());
	assert(m_pUpdateShader.Get());

	m_pParticleBuffer = make_unique<CStructuredBuffer>();
	m_pParticleBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TParticle), m_iMaxParticleCount, false);

	// 파티클용 텍스쳐 로딩
	m_pParticleTex = _origin.m_pParticleTex;
}

CParticleSystem::~CParticleSystem()
{
}

void CParticleSystem::Start()
{
}

void CParticleSystem::Update()
{
}

void CParticleSystem::LateUpdate()
{
	// (jang) : 실행 시 보여주기 위해 FinalUpdate에서 하지 않고 LateUpdate에 넣음.
	m_pUpdateShader->SetParticleBuffer(m_pParticleBuffer.get());
	UINT iSpawnCount = _CalculateSpawnCount();
	m_pUpdateShader->SetSpawnCount(iSpawnCount);
	m_pUpdateShader->SetStartSpeed(m_fStartSpeed);
	m_pUpdateShader->SetEndSpeed(m_fEndSpeed);
	m_pUpdateShader->SetMinLifeTime(m_fMinLifeTime);
	m_pUpdateShader->SetMaxLifeTime(m_fMaxLifeTime);
	m_pUpdateShader->SetRadius(m_vRadius);
	m_pUpdateShader->SetWorldPos(Transform()->GetPosition());
	m_pUpdateShader->SetShape(m_eShape);
	m_pUpdateShader->SetGravityEnable(m_bGravityEnable);
	m_pUpdateShader->Excute();
}

void CParticleSystem::Render()
{
	m_pMaterial->SetData(E_ShaderParam::Texture_0, m_pParticleTex.Get());

	m_pMaterial->SetData(E_ShaderParam::Vector4_0, &m_vStartColor);
	m_pMaterial->SetData(E_ShaderParam::Vector4_1, &m_vEndColor);
	m_pMaterial->SetData(E_ShaderParam::Vector4_2, &m_vStartScale);
	m_pMaterial->SetData(E_ShaderParam::Vector4_3, &m_vEndScale);
	m_pMaterial->UpdateData();

	Transform()->UpdateData();
	m_pParticleBuffer->UpdateData(REGISTER_NUM_Particle, E_ShaderStage::All);

	m_pMesh->Render_Particle(m_iMaxParticleCount);

	m_pMaterial->Clear();
	m_pParticleBuffer->Clear(E_ShaderStage::All);
}

UINT CParticleSystem::_CalculateSpawnCount()
{
	UINT iSpawnCount = 0;
	float OneParticleCreateTime = 1.f / (float)m_iSpawnCntPerSec; // 한개 생성하는데 걸리는 시간

	m_fAccTime += DT;

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

bool CParticleSystem::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	SaveResourceToFile(m_pParticleTex, _pFile);

	FWrite(m_vStartColor, _pFile);
	FWrite(m_vEndColor, _pFile);
	FWrite(m_vStartScale, _pFile);
	FWrite(m_vEndScale, _pFile);
	FWrite(m_fStartSpeed, _pFile);
	FWrite(m_fEndSpeed, _pFile);
	FWrite(m_fMinLifeTime, _pFile);
	FWrite(m_fMaxLifeTime, _pFile);
	FWrite(m_iSpawnCntPerSec, _pFile);
	FWrite(m_iMaxParticleCount, _pFile);
	FWrite(m_vRadius, _pFile);
	FWrite(m_eShape, _pFile);
	FWrite(m_bGravityEnable, _pFile);

	return true;
}

bool CParticleSystem::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	
	LoadResourceFromFile(m_pParticleTex, _pFile);

	FRead(m_vStartColor, _pFile);
	FRead(m_vEndColor, _pFile);
	FRead(m_vStartScale, _pFile);
	FRead(m_vEndScale, _pFile);
	FRead(m_fStartSpeed, _pFile);
	FRead(m_fEndSpeed, _pFile);
	FRead(m_fMinLifeTime, _pFile);
	FRead(m_fMaxLifeTime, _pFile);
	FRead(m_iSpawnCntPerSec, _pFile);
	FRead(m_iMaxParticleCount, _pFile);
	FRead(m_vRadius, _pFile);
	FRead(m_eShape, _pFile);
	FRead(m_bGravityEnable, _pFile);

	if (m_iMaxParticleCount != m_pParticleBuffer->GetElementCount()) {
		m_pParticleBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TParticle), m_iMaxParticleCount, false);
	}

	return true;
}