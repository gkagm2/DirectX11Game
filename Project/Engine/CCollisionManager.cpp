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
#include "CRigidbody.h"

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

	if (InputKeyPress(E_Key::F9))
		m_bCollisionShow = m_bCollisionShow == true ? false : true;
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
			if (vecLeft[l]->Rigidbody2D() || vecRight[r]->Rigidbody2D()) {
				if (!pLeftCol->IsTrigger() && !pRightCol->IsTrigger()) {
					// ������ ���� 
					if(vecLeft[l]->IsActive() && vecRight[r]->IsActive() &&
						pLeftCol->IsActive() && pRightCol->IsActive())
						bIsPushIntersection = true;
				}
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
						if (!vecLeft[l]->IsActive() || !vecRight[r]->IsActive() ||
							!pLeftCol->IsActive() || !pRightCol->IsActive()) {
							// �浹�� �����.
							pLeftCol->OnCollisionExit2D(pRightCol);
							pRightCol->OnCollisionExit2D(pLeftCol);
							iter->second = false;
						}
						else {
							pLeftCol->OnCollisionStay2D(pRightCol);
							pRightCol->OnCollisionStay2D(pLeftCol);
							if(bIsPushIntersection)
								bIsActivePushIntersection = true;
						}

						// �о�� �����ϸ�
						if (bIsActivePushIntersection) {
							if (info.pGameObject == nullptr)
								assert(nullptr);
							Vector3 vPos = info.pGameObject->Transform()->GetLocalPosition();
							vPos += info.vDir * info.fDistance;
							info.pGameObject->Transform()->SetLocalPosition(vPos);
						}
					}
				}
				else {
					// �Ѵ� ���� ������ �ƴϰ� ó�� �浹 �� 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						if (
							vecLeft[l]->IsActive() && 
							vecRight[r]->IsActive() && 
							pLeftCol->IsActive() && 
							pRightCol->IsActive()) {
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
			return _IsCollision(_pLeft, _pRight, &_pPushIntersectionInfo);
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
	Vector3 vProjRight = _pCol->Transform()->GetRightVector();
	Vector3 vProjUp = _pCol->Transform()->GetUpVector();
	
	const int iAxisCnt = 2;
	Vector3 vPoint = Vector3(_vPoint.x, _vPoint.y, 0.f);

	Vector3 vPointToCenter = vCenter - vPoint;
	Vector3 vToRight = (vRT - vLT) * 0.5f;
	Vector3 vToUp = (vLT - vLB) * 0.5f;

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
bool CCollisionManager::_IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight, TRigidCollisionInfo** _tRigidColInfo)
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
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f,-0.5f, 0.f),
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
		// Rigidbody ������Ʈ�� �����Ѵٸ� 
		static Vector3 vForceDir{};
		static Vector3 vToCenter{};
		CRigidbody2D* pRigid = nullptr;
		static Vector3 vecProj[4]{};
		static Vector3 vecNoRigid[2]{};
		static Vector3 vecRigid[2]{};

		if (_pLeft->Rigidbody2D() && _pRight->Rigidbody2D()) {
			float fLMass = _pLeft->Rigidbody2D()->GetMass();
			float fRMass = _pRight->Rigidbody2D()->GetMass();
			if (fLMass < fRMass) {
				pRigid = _pLeft->Rigidbody2D();
				vToCenter = vCenterPosLeftCollider - vCenterPosRightCollider;
				vForceDir = vToCenter;
				vForceDir.Normalize();
				(*_tRigidColInfo)->pGameObject = _pLeft->GetGameObject();
				vecProj[0] =	vToRight2;
				vecProj[1] =	vToDown2;
				vecProj[2] =	-vToRight2;
				vecProj[3] =	-vToDown2;
				vecRigid[0] =	vToRight2;
				vecRigid[1] =	vToDown2;
				vecNoRigid[0]=	vToRight1;
				vecNoRigid[1]=	vToDown1;
			}
			else {
				pRigid = _pRight->Rigidbody2D();
				vToCenter = vCenterPosRightCollider - vCenterPosLeftCollider;
				vForceDir = vToCenter;
				vForceDir.Normalize();
				(*_tRigidColInfo)->pGameObject = _pRight->GetGameObject();
				vecProj[0] = vToRight1;
				vecProj[1] = vToDown1;
				vecProj[2] = -vToRight1;
				vecProj[3] = -vToDown1;
				vecRigid[0] = vToRight1;
				vecRigid[1] = vToDown1;
				vecNoRigid[0] = vToRight2;
				vecNoRigid[1] = vToDown2;
			}
		}
		else {
			if (_pLeft->Rigidbody2D()) {
				pRigid = _pLeft->Rigidbody2D();
				vToCenter = vCenterPosLeftCollider - vCenterPosRightCollider;
				vForceDir = vToCenter;
				vForceDir.Normalize();
				(*_tRigidColInfo)->pGameObject = _pLeft->GetGameObject();
				vecProj[0] = vToRight2;
				vecProj[1] = vToDown2;
				vecProj[2] = -vToRight2;
				vecProj[3] = -vToDown2;
				vecRigid[0] = vToRight2;
				vecRigid[1] = vToDown2;
				vecNoRigid[0] = vToRight1;
				vecNoRigid[1] = vToDown1;
			}

			else if (_pRight->Rigidbody2D()) {
				pRigid = _pRight->Rigidbody2D();
				vToCenter = vCenterPosRightCollider - vCenterPosLeftCollider;
				(*_tRigidColInfo)->pGameObject = _pRight->GetGameObject();
				vForceDir = vToCenter;
				vForceDir.Normalize();
				vecProj[0] = vToRight1;
				vecProj[1] = vToDown1;
				vecProj[2] = -vToRight1;
				vecProj[3] = -vToDown1;
				vecRigid[0] = vToRight1;
				vecRigid[1] = vToDown1;
				vecNoRigid[0] = vToRight2;
				vecNoRigid[1] = vToDown2;
			}
		}

		float fShortestLen = FLOAT_MAX;
		Vector3 vForceDirection = {};
		const int iProjSize = 4;
		const int iRigidSize = 2;
		const int iNoRigidSize = 2;
		for (int i = 0; i < iProjSize; ++i) {
			Vector3 vProjDir = vecProj[i]; // ������ ���� ����
			Vector3 vProjVec = vecProj[i]; // ������ ������ ���� ũ��
			vProjDir.Normalize();

			float rigidLen = 0.f; // rigidBody �ִ� �浹ü�� �߽������� ���������κ����� ����
			for (int j = 0; j < iRigidSize; ++j)
				rigidLen += fabsf(vProjDir.Dot(vecRigid[j]));
			rigidLen *= 0.5f;

			float noRigidLen = 0.f; // rigidbody�� ���� �浹ü�� �߽������� ���������κ����� ����
			for (int j = 0; j < iNoRigidSize; ++j)
				noRigidLen += fabsf(vProjDir.Dot(vecNoRigid[j]));
			noRigidLen *= 0.5f;

			// ��� ���̸� ����
			float fCenterLen = fabsf(vProjDir.Dot(vToCenter));

			float fGepLen = 0.f;
			if (rigidLen + noRigidLen > fCenterLen) { // ��������
				fGepLen = rigidLen + noRigidLen - fCenterLen; // ������ ���̸� ����.
			}

			// �������� Ȯ���ϱ�
			if (vForceDir.Dot(vProjDir) >= 0) {
				if (fGepLen < fShortestLen) {
					fShortestLen = fGepLen;
					vForceDirection = vProjDir;
				}
			}
		}
		(*_tRigidColInfo)->vDir = vForceDirection;
		(*_tRigidColInfo)->fDistance = fShortestLen;
	}
	return true;
}