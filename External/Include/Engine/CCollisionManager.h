#pragma once

class CCollider;
class CCollider2D;
union COLLIDER_ID
{
	struct {
		UINT iLeft;
		UINT iRight;
	};

	LONGLONG llID;
};

class CCollisionManager : public CSingleton<CCollisionManager>
{
	SINGLETON(CCollisionManager)

private:
	typedef bool bIsCollision;
#pragma region Bit flag version
	// UINT arrLayerFlag[MAX_SIZE_LAYER];  
#pragma endregion
	bitset<MAX_SIZE_LAYER> m_bitsetCollisionGroup[MAX_SIZE_LAYER];
	unordered_map<LONGLONG, bIsCollision> m_unmapCollisionInfo; // first : left와 right, second : 이전 프레임의 충돌 정보

public:
	void Update();

public:
	void CollisionByLayer(UINT _iLayerOneIdx, UINT _iLayerTwoIdx); // 레이어간 충돌

	void SetOnOffCollision(UINT _iLayerOne, UINT _iLayerTwo, bool _bIsOn); // 선택한 두 레이어들끼리 충돌을 할지 말지 Flag 세팅
	void ClearAllCollisionLayer() {
		// memset(arrLayerFlag, 0, size(UINT) * MAX_SIZE_LAYER);
		for (UINT i = 0; i < MAX_SIZE_LAYER; ++i)
			m_bitsetCollisionGroup[i].reset();
	}
	// 해당 레이어까리 충돌체크가 되어있는지 체크
	bool CheckCollisionLayer(UINT _iLayerOne, UINT _iLayerTwo);

public:
	bool IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight);

private:
	bool _IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight);
};