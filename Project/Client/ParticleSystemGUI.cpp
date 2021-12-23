#include "pch.h"
#include "ParticleSystemGUI.h"
#include <Engine\CParticleSystem.h>
#include <Engine\CParticleUpdateShader.h>
#include <Engine\CResourceManager.h>
#include "ParamGUI.h"

ParticleSystemGUI::ParticleSystemGUI() :
	ComponentGUI(E_ComponentType::ParticleSystem)
{
	_ResetShapeComboBoxStr(m_vecStrShape);
}

ParticleSystemGUI::~ParticleSystemGUI()
{
}

void ParticleSystemGUI::Update()
{
	if (false == Start())
		return;


	CParticleSystem* pPS = GetTargetObject()->ParticleSystem();
	Vector4 vStartColor = pPS->GetStartColor();
	Vector4 vEndColor = pPS->GetEndColor();

	Vector3 vStartScale = pPS->GetStartScale();
	Vector3 vEndScale = pPS->GetEndScale();

	float fStartSpeed = pPS->GetStartSpeed();
	float fEndSpeed = pPS->GetEndSpeed();

	float fMinLifeTime = pPS->GetMinLifeTime();
	float fMaxLifeTime = pPS->GetMaxLifeTime();

	int iMaxParticleCnt = (int)pPS->GetMaxParticleCount();
	int iSpawnCntPerSec = (int)pPS->GetSpawnCntPerSec();
	E_ParticleShape eShape = pPS->GetShape();

	static int iCurComboItem = 0;
	if (ParamGUI::Render_ComboBox("Shape##ParticleSystemGUI", &iCurComboItem, m_vecStrShape))
		pPS->SetShape(E_ParticleShape(iCurComboItem));

	if (ImGui::DragFloat4("Start Color##ParticleSystemGUI", (float*)vStartColor, 0.01f, 0.f, 1.f, "%.2f", 0))
		pPS->SetStartColor(vStartColor);
	if (ImGui::DragFloat4("End Color##ParticleSystemGUI", (float*)vEndColor, 0.01f, 0.f, 1.f, "%.2f", 0))
		pPS->SetEndColor(vEndColor);

	if (ImGui::DragFloat3("Start Scale##ParticleSystemGUI", (float*)&vStartScale, 0.01f, 0.f, FLOAT_MAX, "%.2f", 0))
		pPS->SetStartScale(vStartScale);
	if (ImGui::DragFloat3("End Scale##ParticleSystemGUI", (float*)&vEndScale, 0.2f, 0.f, FLOAT_MAX, "%.2f", 0))
		pPS->SetEndScale(vEndScale);
	if (ImGui::DragFloat("Start Speed##ParticleSystemGUI", &fStartSpeed, 0.2f, 0.f, FLOAT_MAX, "%.2f", 0))
		pPS->SetStartSpeed(fStartSpeed);
	if (ImGui::DragFloat("End Speed##ParticleSystemGUI", &fEndSpeed, 0.2f, 0.f, FLOAT_MAX, "%.2f", 0))
		pPS->SetEndSpeed(fEndSpeed);
	if (ImGui::DragFloat("Min Life Time##ParticleSystemGUI", &fMinLifeTime, 0.2f, 0.f, FLOAT_MAX, "%.2f", 0))
		pPS->SetMinLifeTime(fMinLifeTime);
	if (ImGui::DragFloat("Max Life Time##ParticleySystemGUI", &fMaxLifeTime, 0.2f, 0.f, FLOAT_MAX, "%.2f", 0))
		pPS->SetMaxLifeTime(fMaxLifeTime);
	if (ImGui::DragInt("Max Particle Cnt##ParticleSystemGUI", &iMaxParticleCnt, 1.f, 0, INT_MAX, "%d"))
		pPS->SetMaxParticleCount((UINT)iMaxParticleCnt);
	if (ImGui::DragInt("Spawn Cnt PerSec##ParticleSystemGUI", &iSpawnCntPerSec, 1.f, 0, INT_MAX, "%d"))
		pPS->SetSpawnCntPerSec((UINT)iSpawnCntPerSec);

	bool isGravityEnable = pPS->IsGravityEnable();
	if (ImGui::Checkbox("Gravity##ParticleSystem", &isGravityEnable))
		pPS->SetGravityEnable(isGravityEnable);

	CTexture* pParticleTex = nullptr;
	if (nullptr != pPS->GetParticleTexture())
		pParticleTex = pPS->GetParticleTexture().Get();
	ParamGUI::Render_Texture("Texture##ParticleSystemGUI", pParticleTex, this, (GUI_CALLBACK)&ParticleSystemGUI::_SelectParticleTexture);



	End();
}

void ParticleSystemGUI::_ResetShapeComboBoxStr(vector<char>& _vecStrShape_out)
{
	_vecStrShape_out.clear();

	vector<tstring> vectShape;
	for (size_t i = 0; i < (size_t)E_ParticleShape::End; ++i)
		vectShape.push_back(ParticleShapeToStr((E_ParticleShape)i));

	vector <string> vecShape;
	TStringToStringVec(vectShape, vecShape);

	ParamGUI::Make_ComboBoxList(vecShape, _vecStrShape_out);
}

void ParticleSystemGUI::_SelectParticleTexture(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
	const char* pStrKey = (const char*)_strKey;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pTex = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pTex);

	GetTargetObject()->ParticleSystem()->SetParticleTexture(pTex);
}