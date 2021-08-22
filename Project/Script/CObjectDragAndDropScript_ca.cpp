#include "pch.h"
#include "CObjectDragAndDropScript_ca.h"
#include <Engine\CRenderManager.h>
#include "CModuleScript_ca.h"
#include "CGirderScript_ca.h"
#include "CBoosterScript_ca.h"
#include "CLaserScript_ca.h"
#include "CCommandModuleScript_ca.h"

CObjectDragAndDropScript_ca::CObjectDragAndDropScript_ca() :
	CScript((UINT)SCRIPT_TYPE::OBJECTDRAGANDDROPSCRIPT_CA),
	m_pTargetObj(nullptr)
{
}

CObjectDragAndDropScript_ca::~CObjectDragAndDropScript_ca()
{
}

void CObjectDragAndDropScript_ca::Awake()
{
	GetGameObject()->AddComponent<CCollider2D>();
	Collider2D()->SetOffsetScale(Vector2(1.f, 1.f));
	Collider2D()->SetActive(false);
}

void CObjectDragAndDropScript_ca::Update()
{
	CCamera* pMainCamera = CRenderManager::GetInstance()->GetMainCamera();
	Vector3 vWorldMousePos = pMainCamera->GetScreenToWorld2DPosition(MousePosition);

	if (InputKeyPress(E_Key::LBUTTON)) {
		Collider2D()->SetActive(true);
	}
	if (InputKeyHold(E_Key::LBUTTON)) {
		if (m_pTargetObj) {

			Vector3 vOffsetPos = {};
			Vector3 vTargetObjPos = m_pTargetObj->Transform()->GetPosition();
			if (m_pModuleScript) {
				// 모듈의 Main연결 위치를 구한다.
				Vector3 vModulePosition = m_pModuleScript->GetMainConnectionPosition();

				// 모듈의 연결 위치와 오브젝트의 위치 차를 구한다.
				vOffsetPos = vModulePosition - vTargetObjPos;
			}
			m_pTargetObj->Transform()->SetLocalPosition(vWorldMousePos - vOffsetPos);
		}
	}
	if (InputKeyRelease(E_Key::LBUTTON)) {
		Collider2D()->SetActive(false);
		if (m_pTargetObj) {
			m_pTargetObj = nullptr;
		}
	}
	Transform()->SetLocalPosition(vWorldMousePos);
}

void CObjectDragAndDropScript_ca::OnCollisionEnter2D(CCollider2D* _pOther)
{
	// 클릭한것이라면
	if (0 == _pOther->GetGameObject()->GetLayer()) {

		// 모듈을 클릭했으면
		if (_pOther->GetGameObject()->GetTag() == 0) {
			CModuleScript_ca* pModuleScript =_pOther->GetGameObject()->GetComponent<CModuleScript_ca>();
			if (pModuleScript) {
				m_pModuleScript = pModuleScript;
				if (nullptr == m_pTargetObj) {
					m_pTargetObj = _pOther->GetGameObject();
					Collider2D()->SetActive(false);
				}
			}
		}
	}
}

bool CObjectDragAndDropScript_ca::SaveToScene(FILE* _pFile)
{
	return false;
}

bool CObjectDragAndDropScript_ca::LoadFromScene(FILE* _pFile)
{
	return false;
}
