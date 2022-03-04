#pragma once

class CCollider;
class CCollider2D;
class CRectTransform;
union COLLIDER_ID
{
	struct {
		UINT iLeft;
		UINT iRight;
	};

	LONGLONG llID;
};

// �浹 �� ���� ��ų �����͸� ������ ����ü
struct TRigidCollisionInfo {
	Vector3 vDir; // �з����� ����
	float fDistance; // �з����� �Ÿ�
	CGameObject* pGameObject; // �з����� ������Ʈ
	TRigidCollisionInfo() : vDir{}, fDistance{}, pGameObject{ nullptr }{}
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
	unordered_map<LONGLONG, bIsCollision> m_unmapCollisionInfo; // first : left�� right, second : ���� �������� �浹 ����

	bool m_bCollisionShow;

public:
	void Update();

public:
	void SetCollsionRendering(bool _bIsRender) { m_bCollisionShow = _bIsRender; }
	bool IsRenderCollisionArea() { return m_bCollisionShow; }
	void CollisionByLayer(UINT _iLayerOneIdx, UINT _iLayerTwoIdx); // ���̾ �浹

	void SetOnOffCollision(UINT _iLayerOne, UINT _iLayerTwo, bool _bIsOn); // ������ �� ���̾�鳢�� �浹�� ���� ���� Flag ����
	void ClearAllCollisionLayer() {
		// memset(arrLayerFlag, 0, size(UINT) * MAX_SIZE_LAYER);
		for (UINT i = 0; i < MAX_SIZE_LAYER; ++i)
			m_bitsetCollisionGroup[i].reset();
	}
	// �ش� ���̾� �浹üũ�� �Ǿ��ִ��� üũ
	bool CheckCollisionLayer(UINT _iLayerOne, UINT _iLayerTwo);

public:
	/// <param name="m_bPushIntersection">�о�� ����</param>
	bool IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight, bool m_bPushIntersection, TRigidCollisionInfo* _pPushIntersectionInfo = nullptr);

	bool IsCollisionSphere(CCollider3D* _pLeft, CCollider3D* _pRight);
	bool IsCollisionTriangle_Point(CCollider2D* _pLeft, const Vector3& _vPoint);

	// BoxCollider, WorldPoint 2d
	bool IsCollision(CCollider2D* _pCol, const Vector3& _vPoint); // 2d collsion
	// RectTransform�� �ִ� ������ ScreenPos�� MousePosition�� ���� �浹 ó��
	bool IsCollision(CRectTransform* _pRT, const Vector2& _vMousePosition);

	bool IsCollisionShow() { return m_bCollisionShow; }

private:
	// OBB
	bool _IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight, TRigidCollisionInfo** _tRigidColInfo);

	friend class CRigidbody;
	friend class CRigidbody2D;
};