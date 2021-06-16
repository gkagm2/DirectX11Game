#pragma once

class CCollider;
class CCollider2D;
class CCollider2DRect;
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
	// UINT arrLayerFlag[(UINT)E_Layer::End];  
#pragma endregion
	bitset<(UINT)E_Layer::End> m_bitsetCollisionGroup[(UINT)E_Layer::End];
	unordered_map<LONGLONG, bIsCollision> m_unmapCollisionInfo; // first : left�� right, second : ���� �������� �浹 ����

public:
	void Update();

public:
	void CollisionByLayer(UINT _iLayerOneIdx, UINT _iLayerTwoIdx); // ���̾ �浹

	void SetOnOffCollision(E_Layer _eLayerOne, E_Layer _eLayerTwo, bool _bIsOn); // ������ �� ���̾�鳢�� �浹�� ���� ���� Flag ����
	void ClearAllCollisionLayer() {
		// memset(arrLayerFlag, 0, size(UINT) * (UINT)E_Layer::End);
		for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
			m_bitsetCollisionGroup[i].reset();
	}

public:
	bool IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight);

private:
	bool _IsCollision(CCollider2DRect* _pLeft, CCollider2DRect* _pRight);
};