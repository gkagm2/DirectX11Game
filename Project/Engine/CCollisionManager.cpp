#include "pch.h"
#include "CCollisionManager.h"
#include "CCore.h"
#include "CCollider.h"
#include "CCollider2D.h"
#include "CCollider2DRect.h"

#include "CGameObject.h"

#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CScript.h"


CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Update()
{
	for (UINT iRow = 0; iRow < MAX_SIZE_LAYER; ++iRow) {
		for (UINT iCol = 0; iCol < MAX_SIZE_LAYER; ++iCol) {
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
		for (size_t r = 0; r < vecRight.size(); ++r) {
			if (!vecRight[r]->Collider2D())
				continue;
			if (_iLayerOneIdx == _iLayerTwoIdx && l == r)
				continue;

			// �� �浹ü�� ���� ���̵� �����ؼ� Ű���� ����� ��
			COLLIDER_ID id = {};
			id.iLeft = vecLeft[l]->Collider2D()->GetID();
			id.iRight = vecRight[r]->Collider2D()->GetID();
			unordered_map<LONGLONG, bIsCollision>::iterator iter = m_unmapCollisionInfo.find(id.llID);
			if (iter == m_unmapCollisionInfo.end()) {
				m_unmapCollisionInfo.insert(std::make_pair(id.llID, false));
				iter = m_unmapCollisionInfo.find(id.llID);
			}
#pragma region Active�� ���� ���
			// �浹 �˻�
			if (IsCollision(vecLeft[l]->Collider2D(), vecRight[r]->Collider2D())) { // �浹���� ���
				if (iter->second) { // �������� �浹���� ��
					// �� �� �ϳ��� ���� ���� ������ ���
					if (vecLeft[l]->IsDead() || vecRight[r]->IsDead()) {
						// �浹�� �����.
						vecLeft[l]->Collider2D()->OnCollisionExit2D(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionExit2D(vecLeft[l]->Collider2D());
						iter->second = false;
					}
					else {
						vecLeft[l]->Collider2D()->OnCollisionStay2D(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionStay2D(vecLeft[l]->Collider2D());
					}
				}
				else {
					// �Ѵ� ���� ������ �ƴϰ� ó�� �浹 �� 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						vecLeft[l]->Collider2D()->OnCollisionEnter2D(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionEnter2D(vecLeft[l]->Collider2D());
						iter->second = true;
					}
				}
			}
			else { // �浹���� �ʾ��� ���
				if (iter->second) { // ������ �浹���� ���
					vecLeft[l]->Collider2D()->OnCollisionExit2D(vecRight[r]->Collider2D());
					vecRight[r]->Collider2D()->OnCollisionExit2D(vecLeft[l]->Collider2D());
					iter->second = false;
				}
			}
#pragma endregion

#pragma region Active�� ���� ���
			/*
			// �浹 �˻�
			if (IsCollision(vecLeft[l]->Collider2D(), vecRight[r]->Collider2D())) { // �浹���� ���
				if (iter->second) { // �������� �浹���� ��
					// �� �� �ϳ��� ���� ���� ������ ���
					if (vecLeft[l]->IsDead() || vecRight[r]->IsDead()) {
						// �浹�� �����.
						vecLeft[l]->Collider2D()->OnCollisionExit(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionExit(vecLeft[l]->Collider2D());
						iter->second = false;
					}
					else {
						// �ϳ��� active�� false�� �Ȱ� ������
						if (!vecLeft[l]->Collider2D()->IsActive() || !vecRight[r]->Collider2D()->IsActive()) {
							// �浹�� �����.
							vecLeft[l]->Collider2D()->OnCollisionExit(vecRight[r]->Collider2D());
							vecRight[r]->Collider2D()->OnCollisionExit(vecLeft[l]->Collider2D());
							iter->second = false;
						}
						else {
							vecLeft[l]->Collider2D()->OnCollisionStay(vecRight[r]->Collider2D());
							vecRight[r]->Collider2D()->OnCollisionStay(vecLeft[l]->Collider2D());
						}
					}
				}
				else {
					// �Ѵ� ���� ������ �ƴϰ� ó�� �浹 �� 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						if (vecLeft[l]->Collider2D()->IsActive() && vecRight[r]->Collider2D()->IsActive()) {
							vecLeft[l]->Collider2D()->OnCollisionEnter(vecRight[r]->Collider2D());
							vecRight[r]->Collider2D()->OnCollisionEnter(vecLeft[l]->Collider2D());
							iter->second = true;
						}
					}
				}
			}
			else { // �浹���� �ʾ��� ���
				if (iter->second) { // ������ �浹���� ���
					vecLeft[l]->Collider2D()->OnCollisionExit(vecRight[r]->Collider2D());
					vecRight[r]->Collider2D()->OnCollisionExit(vecLeft[l]->Collider2D());
					iter->second = false;
				}
			}
			*/
#pragma endregion
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

bool CCollisionManager::IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	{
		auto pLeft = dynamic_cast<CCollider2DRect*>(_pLeft);
		auto pRight = dynamic_cast<CCollider2DRect*>(_pRight);
		if (pLeft && pRight)
			return _IsCollision(pLeft, pRight);
	}

	return false;
}

// OBB Check
bool CCollisionManager::_IsCollision(CCollider2DRect* _pLeft, CCollider2DRect* _pRight)
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
	Vector3 vToLeft1 = v1 - v0;
	Vector3 vToDown1 = v3 - v0;

	// �ٸ� �簢���� ���� �� ���ϱ�
	v0 = XMVector3TransformCoord(arrLocal[0], _pRight->GetWorldMatrix());
	v0.z = 0.f;
	v1 = XMVector3TransformCoord(arrLocal[1], _pRight->GetWorldMatrix());
	v1.z = 0.f;
	v3 = XMVector3TransformCoord(arrLocal[3], _pRight->GetWorldMatrix());
	v3.z = 0.f;

	// ������ ���
	Vector3 vToLeft2 = v1 - v0;
	Vector3 vToDown2 = v3 - v0;

	// �� �������� ��� �˻��ϱ� ���� �迭�� �־���
	Vector3 arrProjAxis[4] = { vToLeft1, vToDown1, vToLeft2, vToDown2 };

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

		if (fProjCenterDis > fProjTwoBoxesDis * 0.5f)
			return false;
	}

	return true;
}