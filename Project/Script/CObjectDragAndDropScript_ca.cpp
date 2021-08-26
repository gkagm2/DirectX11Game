#include "pch.h"
#include "CObjectDragAndDropScript_ca.h"
#include <Engine\CRenderManager.h>
#include "CModuleScript_ca.h"
#include "CGirderScript_ca.h"
#include "CBoosterScript_ca.h"
#include "CLaserScript_ca.h"
#include "CCommandModuleScript_ca.h"
#include <Engine\CLayer.h>
CObjectDragAndDropScript_ca::CObjectDragAndDropScript_ca() :
	CScript((UINT)SCRIPT_TYPE::OBJECTDRAGANDDROPSCRIPT_CA),
	m_pTargetObj(nullptr),
	m_pModuleScript(nullptr),
	m_pNearestModuleObj(nullptr),
	m_fMaxSearchObjDistance(1.f),
	m_fMaxConnectableDistance(0.5f),
	m_bIsConnectableOtherObj(false)
{
}

CObjectDragAndDropScript_ca::~CObjectDragAndDropScript_ca()
{
}

void CObjectDragAndDropScript_ca::Awake()
{
	GetGameObject()->AddComponent<CCollider2D>();
	
	Collider2D()->SetOffsetScale(Vector2(0.1f, 0.1f));
	Collider2D()->SetActive(false);
}

void CObjectDragAndDropScript_ca::Update()
{
	CCamera* pMainCamera = CRenderManager::GetInstance()->GetMainCamera();
	if (nullptr == pMainCamera)
		return;
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

			// 가장 가까운 모듈 오브젝트를 검색한다.
			m_pNearestModuleObj = FindNearestModuleObject(vWorldMousePos);

			// 오브젝트가 존재하면
			if (m_pNearestModuleObj) {
				Vector3 vNearestPos = {};

				// 가장 가까운 모듈의 연결 가능한 위치 찾기
				CModuleScript_ca* pModule = m_pNearestModuleObj->GetComponent< CModuleScript_ca>();
				vNearestPos = pModule->FindNearestConnectionPosition(vWorldMousePos);

				float fDistance = Vector3::Distance(vNearestPos, vWorldMousePos);
			
				// 연결 가능한 거리면
				if (fDistance <= m_fMaxConnectableDistance) {
					TModuleConnector_ca& tConnector = m_pNearestModuleObj->GetComponent<CModuleScript_ca>()->FindNearestConnector(vWorldMousePos);

					// 가장 가까운 연결구를 구한다.

					// 연결이 되지 않는 연결구면
					if (tConnector.bIsConnectable) {
						// TODO (Jang) : Effect가 튀간다.
						m_bIsConnectableOtherObj = true;
					}
					// 현재 잡고 있는 오브젝트가 회전하게 된다.
					Vector3 vDir = tConnector.vDirection;
				}
			}

			m_pTargetObj->Transform()->SetLocalPosition(vWorldMousePos - vOffsetPos);
		}
	}
	if (InputKeyRelease(E_Key::LBUTTON)) {
		if (m_pTargetObj) {
			if (m_pNearestModuleObj) {
				if (m_bIsConnectableOtherObj) {
					TModuleConnector_ca& tConnector = m_pNearestModuleObj->GetComponent<CModuleScript_ca>()->FindNearestConnector(vWorldMousePos);

					if (tConnector.bIsConnectable) {
						Vector3 vConnectPos = tConnector.vPosition;
						Vector3 vMainConnectPos = m_pTargetObj->GetComponent<CModuleScript_ca>()->GetMainConnectionLocalPosition();

						// 위치에 같다 붙인다
						m_pTargetObj->Transform()->SetLocalPosition(vMainConnectPos  - vConnectPos);

						// 회전을 해야 한다.


						CObject::AddChildGameObjectEvn(m_pNearestModuleObj, m_pTargetObj);
					}
				}
			}
			m_pTargetObj = nullptr;
			m_pNearestModuleObj = nullptr;
			m_bIsConnectableOtherObj = false;
		}
		Collider2D()->SetActive(false);
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

void CObjectDragAndDropScript_ca::FindNearbyModuleObjects( vector<CGameObject*> _vecObjects, const Vector3& _vPosition, float _fMaxDistance)
{
	// 마우스 위치에서부터 일정 거리 안에 들어오는 오브젝트를 찾는다.
	static UINT iFindLayer = 0;
	static UINT iModuleTag = 0;

	_vecObjects.clear();
	
	// 1. 레이어의 모든 오브젝트를 가져온다.
	vector<CGameObject*> vecObjects;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecObjects, iFindLayer);

	// 모든 오브젝트에서 Module 컴포넌트가 들어가있는 오브젝트를 찾아서 리스트에 넣는다.
	for (UINT i = 0; i < vecObjects.size(); ++i) {
		if (m_pTargetObj == vecObjects[i])
			continue;
		CModuleScript_ca* pModule = vecObjects[i]->GetComponent<CModuleScript_ca>();
		if (nullptr == pModule)
			continue;
		const Vector3& vObjPosition = vecObjects[i]->Transform()->GetPosition();
		float fDistance = Vector3::Distance(_vPosition, vObjPosition);
		if (fDistance > _fMaxDistance)
			continue;
		
		_vecObjects.push_back(vecObjects[i]);
	}
}

CGameObject* CObjectDragAndDropScript_ca::FindNearestModuleObject(const Vector3& _vPosition)
{
	// 마우스 위치에서부터 일정 거리 안에 들어오는 오브젝트를 찾는다.
	static UINT iFindLayer = 0;
	static UINT iModuleTag = 0;
	CGameObject* pNearestModuleObj = nullptr;

	// 1. 레이어의 모든 오브젝트를 가져온다.

	vector<CGameObject*> vecObjects;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecObjects, iFindLayer);

	// 모든 오브젝트에서 Module 컴포넌트가 들어가있는 오브젝트 중 가장 가까운 오브젝트를 고른다.
	float fMinDistance = (std::numeric_limits<float>::max)();
	for (UINT i = 0; i < vecObjects.size(); ++i) {
		if (m_pTargetObj == vecObjects[i])
			continue;
		CModuleScript_ca* pModule = vecObjects[i]->GetComponent<CModuleScript_ca>();
		if (nullptr == pModule)
			continue;
		const Vector3& vObjPosition = vecObjects[i]->Transform()->GetPosition();
		float fDistance = Vector3::Distance(_vPosition, vObjPosition);
		if (fDistance < fMinDistance) {
			fMinDistance = fDistance;
			pNearestModuleObj = vecObjects[i];
		}
	}
	return pNearestModuleObj;
}
