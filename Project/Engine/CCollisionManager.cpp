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
					// 둘다 삭제 예정이 아니고 처음 충돌 시 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						vecLeft[l]->Collider2D()->OnCollisionEnter2D(vecRight[r]->Collider2D());
						vecRight[r]->Collider2D()->OnCollisionEnter2D(vecLeft[l]->Collider2D());
						iter->second = true;
					}
				}
			}
			else { // 충돌하지 않았을 경우
				if (iter->second) { // 이전에 충돌했을 경우
					vecLeft[l]->Collider2D()->OnCollisionExit2D(vecRight[r]->Collider2D());
					vecRight[r]->Collider2D()->OnCollisionExit2D(vecLeft[l]->Collider2D());
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
	// 로컬 좌표
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
// 주어진 행렬로 3D 벡터를 변환하여 결과를 다시 w = 1로 투영합니다.
//입력 벡터의 w 구성 요소를 무시하고 대신 1.0 값을 사용합니다.반환 된 벡터의 w 구성 요소는 항상 1.0입니다.

// 쉐이더 코드로 치면 mul(float4(arrLoca[0], 0.f), _pLeft->GetWorldMat()); 임.
// 
// XMVector3TransformNormal(arrLocal[2], _pLeft->GetWorldMatrix());을 쓰게 되면 4번째 요소를 0으로 설정해준다.  
#pragma endregion
	// 세 개의 위치값을 충돌체 위치로 보내기
	Vector3 v0 = XMVector3TransformCoord(arrLocal[0], _pLeft->GetWorldMatrix()); // 이동 좌표에 영향을 주도록 4번째 요소값을 1로 확장
	v0.z = 0.f;
	Vector3 v1 = XMVector3TransformCoord(arrLocal[1], _pLeft->GetWorldMatrix());
	v1.z = 0.f;
	Vector3 v3 = XMVector3TransformCoord(arrLocal[3], _pLeft->GetWorldMatrix());
	v3.z = 0.f;
	
	// 투영축 계산
	Vector3 vToLeft1 = v1 - v0;
	Vector3 vToDown1 = v3 - v0;

	// 다른 사각형의 투영 축 구하기
	v0 = XMVector3TransformCoord(arrLocal[0], _pRight->GetWorldMatrix());
	v0.z = 0.f;
	v1 = XMVector3TransformCoord(arrLocal[1], _pRight->GetWorldMatrix());
	v1.z = 0.f;
	v3 = XMVector3TransformCoord(arrLocal[3], _pRight->GetWorldMatrix());
	v3.z = 0.f;

	// 투영축 계산
	Vector3 vToLeft2 = v1 - v0;
	Vector3 vToDown2 = v3 - v0;

	// 각 투영축을 모두 검사하기 위해 배열에 넣어줌
	Vector3 arrProjAxis[4] = { vToLeft1, vToDown1, vToLeft2, vToDown2 };

	Vector3 vCenterPosLeftCollider = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), _pLeft->GetWorldMatrix());
	Vector3 vCenterPosRightCollider = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), _pRight->GetWorldMatrix());

	Vector3 vCenter = vCenterPosRightCollider - vCenterPosLeftCollider;

	// 모든 축으로 투영되어 분리축이 나오는지 검사
	for (int i = 0; i < 4; ++i) {
		Vector3 vAxisDir = arrProjAxis[i];
		vAxisDir.Normalize(); // 단위 벡터로 만듬

		float fProjTwoBoxesDis = 0.f;
		for (int j = 0; j < 4; ++j)
			fProjTwoBoxesDis += abs(vAxisDir.Dot(arrProjAxis[j]));

		float fProjCenterDis = abs(vAxisDir.Dot(vCenter)); // 센터길이를 투영시켜서 길이를 구한다.

		if (fProjCenterDis > fProjTwoBoxesDis * 0.5f)
			return false;
	}

	return true;
}