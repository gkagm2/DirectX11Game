#include "pch.h"
#include "CParticleSystem.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"
#include "CMaterial.h"
const int g_testParticleCnt = 30;
CParticleSystem::CParticleSystem() :
	CComponent(E_ComponentType::ParticleSystem),
	m_pParticleBuffer(nullptr),
	m_fStartSpeed(500.f),
	m_fEndSpeed(50.f)
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
	m_pParticleBuffer->CreateRW(sizeof(TParticle), g_testParticleCnt, arrParticle);
}

void CParticleSystem::Update()
{
}

void CParticleSystem::FinalUpdate()
{
	m_pUpdateShader->SetParticleBuffer(m_pParticleBuffer);
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