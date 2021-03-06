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

CCollisionManager::CCollisionManager() : 
	m_bCollisionShow{ true }
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

	if (E_SceneMode::Play == CSceneManager::GetInstance()->GetSceneMode()) {
		if (InputKeyPress(E_Key::F9))
			m_bCollisionShow = m_bCollisionShow == true ? false : true;
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
			/*if (vecLeft[l]->Rigidbody2D() && vecRight[r]->Rigidbody2D() &&
				!pLeftCol->IsTrigger() && !pRightCol->IsTrigger()) {
				bIsPushIntersection = true;
			}*/
			// 한쪽만이라도 Rigidbody가 존재하면서 양쪽 다 Trigger가 false면
			if (vecLeft[l]->Rigidbody2D() || vecRight[r]->Rigidbody2D()) {
				if (!pLeftCol->IsTrigger() && !pRightCol->IsTrigger()) {
					// 성능을 위해 
					if(vecLeft[l]->IsActive() && vecRight[r]->IsActive() &&
						pLeftCol->IsActive() && pRightCol->IsActive())
						bIsPushIntersection = true;
				}
			}
			bool bIsActivePushIntersection = false;

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
						if (!vecLeft[l]->IsActive() || !vecRight[r]->IsActive() ||
							!pLeftCol->IsActive() || !pRightCol->IsActive()) {
							// 충돌을 벗어난다.
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

						// 밀어내기 실행하면
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
					// 둘다 삭제 예정이 아니고 처음 충돌 시 
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
			return _IsCollision(_pLeft, _pRight, &_pPushIntersectionInfo);
		return _IsCollision(_pLeft, _pRight, nullptr);
	}

	return false;
}

bool CCollisionManager::IsCollisionSphere(CCollider3D* _pLeft, CCollider3D* _pRight)
{
	// TODO (Jang) : 구 충돌 Radius를 Collider3D 컴포넌트에 추가하기.
	const Vector3& vLeftPos = _pLeft->Transform()->GetPosition();
	const Vector3& vRightPos = _pRight->Transform()->GetPosition();

	float fLeftRadius = _pLeft->Transform()->GetScale().x;
	float fRightRadius = _pRight->Transform()->GetScale().x;

	float fDis = Vector3::Distance(vLeftPos, vRightPos);
	if (fDis < (fLeftRadius + fRightRadius) * (fLeftRadius + fRightRadius))
		return true;
	return false;
}

// 2D Triangle, Point
bool CCollisionManager::IsCollisionTriangle_Point(CCollider2D* _pLeft, const Vector3& _vPoint)
{
	// 삼각형 출동 ( 삼각메쉬는 만들지 않았음 )
	// 유향선분을 이용한 삼각형 내부의 점 충돌 검사
	static Vector3 arrLocal[4] = { // 시계방향
	Vector3(0.f, 0.5f, 0.f),
	Vector3(0.5f, -0.5f, 0.f),
	Vector3(-0.5f,0.5f, 0.f),
	};

	// 삼각형 사이클 벡터
	Vector3 v3TriVec0 = XMVector3TransformCoord(arrLocal[0], _pLeft->GetWorldMatrix());
	Vector3 v3TriVec1 = XMVector3TransformCoord(arrLocal[1], _pLeft->GetWorldMatrix());
	Vector3 v3TriVec2 = XMVector3TransformCoord(arrLocal[2], _pLeft->GetWorldMatrix());

	Vector3 vTriDirVec0 = v3TriVec1 - v3TriVec0;
	Vector3 vTriDirVec1 = v3TriVec2 - v3TriVec1;
	Vector3 vTriDirVec2 = v3TriVec0 - v3TriVec2;


	// 삼각형 정점에서 플레이어로 향한 벡터
	Vector3 vHitDirVec0 = _vPoint - v3TriVec0;
	Vector3 vHitDirVec1 = _vPoint - v3TriVec1;
	Vector3 vHitDirVec2 = _vPoint - v3TriVec2;

	// 각각 외적
	float fCross0 = v3TriVec0.z * vHitDirVec0.x - v3TriVec0.x * vHitDirVec0.z;
	float fCross1 = v3TriVec1.z * vHitDirVec1.x - v3TriVec1.x * vHitDirVec1.z;
	float fCross2 = v3TriVec2.z * vHitDirVec2.x - v3TriVec2.x * vHitDirVec2.z;

	bool bHit = false;
	if (fCross0 >= 0.f) {
		if (fCross1 >= 0.f && fCross2 >= 0.f)
			bHit = true;
	}
	else {
		if (fCross1 < 0.f && fCross2 < 0.f)
			bHit = true;
	}
	return bHit;
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
	Vector3 vLocalScale = _pRT->GetLocalScale();

	float fHalfWidth = fabsf(_pRT->GetWidth()) * 0.5f * vLocalScale.x;
	float fHalfHeight = fabsf(_pRT->GetHeight()) * 0.5f * vLocalScale.y;
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
bool CCollisionManager::_IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight, TRigidCollisionInfo** _tRigidColInfo)
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
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f,-0.5f, 0.f),
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
	}

	if (_tRigidColInfo) {
		// Rigidbody 컴포넌트가 존재한다면 
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
			Vector3 vProjDir = vecProj[i]; // 투영축 방향 벡터
			Vector3 vProjVec = vecProj[i]; // 투영축 방향의 벡터 크기
			vProjDir.Normalize();

			float rigidLen = 0.f; // rigidBody 있는 충돌체의 중심점부터 투영축으로부터의 길이
			for (int j = 0; j < iRigidSize; ++j)
				rigidLen += fabsf(vProjDir.Dot(vecRigid[j]));
			rigidLen *= 0.5f;

			float noRigidLen = 0.f; // rigidbody가 없는 충돌체의 중심점부터 투영축으로부터의 길이
			for (int j = 0; j < iNoRigidSize; ++j)
				noRigidLen += fabsf(vProjDir.Dot(vecNoRigid[j]));
			noRigidLen *= 0.5f;

			// 가운데 길이를 구함
			float fCenterLen = fabsf(vProjDir.Dot(vToCenter));

			float fGepLen = 0.f;
			if (rigidLen + noRigidLen > fCenterLen) { // 겹쳐지면
				fGepLen = rigidLen + noRigidLen - fCenterLen; // 겹쳐진 차이를 구함.
			}

			// 예각인지 확인하기
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