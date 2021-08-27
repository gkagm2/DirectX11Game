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

			// ���� ����
			if (m_pTargetObj->GetParentObject() && m_pTargetObj->GetParentObject()->GetParentObject()) {
				CObject::UnlinkParentGameObjectEvn(m_pTargetObj->GetParentObject());
				// ���� ����
				m_pTargetObj->GetComponent<CModuleScript_ca>()->DisConnectModule();
			}
			else {
				// ���� ����� ��� ������Ʈ�� �˻��Ѵ�.
				m_pNearestModuleObj = FindNearestModuleObject(vWorldMousePos);
				if (m_pNearestModuleObj && m_pNearestModuleObj->GetParentObject()) {
					// ������Ʈ�� �����ϸ�
					Vector3 vNearestPos = {};

					// ���� ����� ����� ���� ������ ��ġ ã��
					CModuleScript_ca* pNearestModule = m_pNearestModuleObj->GetComponent< CModuleScript_ca>();
					vNearestPos = pNearestModule->FindNearestConnectionPosition(vWorldMousePos);

					float fDistance = Vector3::Distance(vNearestPos, vWorldMousePos);

					if (fDistance > m_fMaxConnectableDistance)
						m_bIsConnectableOtherObj = false;
					else { // ���� ������ �Ÿ���
						TModuleConnector_ca& tNearestConnector = pNearestModule->FindNearestConnector(vWorldMousePos);
						TModuleConnector_ca& tDraggedMainConnector = m_pTargetObj->GetComponent<CModuleScript_ca>()->MainConnector();

						// ���� ����� ���ᱸ�� ���Ѵ�.
						// ����ִ� ���ᱸ��
						if (tNearestConnector.bIsConnectable) {
							// TODO (Jang) : Effect�� Ƣ����.
							m_bIsConnectableOtherObj = true;

							Vector3 vOtherModuleRotation = m_pNearestModuleObj->GetParentObject()->Transform()->GetLocalRotationDegree();
							Vector3 vTargetModuleRotation = m_pTargetObj->GetParentObject()->Transform()->GetLocalRotationDegree();

							// ȸ�� �� ���� ���ϱ�

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
			// ���� ����� ��� ������Ʈ�� �˻��Ѵ�.
			if(m_pNearestModuleObj && m_pNearestModuleObj->GetParentObject()) {
				if (m_bIsConnectableOtherObj) {
					TModuleConnector_ca& tNearestConnector = m_pNearestModuleObj->GetComponent<CModuleScript_ca>()->FindNearestConnector(vWorldMousePos);

					if (tNearestConnector.bIsConnectable) {
						Vector3 vConnectPos = tNearestConnector.vPosition;

						Vector3 offset = m_pNearestModuleObj->Transform()->GetLocalPosition();

						// ��ġ�� ���� ���δ�
						m_pTargetObj->GetParentObject()->Transform()->SetLocalPosition(vConnectPos + offset);

						// ȸ���� �ؾ� �Ѵ�.
						Vector3 vOtherModuleRotation = m_pNearestModuleObj->GetParentObject()->Transform()->GetLocalRotationDegree();
						Vector3 vTargetModuleRotation = m_pTargetObj->GetParentObject()->Transform()->GetLocalRotationDegree();

						// ȸ�� �� ���� ���ϱ�

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


						// �ڽ� ������Ʈ�� �ִ´�.
						CObject::AddChildGameObjectEvn(m_pNearestModuleObj->GetParentObject(), m_pTargetObj->GetParentObject());

						// �����ϴ�.
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
	// Ŭ���Ѱ��̶��
	if (0 == _pOther->GetGameObject()->GetLayer()) {

		// ����� Ŭ��������
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
	// ���콺 ��ġ�������� ���� �Ÿ� �ȿ� ������ ������Ʈ�� ã�´�.
	static UINT iFindLayer = 0;
	static UINT iModuleTag = 0;

	_vecObjects.clear();
	
	// 1. ���̾��� ��� ������Ʈ�� �����´�.
	vector<CGameObject*> vecObjects;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecObjects, iFindLayer);

	// ��� ������Ʈ���� Module ������Ʈ�� ���ִ� ������Ʈ�� ã�Ƽ� ����Ʈ�� �ִ´�.
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
	// ���콺 ��ġ�������� ���� �Ÿ� �ȿ� ������ ������Ʈ�� ã�´�.
	static UINT iFindLayer = 0;
	static UINT iModuleTag = 0;
	CGameObject* pNearestModuleObj = nullptr;

	// 1. ���̾��� ��� ������Ʈ�� �����´�.

	vector<CGameObject*> vecObjects;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecObjects, iFindLayer);

	// ��� ������Ʈ���� Module ������Ʈ�� ���ִ� ������Ʈ �� ���� ����� ������Ʈ�� ����.
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
