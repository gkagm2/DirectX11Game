#include "pch.h"
#include "CMouseCollision2DScript.h"
#include <Engine\CRenderManager.h>

CMouseCollision2DScript::CMouseCollision2DScript() :
	CScript((UINT)SCRIPT_TYPE::MOUSECOLLISION2DSCRIPT),
	m_pTargetObj(nullptr)
{
}

CMouseCollision2DScript::~CMouseCollision2DScript()
{
}

void CMouseCollision2DScript::Awake()
{
	GetGameObject()->AddComponent<CCollider2D>();
	Collider2D()->SetOffsetScale(Vector2(1.f, 1.f));
}

void CMouseCollision2DScript::Start()
{
}

void CMouseCollision2DScript::Update()
{
	static Vector3 vOffset = {};
	static Vector3 vStart = {};
	static Vector3 vTargetStartPos = {};
	if (InputKeyPress(E_Key::LBUTTON)) {
		Collider2D()->SetActive(true);
		if (m_pTargetObj) {
			Vector3 vMouseWorldPos = CRenderManager::GetInstance()->GetMainCamera()->GetScreenToWorld2DPosition(); 
			vStart = vMouseWorldPos;
			vTargetStartPos = m_pTargetObj->Transform()->GetPosition();
		}
	}
	if (InputKeyHold(E_Key::LBUTTON)) {
		if (m_pTargetObj) {
			Vector3 vMouseWorldPos = CRenderManager::GetInstance()->GetMainCamera()->GetScreenToWorld2DPosition();
			vOffset = vMouseWorldPos - vStart;
			m_pTargetObj->Transform()->SetLocalPosition(vTargetStartPos + vOffset);
		}
	}
	if (InputKeyRelease(E_Key::LBUTTON)) {
		Collider2D()->SetActive(false);
		if (m_pTargetObj) {
			m_pTargetObj = nullptr;
		}
	}
	Vector3 mousePos = Vector3(MousePosition.x, MousePosition.y, 0.f);
	CCamera* pMainCamera = CRenderManager::GetInstance()->GetMainCamera();
	Vector3 vWorldPos = pMainCamera->GetScreenToWorld2DPosition();
	Transform()->SetLocalPosition(vWorldPos);
}

void CMouseCollision2DScript::OnCollisionEnter2D(CCollider2D* _pOther)
{
	// 클릭한것이라면
	if (0 == _pOther->GetGameObject()->GetLayer())
		if(nullptr == m_pTargetObj)
			m_pTargetObj = _pOther->GetGameObject();
}

void CMouseCollision2DScript::OnCollisionStay2D(CCollider2D* _pOther)
{
}

void CMouseCollision2DScript::OnCollisionExit2D(CCollider2D* _pOther)
{
}