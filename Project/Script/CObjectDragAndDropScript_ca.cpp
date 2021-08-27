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


	if (m_pTargetObj && m_pNearestModuleObj) {
		Vector3 vDownDir = -(m_pTargetObj->GetParentObject()->Transform()->GetUpVector());
		Vector3 vNDownDir = m_pNearestModuleObj->GetComponent<CModuleScript_ca>()->MainConnector().vDirection;
		Vector3 vOtherModuleDir = XMVector3TransformCoord(vNDownDir, m_pNearestModuleObj->Transform()->GetWorldMatrix());

		Vector3 r1 = m_pTargetObj->GetParentObject()->Transform()->GetLocalRotationDegree();
		Vector3 r2 = m_pNearestModuleObj->GetParentObject()->Transform()->GetLocalRotationDegree();
		_tcprintf(_T("1[%.2f %.2f %.2f][%.2f %.2f %.2f]\n"), r1.x, r1.y, r1.z, r1.x * CMyMath::Deg2Rad(), r1.y * CMyMath::Deg2Rad(), r1.z * CMyMath::Deg2Rad());
		_tcprintf(_T("2[%.2f %.2f %.2f][%.2f %.2f %.2f]\n"), r2.x, r2.y, r2.z, r2.x * CMyMath::Deg2Rad(), r2.y * CMyMath::Deg2Rad(), r2.z * CMyMath::Deg2Rad());
	}

	if (InputKeyPress(E_Key::LBUTTON)) {
		Collider2D()->SetActive(true);
	}
	if (InputKeyHold(E_Key::LBUTTON)) {
		if (m_pTargetObj) {

			// 연결 해제
			if (m_pTargetObj->GetParentObject() && m_pTargetObj->GetParentObject()->GetParentObject()) {
				CObject::UnlinkParentGameObjectEvn(m_pTargetObj->GetParentObject());
				// 연결 해제
				m_pTargetObj->GetComponent<CModuleScript_ca>()->DisConnectModule();
			}
			else {
				// 가장 가까운 모듈 오브젝트를 검색한다.
				m_pNearestModuleObj = FindNearestModuleObject(vWorldMousePos);
				if (m_pNearestModuleObj && m_pNearestModuleObj->GetParentObject()) {
					// 오브젝트가 존재하면
					Vector3 vNearestPos = {};

					// 가장 가까운 모듈의 연결 가능한 위치 찾기
					CModuleScript_ca* pNearestModule = m_pNearestModuleObj->GetComponent< CModuleScript_ca>();
					vNearestPos = pNearestModule->FindNearestConnectionPosition(vWorldMousePos);

					float fDistance = Vector3::Distance(vNearestPos, vWorldMousePos);

					if (fDistance > m_fMaxConnectableDistance)
						m_bIsConnectableOtherObj = false;
					else { // 연결 가능한 거리면
						TModuleConnector_ca& tNearestConnector = pNearestModule->FindNearestConnector(vWorldMousePos);
						TModuleConnector_ca& tDraggedMainConnector = m_pTargetObj->GetComponent<CModuleScript_ca>()->MainConnector();

						// 가장 가까운 연결구를 구한다.
						// 비어있는 연결구면
						if (tNearestConnector.bIsConnectable) {
							// TODO (Jang) : Effect가 튀간다.
							m_bIsConnectableOtherObj = true;

							Vector3 vOtherModuleRotation = m_pNearestModuleObj->GetParentObject()->Transform()->GetLocalRotationDegree();
							Vector3 vTargetModuleRotation = m_pTargetObj->GetParentObject()->Transform()->GetLocalRotationDegree();

							// 회전 할 각도 구하기

							float degree = 0;
							vTargetModuleRotation = vOtherModuleRotation;
							switch (tNearestConnector.eDirection) {
							case E_Direction_ca::Left:
								degree -= 90.f;
								break;
							case E_Direction_ca::Right:
								degree += 90.f;
								break;
							case E_Direction_ca::Forward:
								degree = 180.f;
								break;
							default:
								break;
							}
							vTargetModuleRotation.z += degree;
							m_pTargetObj->GetParentObject()->Transform()->SetLocalRotationDegree(vTargetModuleRotation);
						}
					}
				}
				m_pTargetObj->GetParentObject()->Transform()->SetLocalPosition(vWorldMousePos);
			}
		}
	}
	if (InputKeyRelease(E_Key::LBUTTON)) {
		if (m_pTargetObj) {
			// 가장 가까운 모듈 오브젝트를 검색한다.
			if(m_pNearestModuleObj && m_pNearestModuleObj->GetParentObject()) {
				if (m_bIsConnectableOtherObj) {
					TModuleConnector_ca& tNearestConnector = m_pNearestModuleObj->GetComponent<CModuleScript_ca>()->FindNearestConnector(vWorldMousePos);

					if (tNearestConnector.bIsConnectable) {
						Vector3 vConnectPos = tNearestConnector.vPosition;

						Vector3 offset = m_pNearestModuleObj->Transform()->GetLocalPosition();

						// 위치에 같다 붙인다
						m_pTargetObj->GetParentObject()->Transform()->SetLocalPosition(vConnectPos + offset);

						// 회전을 해야 한다.
						Vector3 vOtherModuleRotation = m_pNearestModuleObj->GetParentObject()->Transform()->GetLocalRotationDegree();
						Vector3 vTargetModuleRotation = m_pTargetObj->GetParentObject()->Transform()->GetLocalRotationDegree();

						// 회전 할 각도 구하기

						float degree = 0;
						vTargetModuleRotation = vOtherModuleRotation;
						switch (tNearestConnector.eDirection) {
						case E_Direction_ca::Left:
							degree = -90.f;
							break;
						case E_Direction_ca::Right:
							degree = 90.f;
							break;
						case E_Direction_ca::Forward:
							degree = 180.f;
							break;
						default:
							break;
						}
						vTargetModuleRotation.z = degree;
						m_pTargetObj->GetParentObject()->Transform()->SetLocalRotationDegree(vTargetModuleRotation);


						// 자식 오브젝트로 넣는다.
						CObject::AddChildGameObjectEvn(m_pNearestModuleObj->GetParentObject(), m_pTargetObj->GetParentObject());

						// 연결하다.
						m_pTargetObj->GetComponent<CModuleScript_ca>()->ConnectModule(tNearestConnector);
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
				if (nullptr == m_pTargetObj && _pOther->GetGameObject()->GetParentObject()) {
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
