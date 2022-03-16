#include "pch.h"
#include "CBoundingBox.h"
#include "CRenderManager.h"
#include "CTransform.h"
CBoundingBox::CBoundingBox() :
	CComponent(E_ComponentType::BoundingBox)
{
}

CBoundingBox::~CBoundingBox()
{
}

void CBoundingBox::Render()
{
	void RenderDebugSphere(const Vector3 & _vWorldPos, const Vector3 & _vLocalRot, const Vector3 & _vColor, float _fRadius, float _fLifeTime);
	const Vector3& vWorldPos = Transform()->GetPosition();
	const Vector3& vWorldRot = Transform()->GetRotation();
	Vector3 vColor = Vector3(0.f, 0.5f, 0.5f);
	Vector3 vScale = Transform()->GetScale();
	float fRadius = max(vScale.x, (max(vScale.y, vScale.z))) * 0.5f;

	// TODO (Jang) : Render에서 계속 업데이트를 하면 렌더링되게 할까?
	//CRenderManager::GetInstance()->RenderDebugSphere(vWorldPos, vWorldRot, vColor, fRadius, 0.f);
}