#include "pch.h"
#include "CParticleSystem.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"
#include "CMaterial.h"

#include "CTimeManager.h"

CParticleSystem::CParticleSystem() :
	CComponent(E_ComponentType::ParticleSystem),
	m_iTexIdx(6),
	m_pParticleBuffer(nullptr),
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
	m_vRadius(50.f, 50.f, 0.f)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_PointMesh);
	m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_ParticleMtrl);
	m_pUpdateShader = dynamic_cast<CParticleUpdateShader*>(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_ParticleUpdateShader).Get());

	assert(m_pMesh.Get());
	assert(m_pMaterial.Get());
	assert(m_pUpdateShader.Get());

	m_pParticleBuffer = make_unique<CStructuredBuffer>();
	m_pParticleBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TParticle), m_iMaxParticleCount, nullptr);

	// ��ƼŬ�� �ؽ��� �ε�
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle1"), _T("texture\\particle\\AlphaCircle.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle2"), _T("texture\\particle\\Bubbles50px.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle3"), _T("texture\\particle\\Bubbles99px.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle4"), _T("texture\\particle\\CartoonSmoke.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle5"), _T("texture\\particle\\HardCircle.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle6"), _T("texture\\particle\\HardRain.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle7"), _T("texture\\particle\\smokeparticle.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle8"), _T("texture\\particle\\Snow50px.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle9"), _T("texture\\particle\\Sparks.png")));

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
	m_vRadius(_origin.m_vRadius)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_PointMesh);
	m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_ParticleMtrl);
	m_pUpdateShader = dynamic_cast<CParticleUpdateShader*>(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_ParticleUpdateShader).Get());

	assert(m_pMesh.Get());
	assert(m_pMaterial.Get());
	assert(m_pUpdateShader.Get());

	m_pParticleBuffer = make_unique<CStructuredBuffer>();
	m_pParticleBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TParticle), m_iMaxParticleCount, nullptr);

	// ��ƼŬ�� �ؽ��� �ε�
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle1"), _T("texture\\particle\\AlphaCircle.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle2"), _T("texture\\particle\\Bubbles50px.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle3"), _T("texture\\particle\\Bubbles99px.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle4"), _T("texture\\particle\\CartoonSmoke.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle5"), _T("texture\\particle\\HardCircle.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle6"), _T("texture\\particle\\HardRain.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle7"), _T("texture\\particle\\smokeparticle.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle8"), _T("texture\\particle\\Snow50px.png")));
	m_vecParticleTex.push_back(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Particle9"), _T("texture\\particle\\Sparks.png")));

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

void CParticleSystem::FinalUpdate()
{
	m_pUpdateShader->SetParticleBuffer(m_pParticleBuffer.get());
	UINT iSpawnCount = _CalculateSpawnCount();
	m_pUpdateShader->SetSpawnCount(iSpawnCount);
	m_pUpdateShader->SetStartSpeed(m_fStartSpeed);
	m_pUpdateShader->SetEndSpeed(m_fEndSpeed);
	m_pUpdateShader->SetMinLifeTime(m_fMinLifeTime);
	m_pUpdateShader->SetMaxLifeTime(m_fMaxLifeTime);
	m_pUpdateShader->SetRadius(m_vRadius);
	m_pUpdateShader->SetWorldPos(Transform()->GetPosition());
	m_pUpdateShader->Excute();
}

void CParticleSystem::Render()
{
	m_pMaterial->SetData(E_ShaderParam::Texture_0, m_vecParticleTex[m_iTexIdx].Get());
	m_pMaterial->SetData(E_ShaderParam::Vector4_0, &m_vStartColor);
	m_pMaterial->SetData(E_ShaderParam::Vector4_1, &m_vEndColor);
	m_pMaterial->SetData(E_ShaderParam::Vector4_2, &m_vStartScale);
	m_pMaterial->SetData(E_ShaderParam::Vector4_3, &m_vEndScale);
	m_pMaterial->UpdateData();

	Transform()->UpdateData();
	m_pParticleBuffer->UpdateData(REGISTER_NUM_Particle, E_ShaderStage::All);

	m_pMesh->UpdateData();
	m_pMesh->Render_Particle(m_iMaxParticleCount);

	m_pMaterial->Clear();
	m_pParticleBuffer->Clear(E_ShaderStage::All);
}

UINT CParticleSystem::_CalculateSpawnCount()
{
	UINT iSpawnCount = 0;
	float OneParticleCreateTime = 1.f / (float)m_iSpawnCntPerSec; // �Ѱ� �����ϴµ� �ɸ��� �ð�

	m_fAccTime += DeltaTime;

	// �����ð��� �Ѿ����
	if (OneParticleCreateTime < m_fAccTime) {
		float fCreateValue = m_fAccTime / OneParticleCreateTime; // �����ؾߵ� ����

		// �̹� �����ӿ� ������ ��ƼŬ ����
		iSpawnCount = (UINT)floorf(fCreateValue); // ���� �κи� �����´�. (����floorf ���)

		// ���� �����ð�
		m_fAccTime = fmodf(m_fAccTime, OneParticleCreateTime); // ������(�Ҽ�) �κи� ��������. (fmodf ���)
	}

	return iSpawnCount;
}