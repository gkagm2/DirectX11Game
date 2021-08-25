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
	m_pNearestModuleObj(nullptr),
	m_fMaxSearchObjDistance(600.f),
	m_fMaxConnectableDistance(25.f),
	m_bIsConnectableOtherObj(false)
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
				// ����� Main���� ��ġ�� ���Ѵ�.
				Vector3 vModulePosition = m_pModuleScript->GetMainConnectionPosition();

				// ����� ���� ��ġ�� ������Ʈ�� ��ġ ���� ���Ѵ�.
				vOffsetPos = vModulePosition - vTargetObjPos;
			}

			// ���� ����� ��� ������Ʈ�� �˻��Ѵ�.
			m_pNearestModuleObj = FindNearestModuleObject(vWorldMousePos);

			// ������Ʈ�� �����ϸ�
			if (m_pNearestModuleObj) {
				Vector3 vNearestPos = {};

				// ���� ����� ����� ���� ������ ��ġ ã��
				CModuleScript_ca* pModule = m_pNearestModuleObj->GetComponent< CModuleScript_ca>();
				vNearestPos = pModule->FindNearestConnectionPosition(vWorldMousePos);

				float fDistance = Vector3::Distance(vNearestPos, vWorldMousePos);
			
				// ���� ������ �Ÿ���
				if (fDistance <= m_fMaxConnectableDistance) {
					TModuleConnector_ca& tConnector = m_pNearestModuleObj->GetComponent<CModuleScript_ca>()->FindNearestConnector(vWorldMousePos);

					// ���� ����� ���ᱸ�� ���Ѵ�.

					// ������ ���� �ʴ� ���ᱸ��
					if (tConnector.bIsConnectable) {
						// TODO (Jang) : Effect�� Ƣ����.
						m_bIsConnectableOtherObj = true;
					}
					// ���� ��� �ִ� ������Ʈ�� ȸ���ϰ� �ȴ�.
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
						m_pTargetObj->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
						m_pTargetObj->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
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
	// Ŭ���Ѱ��̶��
	if (0 == _pOther->GetGameObject()->GetLayer()) {

		// ����� Ŭ��������
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
