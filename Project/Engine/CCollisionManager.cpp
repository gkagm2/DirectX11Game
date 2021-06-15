#include "pch.h"
#include "CCollisionManager.h"

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