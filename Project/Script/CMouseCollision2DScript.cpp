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
	if (InputKeyPress(E_Key::LBUTTON)) {
		if (!m_listHitObjs.empty()) {
			m_pTargetObj = m_listHitObjs.front();
			m_listHitObjs.clear();
		}
	}
	if (InputKeyHold(E_Key::LBUTTON)) {
		if (m_pTargetObj) {
			Vector3 vTargetPos = m_pTargetObj->Transform()->GetPosition();
			Vector2 vMousePos = MousePosition;
			// Camera의 Screen의 World Position을 구해서 넣어주자
			vTargetPos.x = vMousePos.x;
			vTargetPos.y = vMousePos.y;
			m_pTargetObj->Transform()->SetLocalPosition(vTargetPos);
		}
	}
	if (InputKeyPress(E_Key::LBUTTON)) {
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
		m_listHitObjs.push_back(_pOther->GetGameObject());
}

void CMouseCollision2DScript::OnCollisionStay2D(CCollider2D* _pOther)
{
}

void CMouseCollision2DScript::OnCollisionExit2D(CCollider2D* _pOther)
{
}