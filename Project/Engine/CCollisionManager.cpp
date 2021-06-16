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


CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Update()
{
	for (UINT iRow = 0; iRow < (UINT)E_Layer::End; ++iRow) {
		for (UINT iCol = 0; iCol < (UINT)E_Layer::End; ++iCol) {
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
						vecLeft[l]->Collider2D()->OnCollisionExit(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionExit(vecLeft[l]->Collider2D());
						iter->second = false;
					}
					else {
						vecLeft[l]->Collider2D()->OnCollisionStay(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionStay(vecLeft[l]->Collider2D());
					}
				}
				else {
					// �Ѵ� ���� ������ �ƴϰ� ó�� �浹 �� 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						vecLeft[l]->Collider2D()->OnCollisionEnter(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionEnter(vecLeft[l]->Collider2D());
						iter->second = true;
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

void CCollisionManager::SetOnOffCollision(E_Layer _eLayerOne, E_Layer _eLayerTwo, bool _bIsOn)
{
	UINT iLeft = (UINT)_eLayerOne;
	UINT iRight = (UINT)_eLayerTwo;

	// Flag��(���) ������ �̿��ϸ� �ǹǷ� ���� �� ���������� ��ȯ
	UINT iRow = iLeft < iRight ? iLeft : iRight;
	UINT iCol = iLeft > iRight ? iLeft : iRight;

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
	// TODO : �浹 üũ
	return false;
}