#include "pch.h"
#include "CPostProcessingScript.h"

CPostProcessingScript::CPostProcessingScript() :
	CScript((UINT)SCRIPT_TYPE::POSTPROCESSINGSCRIPT),
	m_fScreenScale{2.f},
	m_fAmplitude{20.f},
	m_fCompression{},
	m_fSpeed{12.56f},
	m_iPostProcessingType{}
{
}

CPostProcessingScript::~CPostProcessingScript()
{
}

void CPostProcessingScript::Start()
{
	m_pPostEffectMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_BlurMtrl);
	assert(m_pPostEffectMtrl.Get());
	m_pPostEffectMtrl = m_pPostEffectMtrl->Clone();
	MeshRenderer()->SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh));
	MeshRenderer()->SetMaterial(m_pPostEffectMtrl, 0);


}

void CPostProcessingScript::Update()
{
	if (InputKeyHold(E_Key::NUM1)) {
		m_fScreenScale += DT;
	}
	if (InputKeyHold(E_Key::NUM2)) {
		m_fScreenScale -= DT;
	}
	if (InputKeyHold(E_Key::NUM2)) {
		m_fAmplitude += DT * 0.3f;
	}
	if (InputKeyHold(E_Key::NUM3)) {
		m_fAmplitude -= DT * 0.3f;
	}
	if (InputKeyHold(E_Key::NUM4)) {
		m_fAmplitude += DT * 0.3f;
	}
	if (InputKeyHold(E_Key::NUM5)) {
		m_fAmplitude -= DT * 0.3f;
	}
	if (InputKeyHold(E_Key::NUM6)) {
		m_fSpeed += DT * 0.3f;
	}
	if (InputKeyHold(E_Key::NUM7)) {
		m_fSpeed -= DT * 0.3f;
	}

	if (InputKeyHold(E_Key::NUM8)) {
		
	}
	if (InputKeyHold(E_Key::NUM8)) {

	}

	m_pPostEffectMtrl->SetData(E_ShaderParam::Float_0, &m_fAmplitude);
	m_pPostEffectMtrl->SetData(E_ShaderParam::Float_1, &m_fCompression);
	m_pPostEffectMtrl->SetData(E_ShaderParam::Float_2, &m_fScreenScale);
	m_pPostEffectMtrl->SetData(E_ShaderParam::Float_3, &m_fSpeed);

	m_pPostEffectMtrl->SetData(E_ShaderParam::Int_0, &m_iPostProcessingType);
	
}