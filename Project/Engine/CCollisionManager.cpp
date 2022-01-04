#include "pch.h"
#include "CCollisionManager.h"
#include "CCore.h"
#include "CCollider.h"
#include "CCollider2D.h"

#include "CGameObject.h"

#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CScript.h"

#include "CRigidbody2D.h"

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Update()
{
	for (UINT iRow = 0; iRow < MAX_SIZE_LAYER; ++iRow) {
		for (UINT iCol = iRow; iCol < MAX_SIZE_LAYER; ++iCol) {
#pragma region Bit flag version
			// arrLayerFlag[iRow] & ( 1 << iCol ) => iRow���ο� iCol�ڸ� ��Ʈ�� 1���� üũ  
#pragma endregion

			if (m_bitsetCollisionGroup[iRow].test(iCol)) {
				CollisionByLayer(iRow, iCol);
			}
		}
	}
}

void CCollisionManager::CollisionByLayer(UINT _iLayerOneIdx, UINT _iLayerTwoIdx)
{
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	const vector<CGameObject*>& vecLeft = pCurScene->GetLayer(_iLayerOneIdx)->GetGameObjects();
	const vector<CGameObject*>& vecRight = pCurScene->GetLayer(_iLayerTwoIdx)->GetGameObjects();

	for (size_t l = 0; l < vecLeft.size(); ++l) {
		if (!vecLeft[l]->Collider2D())
			continue;

		size_t r = 0;
		if (_iLayerOneIdx == _iLayerTwoIdx)
			r = l;
		for (; r < vecRight.size(); ++r) {
			if (!vecRight[r]->Collider2D())
				continue;
			// �ڱ� �ڽŰ��� �ߵ��� ���
			if (vecLeft[l] == vecRight[r])
				continue;

			CCollider2D* pLeftCol = vecLeft[l]->Collider2D();
			CCollider2D* pRightCol = vecRight[r]->Collider2D();


			// �� �浹ü�� ���� ���̵� �����ؼ� Ű���� ����� ��
			COLLIDER_ID id = {};
			id.iLeft = pLeftCol->GetID();
			id.iRight = pRightCol->GetID();
			unordered_map<LONGLONG, bIsCollision>::iterator iter = m_unmapCollisionInfo.find(id.llID);
			if (iter == m_unmapCollisionInfo.end()) {
				m_unmapCollisionInfo.insert(std::make_pair(id.llID, false));
				iter = m_unmapCollisionInfo.find(id.llID);
			}

			bool bIsPushIntersection = false;
			TRigidCollisionInfo info = {};
			// Rigidbody�� �����ϸ� �� �浹ü ��� ��� Trigger�� false�� �о�� ��� ����
			/*if (vecLeft[l]->Rigidbody2D() && vecRight[r]->Rigidbody2D() &&
				!pLeftCol->IsTrigger() && !pRightCol->IsTrigger()) {
				bIsPushIntersection = true;
			}*/
			// ���ʸ��̶� Rigidbody�� �����ϸ鼭 ���� �� Trigger�� false��
			if (vecLeft[l]->Rigidbody2D() || vecRight[r]->Rigidbody2D() && !pLeftCol->IsTrigger() && !pRightCol->IsTrigger()) {
				bIsPushIntersection = true;
			}
			bool bIsActivePushIntersection = false;

			// �浹 �˻�
			if (IsCollision(pLeftCol, pRightCol, bIsPushIntersection, &info)) { // �浹���� ���
				if (iter->second) { // �������� �浹���� ��
					// �� �� �ϳ��� ���� ���� ������ ���
					if (vecLeft[l]->IsDead() || vecRight[r]->IsDead()) {
						// �浹�� �����.
						pLeftCol->OnCollisionExit2D(pRightCol);
						pRightCol->OnCollisionExit2D(pLeftCol);
						iter->second = false;
					}
					else {
						// �ϳ��� active�� �Ȱ� ������
						if (!pLeftCol->IsActive() || !pRightCol->IsActive()) {
							// �浹�� �����.
							pLeftCol->OnCollisionExit2D(pRightCol);
							pRightCol->OnCollisionExit2D(pLeftCol);
							iter->second = false;
						}
						else {
							// �о�� ���� �� 
							if (bIsPushIntersection) {
								bIsActivePushIntersection = true;
								pLeftCol->OnCollisionStay2D(pRightCol, &info);
								pRightCol->OnCollisionStay2D(pLeftCol, &info);
							}
							else {
								pLeftCol->OnCollisionStay2D(pRightCol);
								pRightCol->OnCollisionStay2D(pLeftCol);
							}
						}
					}

					// �о�� �����ϸ�
					if (bIsActivePushIntersection) {
						Vector3 vPos = info.pGameObject->Transform()->GetLocalPosition();
						vPos += info.vDir * info.fDistance;
						info.pGameObject->Transform()->SetLocalPosition(vPos);
					}
				}
				else {
					// �Ѵ� ���� ������ �ƴϰ� ó�� �浹 �� 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						if (pLeftCol->IsActive() && pRightCol->IsActive()) {
							pLeftCol->OnCollisionEnter2D(pRightCol);
							pRightCol->OnCollisionEnter2D(pLeftCol);
							iter->second = true;
						}
					}
				}
			}
			else { // �浹���� �ʾ��� ���
				if (iter->second) { // ������ �浹���� ���
					pLeftCol->OnCollisionExit2D(pRightCol);
					pRightCol->OnCollisionExit2D(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

void CCollisionManager::SetOnOffCollision(UINT _iLayerOne, UINT _iLayerTwo, bool _bIsOn)
{
	// Flag��(���) ������ �̿��ϸ� �ǹǷ� ���� �� ���������� ��ȯ
	UINT iRow = _iLayerOne < _iLayerTwo ? _iLayerOne : _iLayerTwo;
	UINT iCol = _iLayerOne > _iLayerTwo ? _iLayerOne : _iLayerTwo;

	if (_bIsOn)
		m_bitsetCollisionGroup[iRow].set(iCol); // iRowĭ�� iCol�� ��Ʈ�� 1�� ����
	else
		m_bitsetCollisionGroup[iRow].reset(iCol); // iRowĭ�� iCol�� ��Ʈ�� 0���� ����

#pragma region Bit flag version
			/*
if (_bIsOn)
	arrLayerFlag[iRow] = arrLayerFlag[iRow] | (1 << iCol); // iRowĭ�� iCol��Ʈ�� 1�� ����
else
	arrLayerFlag[iRow] = arrLayerFlag[iRow] & ~(1 << iCol); // iRowĭ�� iCol��Ʈ�� 0���� ����
*/
#pragma endregion

}

bool CCollisionManager::CheckCollisionLayer(UINT _iLayerOne, UINT _iLayerTwo)
{
	// Flag��(���) ������ �̿��ϸ� �ǹǷ� ���� �� ���������� ��ȯ
	UINT iRow = _iLayerOne < _iLayerTwo ? _iLayerOne : _iLayerTwo;
	UINT iCol = _iLayerOne > _iLayerTwo ? _iLayerOne : _iLayerTwo;

	if (m_bitsetCollisionGroup[iRow].test(iCol))
		return true;
	return false;
}

bool CCollisionManager::IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight, bool m_bPushIntersection, TRigidCollisionInfo* _pPushIntersectionInfo)
{
	{
		// FIXED : Collider2D���� Rect�θ� �Ǿ��ִµ� ���߿� �߰��Ǹ� Ÿ�Կ� ���� �б��� �� �ֵ��� �ϱ�

		if(m_bPushIntersection)
			return _IsCollision(_pLeft, _pRight, &(*_pPushIntersectionInfo));
		return _IsCollision(_pLeft, _pRight, nullptr);
	}

	return false;
}

// 2D Box, Point
bool CCollisionManager::IsCollision(CCollider2D* _pCol, const Vector3& _vPoint)
{
	static Vector3 arrLocal[4] = {
	Vector3(-0.5f, 0.5f, 0.f),
	Vector3(0.5f, 0.5f, 0.f),
	Vector3(0.5f,-0.5f, 0.f),
	Vector3(-0.5f,-0.5f, 0.f)
	};

	// �� vertex�� world��ǥ
	Vector3 vLT = XMVector3TransformCoord(arrLocal[0], _pCol->GetWorldMatrix());
	Vector3 vRT = XMVector3TransformCoord(arrLocal[1], _pCol->GetWorldMatrix());
	Vector3 vRB = XMVector3TransformCoord(arrLocal[2], _pCol->GetWorldMatrix());
	Vector3 vLB = XMVector3TransformCoord(arrLocal[3], _pCol->GetWorldMatrix());
	Vector3 vCenter = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), _pCol->GetWorldMatrix());
	vLT.z = vRT.z = vRB.z = vLB.z = vCenter.z = 0.f;

	// ���� ��
	Vector3 vProjRight = _pCol->Transform()->GetLocalRightVector();
	Vector3 vProjUp = _pCol->Transform()->GetLocalUpVector();
	
	const int iAxisCnt = 2;
	Vector3 vPoint = Vector3(_vPoint.x, _vPoint.y, 0.f);

	Vector3 vPointToCenter = vCenter - vPoint;
	Vector3 vToRight = vRT - vLT;
	Vector3 vToUp = vLT - vLB;

	bool bIsMouseInBox1 = false;
	bool bIsMouseInBox2 = false;

	float fLength = fabsf(vProjRight.Dot(vPointToCenter));
	float fRectLength = fabsf(vProjRight.Dot(vToRight));
	if (fLength <= fRectLength)
		bIsMouseInBox1 = true;

	fLength = fabsf(vProjUp.Dot(vPointToCenter));
	fRectLength = fabsf(vProjUp.Dot(vToUp));
	if (fLength <= fRectLength)
		bIsMouseInBox2 = true;

	if (bIsMouseInBox1 && bIsMouseInBox2)
		return true;
	return false;
}

bool CCollisionManager::IsCollision(CRectTransform* _pRT, const Vector2& _vMousePosition)
{
	// FIXED (Jang) : ����ȭ �ʿ�.
	CCamera* pUICam = CRenderManager::GetInstance()->GetUICamera();
	if (!pUICam) {
		assert(pUICam && _T("UI Camera ����"));
		return false;
	}

	// �߽������κ��� width, height�� ���ؼ� ��ġ�� ����
	if (nullptr == _pRT) {
		assert(_pRT && _T("RectTransform ����"));
		return false;
	}
	Vector3 vCenterPos = _pRT->GetPosition();
	Vector2 vScreenCenter = pUICam->GetWorldToScreen2DPosition(vCenterPos);
	Vector3 vLocalScale = _pRT->GetLocalScale();

	float fHalfWidth = fabsf(_pRT->GetWidth()) * 0.5f * vLocalScale.x;
	float fHalfHeight = fabsf(_pRT->GetHeight()) * 0.5f * vLocalScale.y;
	float vRotZ = _pRT->GetRotationDegree().z;

	// Screen��ǥ�� �𼭸� ��ġ�� ������.
	Vector2 vLT = GetScreenPosFromCenter(Vector2(vScreenCenter.x - fHalfWidth, vScreenCenter.y - fHalfHeight), vScreenCenter, vRotZ);

	Vector2 vRightPointPos = GetScreenPosFromCenter(Vector2(vScreenCenter.x + fHalfWidth, vScreenCenter.y), vScreenCenter, vRotZ);
	Vector2 vUpPointPos = GetScreenPosFromCenter(Vector2(vScreenCenter.x, vScreenCenter.y - fHalfHeight), vScreenCenter, vRotZ);
	Vector2 vRDir = vRightPointPos - vScreenCenter;
	vRDir.Normalize();
	Vector2 vUpDir = vUpPointPos - vScreenCenter;
	vUpDir.Normalize();

	Vector2 vMousePosToCenter = Vector2{ vScreenCenter.x - _vMousePosition.x, vScreenCenter.y - _vMousePosition.y };
	Vector2 vEdgePosToCenter = Vector2{ vScreenCenter.x - vLT.x, vScreenCenter.y - vLT.y };

	bool isInRightVec = false;
	{
		float fCenterLen = fabsf(vRDir.Dot(vMousePosToCenter));
		float fRightLen = fabsf(vRDir.Dot(vEdgePosToCenter));
		if (fCenterLen < fRightLen)
			isInRightVec = true;
	}

	bool isInUpVec = false;
	{
		float fCenterLen = fabsf(vUpDir.Dot(vMousePosToCenter));
		float fUpLen = fabsf(vUpDir.Dot(vEdgePosToCenter));
		if (fCenterLen < fUpLen)
			isInUpVec = true;
	}

	if (isInUpVec && isInRightVec)
		return true;
	return false;
}

// OBB Check
bool CCollisionManager::_IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight, TRigidCollisionInfo* _tRigidColInfo)
{
	// ���� ��ǥ
	/*
	0-----1
	|     |
	|     |
	3-----2
	*/
	static Vector3 arrLocal[4] = {
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3( 0.5f, 0.5f, 0.f),
		Vector3( 0.5f,-0.5f, 0.f),
		Vector3(-0.5f,-0.5f, 0.f)
	};

#pragma region Tip
	// XMVector3TransformCoord
// �־��� ��ķ� 3D ���͸� ��ȯ�Ͽ� ����� �ٽ� w = 1�� �����մϴ�.
//�Է� ������ w ���� ��Ҹ� �����ϰ� ��� 1.0 ���� ����մϴ�.��ȯ �� ������ w ���� ��Ҵ� �׻� 1.0�Դϴ�.

// ���̴� �ڵ�� ġ�� mul(float4(arrLoca[0], 0.f), _pLeft->GetWorldMat()); ��.
// 
// XMVector3TransformNormal(arrLocal[2], _pLeft->GetWorldMatrix());�� ���� �Ǹ� 4��° ��Ҹ� 0���� �������ش�.  
#pragma endregion
	// �� ���� ��ġ���� �浹ü ��ġ�� ������
	Vector3 v0 = XMVector3TransformCoord(arrLocal[0], _pLeft->GetWorldMatrix()); // �̵� ��ǥ�� ������ �ֵ��� 4��° ��Ұ��� 1�� Ȯ��
	v0.z = 0.f;
	Vector3 v1 = XMVector3TransformCoord(arrLocal[1], _pLeft->GetWorldMatrix());
	v1.z = 0.f;
	Vector3 v3 = XMVector3TransformCoord(arrLocal[3], _pLeft->GetWorldMatrix());
	v3.z = 0.f;
	
	// ������ ���
	Vector3 vToRight1 = v1 - v0;
	Vector3 vToDown1 = v3 - v0;

	// �ٸ� �簢���� ���� �� ���ϱ�
	v0 = XMVector3TransformCoord(arrLocal[0], _pRight->GetWorldMatrix());
	v0.z = 0.f;
	v1 = XMVector3TransformCoord(arrLocal[1], _pRight->GetWorldMatrix());
	v1.z = 0.f;
	v3 = XMVector3TransformCoord(arrLocal[3], _pRight->GetWorldMatrix());
	v3.z = 0.f;

	// ������ ���
	Vector3 vToRight2 = v1 - v0;
	Vector3 vToDown2 = v3 - v0;

	// �� �������� ��� �˻��ϱ� ���� �迭�� �־���
	Vector3 arrProjAxis[4] = { vToRight1 , vToDown1 , vToRight2 , vToDown2 };

	Vector3 vCenterPosLeftCollider = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), _pLeft->GetWorldMatrix());
	Vector3 vCenterPosRightCollider = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), _pRight->GetWorldMatrix());

	Vector3 vCenter = vCenterPosRightCollider - vCenterPosLeftCollider;

	// ��� ������ �����Ǿ� �и����� �������� �˻�
	for (int i = 0; i < 4; ++i) {
		Vector3 vAxisDir = arrProjAxis[i];
		vAxisDir.Normalize(); // ���� ���ͷ� ����

		float fProjTwoBoxesDis = 0.f;
		for (int j = 0; j < 4; ++j)
			fProjTwoBoxesDis += abs(vAxisDir.Dot(arrProjAxis[j]));

		float fProjCenterDis = abs(vAxisDir.Dot(vCenter)); // ���ͱ��̸� �������Ѽ� ���̸� ���Ѵ�.

		if (fProjCenterDis > fProjTwoBoxesDis * 0.5f) {
			return false;
		}


	
	}

	if (_tRigidColInfo) {
		Vector3 vForceDir;
		CRigidbody2D* pRigid = nullptr;
		vector<Vector3> vecProj;
		vector<Vector3> vecNoRigid;
		if (_pLeft->Rigidbody2D() && _pRight->Rigidbody2D()) {
			float fLMass = _pLeft->Rigidbody2D()->GetMass();
			float fRMass = _pRight->Rigidbody2D()->GetMass();
			if (fLMass < fRMass) {
				pRigid = _pLeft->Rigidbody2D();
				vForceDir = vCenterPosLeftCollider - vCenterPosRightCollider;
				_tRigidColInfo->pGameObject = _pLeft->GetGameObject();
				vecProj.push_back(vToRight2);
				vecProj.push_back(vToDown2);
				vecProj.push_back(-vToRight2);
				vecProj.push_back(-vToDown2);
				vecNoRigid.push_back(vToRight1);
				vecNoRigid.push_back(vToDown1);
			}
			else {
				pRigid = _pRight->Rigidbody2D();
				vForceDir = vCenterPosRightCollider - vCenterPosLeftCollider;
				_tRigidColInfo->pGameObject = _pRight->GetGameObject();
				vecProj.push_back(vToRight1);
				vecProj.push_back(vToDown1);
				vecProj.push_back(-vToRight1);
				vecProj.push_back(-vToDown1);
				vecNoRigid.push_back(vToRight2);
				vecNoRigid.push_back(vToDown2);
			}
		}
		else {
			if (_pLeft->Rigidbody2D()) {
				pRigid = _pLeft->Rigidbody2D();
				vForceDir = vCenterPosLeftCollider - vCenterPosRightCollider;
				_tRigidColInfo->pGameObject = _pLeft->GetGameObject();
				vecProj.push_back(vToRight2);
				vecProj.push_back(vToDown2);
				vecProj.push_back(-vToRight2);
				vecProj.push_back(-vToDown2);
				vecNoRigid.push_back(vToRight1);
				vecNoRigid.push_back(vToDown1);
			}

			else if (_pRight->Rigidbody2D()) {
				pRigid = _pRight->Rigidbody2D();
				vForceDir = vCenterPosRightCollider - vCenterPosLeftCollider;
				_tRigidColInfo->pGameObject = _pRight->GetGameObject();
				vecProj.push_back(vToRight1);
				vecProj.push_back(vToDown1);
				vecProj.push_back(-vToRight1);
				vecProj.push_back(-vToDown1);
				vecNoRigid.push_back(vToRight2);
				vecNoRigid.push_back(vToDown2);
			}
		}

		// Rigidbody�� ���� Collider�� right,down�� ���⺤�� �������� �з������� �Ÿ� ���
			// TODO (Jang) : �ٸ��� �鿡 ������� �����̵� ��. �𼭸��� ������ ������ �� ����. 

		vForceDir.Normalize();
		float fShortedGetLen = FLOAT_MAX;
		Vector3 vShortProj = {};
		Vector3 vForceDirection = {};
		for (int i = 0; i < vecProj.size(); ++i) {
			Vector3 vProj = vecProj[i];
			vProj.Normalize();
			float vLen = fabsf(vProj.Dot(vecProj[i]));
			float len1 = vProj.Dot(vToRight1);
			float len2 = vProj.Dot(vToDown1);
			float rigidLen = 0.f;
			for (int i = 0; i < vecProj.size(); ++i)
				rigidLen += fabsf(vProj.Dot(vecProj[i]));
			rigidLen *= 0.5f;

			float noRigidLen = 0.f;
			for (int i = 0; i < vecNoRigid.size(); ++i)
				noRigidLen += fabsf(vProj.Dot(vecNoRigid[i]));
			noRigidLen *= 0.5f;

			float fCenterLen = fabsf(vProj.Dot(vCenter));
			fCenterLen *= 0.5f;
			

			float fGepLen = 0.f;
			if (rigidLen + noRigidLen > fCenterLen) { // ������.
				// ������ ���̸� ����.
				fGepLen = fabsf(rigidLen - noRigidLen);
				fGepLen = 0.01f;
			}

			// �������� Ȯ���ϱ�
			if (vForceDir.Dot(vProj) >= 0) {
				if (fGepLen < fShortedGetLen) {
					fShortedGetLen = fGepLen;
					vForceDirection = vProj;
				}
			}
			else {
				int i = 0;
			}
		}
		_tRigidColInfo->vDir = vForceDirection;
		_tRigidColInfo->fDistance = fShortedGetLen;
		//// �밢������ �з������� �Ÿ� ���
		//Vector3 vCenter = vForceDir;
		//vForceDir.Normalize();

		//float centerlen = fabsf(vForceDir.Dot(vCenter));

		//float len1 = fabsf(vForceDir.Dot(vToRight1));
		//float len2 = fabsf(vForceDir.Dot(vToDown1));
		//float len3 = fabsf(vForceDir.Dot(vToRight2));
		//float len4 = fabsf(vForceDir.Dot(vToDown2));

		//float resultLen2 = (len1 + len2 + len3 + len4) * 0.5f - centerlen;

		//// �Ÿ��� ª�� ������ ����
		//if (shortLen < resultLen2) {
		//	_tRigidColInfo->vDir = vForceDirection;
		//	_tRigidColInfo->fDistance = shortLen;
		//}
		//else {

		//	_tRigidColInfo->vDir = vForceDir;
		//	_tRigidColInfo->fDistance = resultLen2;
		//}
	}
	return true;
}