#pragma once
#include "CComponent.h"
#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CGameObject;
class CCollider : public CComponent
{
protected:
	CGameObject* m_pOwnerGameObj; // �� ��ü�� ������ �ִ� ������Ʈ
	int m_iCollisionCount;		  // �� ��ü�� �ٸ� �浹ü�� �浹�� Ƚ��
	
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

public:
	CCollider() = delete;
	CCollider(E_ComponentType _eComponentType);
	virtual ~CCollider() override;
};