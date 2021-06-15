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
	CGameObject* m_pOwnerGameObj; // 이 객체를 가지고 있는 오브젝트
	int m_iCollisionCount;		  // 이 객체가 다른 충돌체와 충돌한 횟수

	Vector3 m_vOffsetPosition;		// 오브젝트 위치로부터 상대적인 차이값
	UINT m_iID;						// 충돌체 고유 식별번호

public:
	CCollider() = delete;
	CCollider(E_ComponentType _eComponentType);
	virtual ~CCollider() override;
};