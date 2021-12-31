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

		size_t r = 0;
		if (_iLayerOneIdx == _iLayerTwoIdx)
			r = l;
		for (; r < vecRight.size(); ++r) {
			if (!vecRight[r]->Collider2D())
				continue;
			// 자기 자신과의 추돌일 경우
			if (vecLeft[l] == vecRight[r])
				continue;

			CCollider2D* pLeftCol = vecLeft[l]->Collider2D();
			CCollider2D* pRightCol = vecRight[r]->Collider2D();


			// 두 충돌체의 고유 아이디를 조합해서 키값을 만들어 냄
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
			// Rigidbody가 존재하며 두 충돌체 모두 통과 Trigger가 false면 밀어내기 기능 실행
			if (vecLeft[l]->Rigidbody2D() && vecRight[r]->Rigidbody2D() &&
				!pLeftCol->IsTrigger() && !pRightCol->IsTrigger()) {
				bIsPushIntersection = true;
			}

			// 충돌 검사
			if (IsCollision(pLeftCol, pRightCol, bIsPushIntersection, &info)) { // 충돌했을 경우
				if (iter->second) { // 이전에도 충돌했을 시
					// 둘 중 하나라도 삭제 예정 상태일 경우
					if (vecLeft[l]->IsDead() || vecRight[r]->IsDead()) {
						// 충돌을 벗어난다.
						pLeftCol->OnCollisionExit2D(pRightCol);
						pRightCol->OnCollisionExit2D(pLeftCol);
						iter->second = false;
					}
					else {
						// 하나라도 active가 된게 있으면
						if (!pLeftCol->IsActive() || !pRightCol->IsActive()) {
							// 충돌을 벗어난다.
							pLeftCol->OnCollisionExit2D(pRightCol);
							pRightCol->OnCollisionExit2D(pLeftCol);
							iter->second = false;
						}
						else {
							// 밀어내기 실행 시 
							if (bIsPushIntersection) {
								pLeftCol->OnCollisionStay2D(pRightCol, &info);
								pRightCol->OnCollisionStay2D(pLeftCol, &info);
							}
							else {
								pLeftCol->OnCollisionStay2D(pRightCol);
								pRightCol->OnCollisionStay2D(pLeftCol);
							}
						}
					}
				}
				else {
					// 둘다 삭제 예정이 아니고 처음 충돌 시 
					if (!vecLeft[l]->IsDead() && !vecRight[r]->IsDead()) {
						if (pLeftCol->IsActive() && pRightCol->IsActive()) {
							pLeftCol->OnCollisionEnter2D(pRightCol);
							pRightCol->OnCollisionEnter2D(pLeftCol);
							iter->second = true;
						}
					}
				}
			}
			else { // 충돌하지 않았을 경우
				if (iter->second) { // 이전에 충돌했을 경우
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
	// Flag의(행렬) 반절만 이용하면 되므로 행을 더 작은값으로 변환
	UINT iRow = _iLayerOne < _iLayerTwo ? _iLayerOne : _iLayerTwo;
	UINT iCol = _iLayerOne > _iLayerTwo ? _iLayerOne : _iLayerTwo;

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

bool CCollisionManager::CheckCollisionLayer(UINT _iLayerOne, UINT _iLayerTwo)
{
	// Flag의(행렬) 반절만 이용하면 되므로 행을 더 작은값으로 변환
	UINT iRow = _iLayerOne < _iLayerTwo ? _iLayerOne : _iLayerTwo;
	UINT iCol = _iLayerOne > _iLayerTwo ? _iLayerOne : _iLayerTwo;

	if (m_bitsetCollisionGroup[iRow].test(iCol))
		return true;
	return false;
}

bool CCollisionManager::IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight, bool m_bPushIntersection, TRigidCollisionInfo* _pPushIntersectionInfo)
{
	{
		// FIXED : Collider2D에서 Rect로만 되어있는데 나중에 추가되면 타입에 따라 분기할 수 있도록 하기

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

	// 각 vertex의 world좌표
	Vector3 vLT = XMVector3TransformCoord(arrLocal[0], _pCol->GetWorldMatrix());
	Vector3 vRT = XMVector3TransformCoord(arrLocal[1], _pCol->GetWorldMatrix());
	Vector3 vRB = XMVector3TransformCoord(arrLocal[2], _pCol->GetWorldMatrix());
	Vector3 vLB = XMVector3TransformCoord(arrLocal[3], _pCol->GetWorldMatrix());
	Vector3 vCenter = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), _pCol->GetWorldMatrix());
	vLT.z = vRT.z = vRB.z = vLB.z = vCenter.z = 0.f;

	// 투영 축
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
	// FIXED (Jang) : 최적화 필요.
	CCamera* pUICam = CRenderManager::GetInstance()->GetUICamera();
	if (!pUICam) {
		assert(pUICam && _T("UI Camera 없음"));
		return false;
	}

	// 중심점으로부터 width, height을 구해서 위치를 구함
	if (nullptr == _pRT) {
		assert(_pRT && _T("RectTransform 없음"));
		return false;
	}
	Vector3 vCenterPos = _pRT->GetPosition();
	Vector2 vScreenCenter = pUICam->GetWorldToScreen2DPosition(vCenterPos);

	float fHalfWidth = fabsf(_pRT->GetWidth()) * 0.5f;
	float fHalfHeight = fabsf(_pRT->GetHeight()) * 0.5f;
	float vRotZ = _pRT->GetRotationDegree().z;

	// Screen좌표로 모서리 위치를 가져옴.
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
	Vector3 vToRight1 = v1 - v0;
	Vector3 vToDown1 = v3 - v0;

	// 다른 사각형의 투영 축 구하기
	v0 = XMVector3TransformCoord(arrLocal[0], _pRight->GetWorldMatrix());
	v0.z = 0.f;
	v1 = XMVector3TransformCoord(arrLocal[1], _pRight->GetWorldMatrix());
	v1.z = 0.f;
	v3 = XMVector3TransformCoord(arrLocal[3], _pRight->GetWorldMatrix());
	v3.z = 0.f;

	// 투영축 계산
	Vector3 vToRight2 = v1 - v0;
	Vector3 vToDown2 = v3 - v0;

	// 각 투영축을 모두 검사하기 위해 배열에 넣어줌
	Vector3 arrProjAxis[4] = { vToRight1 , vToDown1 , vToRight2 , vToDown2 };

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

		if (fProjCenterDis > fProjTwoBoxesDis * 0.5f) {
			return false;
		}
		else {
			if (_tRigidColInfo) {
				float fLMass = _pLeft->Rigidbody2D()->GetMass();
				float fRMass = _pRight->Rigidbody2D()->GetMass();
				Vector3 vForceDir;
				if (fLMass < fRMass) {
					vForceDir = vCenterPosLeftCollider - vCenterPosRightCollider;
					_tRigidColInfo->pGameObject = _pLeft->GetGameObject();
				}
				else {
					vForceDir = vCenterPosRightCollider - vCenterPosLeftCollider;
					_tRigidColInfo->pGameObject = _pRight->GetGameObject();
				}
				vForceDir.Normalize();

				_tRigidColInfo->vDir = vForceDir;
				_tRigidColInfo->fDistance += fabsf((fProjTwoBoxesDis * 0.5f) - fProjCenterDis);
			}

			// 가깝다면
			// rigidbody에서 weight값을 이용하여 fHalf - fCenterDis 의 길이만큼
			// weight이 작은 오브젝트의 center위치 - weight이 큰 오브젝트의 center위치를 구해서 normalize해주면 weight이 작은 오브젝트의 방향을 구할 수 있다.
			// 구한 값으로 fHalf - fCenterDis길이만큼 오브젝트를 이동시킨다.
			// 그렇다면 fHalf- fCenterDis와 작은 weight의 방향값을 체크할 때 리턴시켜야한다. 
		}
	}
	if (_tRigidColInfo)
		_tRigidColInfo->fDistance = _tRigidColInfo->fDistance / 4.f;

	return true;
}