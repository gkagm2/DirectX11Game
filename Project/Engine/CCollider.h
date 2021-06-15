#pragma once
#include "CComponent.h"
#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CGameObject;
class CCollider : public CComponent
{
private:
	static UINT g_iColliderID;

protected:
	CGameObject* m_pOwnerGameObj; // �� ��ü�� ������ �ִ� ������Ʈ
	int m_iCollisionCount;		  // �� ��ü�� �ٸ� �浹ü�� �浹�� Ƚ��

	Vector3 m_vOffsetPosition;		// ������Ʈ ��ġ�κ��� ������� ���̰�
	UINT m_iID;						// �浹ü ���� �ĺ���ȣ

public:
	CCollider() = delete;
	CCollider(E_ComponentType _eComponentType);
	virtual ~CCollider() override;
};