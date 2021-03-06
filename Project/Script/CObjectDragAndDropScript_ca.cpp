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

			// ???? ????
			if (m_pTargetObj->GetParentObject() && m_pTargetObj->GetParentObject()->GetParentObject()) {
				CObject::UnlinkParentGameObjectEvn(m_pTargetObj->GetParentObject());
				// ???? ????
				m_pTargetObj->GetComponent<CModuleScript_ca>()->DisConnectModule();
			}
			else {
				// ???? ?????? ???? ?????????? ????????.
				m_pNearestModuleObj = FindNearestModuleObject(vWorldMousePos);
				if (m_pNearestModuleObj && m_pNearestModuleObj->GetParentObject()) {
					// ?????????? ????????
					Vector3 vNearestPos = {};

					// ???? ?????? ?????? ???? ?????? ???? ????
					CModuleScript_ca* pNearestModule = m_pNearestModuleObj->GetComponent< CModuleScript_ca>();
					vNearestPos = pNearestModule->FindNearestConnectionPosition(vWorldMousePos);

					float fDistance = Vector3::Distance(vNearestPos, vWorldMousePos);

					if (fDistance > m_fMaxConnectableDistance)
						m_bIsConnectableOtherObj = false;
					else { // ???? ?????? ??????
						TModuleConnector_ca& tNearestConnector = pNearestModule->FindNearestConnector(vWorldMousePos);
						TModuleConnector_ca& tDraggedMainConnector = m_pTargetObj->GetComponent<CModuleScript_ca>()->MainConnector();

						// ???? ?????? ???????? ??????.
						// ???????? ????????
						if (tNearestConnector.bIsConnectable) {
							// TODO (Jang) : Effect?? ??????.
							m_bIsConnectableOtherObj = true;

							Vector3 vOtherModuleRotation = m_pNearestModuleObj->GetParentObject()->Transform()->GetLocalRotationDegree();
							Vector3 vTargetModuleRotation = m_pTargetObj->GetParentObject()->Transform()->GetLocalRotationDegree();

							// ???? ?? ???? ??????

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
			// ???? ?????? ???? ?????????? ????????.
			if(m_pNearestModuleObj && m_pNearestModuleObj->GetParentObject()) {
				if (m_bIsConnectableOtherObj) {
					TModuleConnector_ca& tNearestConnector = m_pNearestModuleObj->GetComponent<CModuleScript_ca>()->FindNearestConnector(vWorldMousePos);

					if (tNearestConnector.bIsConnectable) {
						Vector3 vConnectPos = tNearestConnector.vPosition;

						Vector3 offset = m_pNearestModuleObj->Transform()->GetLocalPosition();

						// ?????? ???? ??????
						m_pTargetObj->GetParentObject()->Transform()->SetLocalPosition(vConnectPos + offset);

						// ?????? ???? ????.
						Vector3 vOtherModuleRotation = m_pNearestModuleObj->GetParentObject()->Transform()->GetLocalRotationDegree();
						Vector3 vTargetModuleRotation = m_pTargetObj->GetParentObject()->Transform()->GetLocalRotationDegree();

						// ???? ?? ???? ??????

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


						// ???? ?????????? ??????.
						CObject::AddChildGameObjectEvn(m_pNearestModuleObj->GetParentObject(), m_pTargetObj->GetParentObject());

						// ????????.
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
	// ??????????????
	if (0 == _pOther->GetGameObject()->GetLayer()) {

		// ?????? ??????????
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
	// ?????? ???????????? ???? ???? ???? ???????? ?????????? ??????.
	static UINT iFindLayer = 0;
	static UINT iModuleTag = 0;

	_vecObjects.clear();
	
	// 1. ???????? ???? ?????????? ????????.
	vector<CGameObject*> vecObjects;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecObjects, iFindLayer);

	// ???? ???????????? Module ?????????? ?????????? ?????????? ?????? ???????? ??????.
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
	// ?????? ???????????? ???? ???? ???? ???????? ?????????? ??????.
	static UINT iFindLayer = 0;
	static UINT iModuleTag = 0;
	CGameObject* pNearestModuleObj = nullptr;

	// 1. ???????? ???? ?????????? ????????.

	vector<CGameObject*> vecObjects;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecObjects, iFindLayer);

	// ???? ???????????? Module ?????????? ?????????? ???????? ?? ???? ?????? ?????????? ??????.
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
