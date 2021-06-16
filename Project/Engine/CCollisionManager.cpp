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
			// arrLayerFlag[iRow] & ( 1 << iCol ) => iRow라인에 iCol자리 비트가 1인지 체크  
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

			// 두 충돌체의 고유 아이디를 조합해서 키값을 만들어 냄
			COLLIDER_ID id = {};
			id.iLeft = vecLeft[l]->Collider2D()->GetID();
			id.iRight = vecRight[r]->Collider2D()->GetID();
			unordered_map<LONGLONG, bIsCollision>::iterator iter = m_unmapCollisionInfo.find(id.llID);
			if (iter == m_unmapCollisionInfo.end()) {
				m_unmapCollisionInfo.insert(std::make_pair(id.llID, false));
				iter = m_unmapCollisionInfo.find(id.llID);
			}
#pragma region Active가 없을 경우
			// 충돌 검사
			if (IsCollision(vecLeft[l]->Collider2D(), vecRight[r]->Collider2D())) { // 충돌했을 경우
				if (iter->second) { // 이전에도 충돌했을 시
					// 둘 중 하나라도 삭제 예정 상태일 경우
					if (vecLeft[l]->IsDead() || vecRight[r]->IsDead()) {
						// 충돌을 벗어난다.
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
					// 둘다 삭제 예정이 아니고 처음 충돌 시 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						vecLeft[l]->Collider2D()->OnCollisionEnter(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionEnter(vecLeft[l]->Collider2D());
						iter->second = true;
					}
				}
			}
			else { // 충돌하지 않았을 경우
				if (iter->second) { // 이전에 충돌했을 경우
					vecLeft[l]->Collider2D()->OnCollisionExit(vecRight[r]->Collider2D());
					vecRight[r]->Collider2D()->OnCollisionExit(vecLeft[l]->Collider2D());
					iter->second = false;
				}
			}
#pragma endregion

#pragma region Active가 있을 경우
			/*
			// 충돌 검사
			if (IsCollision(vecLeft[l]->Collider2D(), vecRight[r]->Collider2D())) { // 충돌했을 경우
				if (iter->second) { // 이전에도 충돌했을 시
					// 둘 중 하나라도 삭제 예정 상태일 경우
					if (vecLeft[l]->IsDead() || vecRight[r]->IsDead()) {
						// 충돌을 벗어난다.
						vecLeft[l]->Collider2D()->OnCollisionExit(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionExit(vecLeft[l]->Collider2D());
						iter->second = false;
					}
					else {
						// 하나라도 active가 false가 된게 있으면
						if (!vecLeft[l]->Collider2D()->IsActive() || !vecRight[r]->Collider2D()->IsActive()) {
							// 충돌을 벗어난다.
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
					// 둘다 삭제 예정이 아니고 처음 충돌 시 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						if (vecLeft[l]->Collider2D()->IsActive() && vecRight[r]->Collider2D()->IsActive()) {
							vecLeft[l]->Collider2D()->OnCollisionEnter(vecRight[r]->Collider2D());
							vecRight[r]->Collider2D()->OnCollisionEnter(vecLeft[l]->Collider2D());
							iter->second = true;
						}
					}
				}
			}
			else { // 충돌하지 않았을 경우
				if (iter->second) { // 이전에 충돌했을 경우
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

	// Flag의(행렬) 반절만 이용하면 되므로 행을 더 작은값으로 변환
	UINT iRow = iLeft < iRight ? iLeft : iRight;
	UINT iCol = iLeft > iRight ? iLeft : iRight;

	if (_bIsOn)
		m_bitsetCollisionGroup[iRow].set(iCol); // iRow칸의 iCol의 비트를 1로 세팅
	else
		m_bitsetCollisionGroup[iRow].reset(iCol); // iRow칸의 iCol의 비트를 0으로 세팅

#pragma region Bit flag version
			/*
if (_bIsOn)
	arrLayerFlag[iRow] = arrLayerFlag[iRow] | (1 << iCol); // iRow칸의 iCol비트를 1로 세팅
else
	arrLayerFlag[iRow] = arrLayerFlag[iRow] & ~(1 << iCol); // iRow칸의 iCol비트를 0으로 세팅
*/
#pragma endregion

}

bool CCollisionManager::IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// TODO
	return false;
}