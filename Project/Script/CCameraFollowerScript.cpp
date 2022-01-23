#include "pch.h"
#include "CCameraFollowerScript.h"
#include <Engine\CRenderManager.h>
#include "CPlayerController_bu.h"

CCameraFollowerScript::CCameraFollowerScript() :
	CScript((UINT)SCRIPT_TYPE::CAMERAFOLLOWERSCRIPT),
	m_pTargetObj(nullptr),
	m_fMouseScrollSpeed(3.f),
	m_fLerpSpeed(8.0f)
{
	AddParam(TScriptParam(_T("Enable Follow"), E_ScriptParam::BOOL, &m_bEnableFollow));
	AddParam(TScriptParam{});
	AddParam(TScriptParam(_T("Lerp Time"), E_ScriptParam::FLOAT, &m_fLerpSpeed));

	AddParam(TScriptParam(_T("Enable Zoom"), E_ScriptParam::BOOL, &m_bEnableZoom));
	AddParam(TScriptParam(_T("MousScroll Speed"), E_ScriptParam::FLOAT, &m_fMouseScrollSpeed));
}

CCameraFollowerScript::CCameraFollowerScript(const CCameraFollowerScript& _origin) :
	CScript((UINT)SCRIPT_TYPE::CAMERAFOLLOWERSCRIPT),
	m_pTargetObj(nullptr),
	m_fMouseScrollSpeed(_origin.m_fMouseScrollSpeed),
	m_fLerpSpeed(_origin.m_fLerpSpeed)
{
	AddParam(TScriptParam(_T("Enable Follow"), E_ScriptParam::BOOL, &m_bEnableFollow));

	AddParam(TScriptParam(_T("Lerp Time"), E_ScriptParam::FLOAT, &m_fLerpSpeed));

	AddParam(TScriptParam(_T("Enable Zoom"), E_ScriptParam::BOOL, &m_bEnableZoom));
	AddParam(TScriptParam(_T("MousScroll Speed"), E_ScriptParam::FLOAT, &m_fMouseScrollSpeed));
}

CCameraFollowerScript::~CCameraFollowerScript()
{
}

void CCameraFollowerScript::Awake()
{
	assert(Camera());
	CPlayerController_bu* pPlayerCtrl = nullptr;
	FIND_Component(pPlayerCtrl, CPlayerController_bu);
	CGameObject* pPlayerObj = pPlayerCtrl->GetGameObject();
	m_pTargetObj = pPlayerObj;
	assert(pPlayerObj);
}

void CCameraFollowerScript::Update()
{
	if (nullptr == Camera())
		return;
	if (nullptr == m_pTargetObj || m_pTargetObj->IsDead()) {
		m_pTargetObj = nullptr;
		return;
	}

	if (m_bEnableFollow) {
		Vector3 vTargetPos = m_pTargetObj->Transform()->GetPosition();
		Vector3 vCurPos = Transform()->GetPosition();

		Vector3 vResultPos{};
		if (m_bIsShaking)
			vResultPos = vTargetPos;
		else
			vResultPos = CMyMath::Lerp(vCurPos, vTargetPos, m_fLerpSpeed * DT);

		vResultPos.z = vCurPos.z;
		Transform()->SetLocalPosition(vResultPos);
	}

	if (m_bEnableZoom) {
		CMouseEvent evt = CMouseManager::GetInstance()->GetMouseEvent();
		if (E_MouseEventType::WheelUp == evt.GetType() ||
			E_MouseEventType::WheelDown == evt.GetType()) {
			if (E_ProjectionType::Orthographic == Camera()->GetProjectionType())
				Zoom();
		}
	}

	if (m_bIsShaking) {
		m_fShakingTime += DT;
		if (m_fShakingTime >= m_fMaxShakingTime) { // 시간 다되면 끝냄
			m_bIsShaking = false;
		}
		else { // 흔들기
			if (m_bIsRandomShake) {
				float fCamPosX = rand() % (int)m_fShakeRange * 2 - m_fShakeRange;
				float fCamPosY = rand() % (int)m_fShakeRange * 2 - m_fShakeRange;
				Vector3 vShakingOffsetPos = Vector3(fCamPosX, fCamPosY, 0.f);
				Vector3 vResPos = Transform()->GetLocalPosition() + vShakingOffsetPos;
				vShakingOffsetPos.z = Transform()->GetLocalPosition().z;
				Transform()->SetLocalPosition(vResPos);
			}
			else {
				Vector3 vShakingOffsetPos = m_vShakeDir * m_fShakeRange;
				Vector3 vResPos = Transform()->GetLocalPosition() + vShakingOffsetPos;
				vShakingOffsetPos.z = Transform()->GetLocalPosition().z;
				Transform()->SetLocalPosition(vResPos);
			}
		}
	}
}

// 20220107 : Butcher 게임에서 사용중임 다른 프로젝트에서 수정하면 큰일난다?
bool CCameraFollowerScript::SaveToScene(FILE* _pFile)
{
	FWrite(m_fLerpSpeed, _pFile);
	FWrite(m_bEnableFollow, _pFile);
	FWrite(m_bEnableZoom, _pFile);
	FWrite(m_fMouseScrollSpeed, _pFile);
	return true;
}

bool CCameraFollowerScript::LoadFromScene(FILE* _pFile)
{
	FRead(m_fLerpSpeed, _pFile);
	FRead(m_bEnableFollow, _pFile);
	FRead(m_bEnableZoom, _pFile);
	FRead(m_fMouseScrollSpeed, _pFile);
	return true;
}

void CCameraFollowerScript::Zoom()
{
	float fY = MouseScrollDelta * m_fMouseScrollSpeed;
	float fSize = Camera()->GetSize();
	if (fY == 0.f)
		fY = 0.001f;
	fSize += fY;
	fSize = max(0.001f, fSize);
	Camera()->SetSize(fSize);
}

void CCameraFollowerScript::Shaking(bool _bIsRandom, float _fRange, float _fTime, const Vector3& _vShakeDir)
{
	m_bIsShaking = true;
	m_fShakingTime = 0.f;
	m_fMaxShakingTime = _fTime;
	m_bIsRandomShake = _bIsRandom;
	m_fShakeRange = _fRange;
	m_vShakeDir = _vShakeDir;
}