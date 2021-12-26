#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CLight2D : public CComponent
{
private:
	// Spot Light일 경우
	// Diffuse 사용
	// Range 사용
	// Angle 사용

	// Point Light일 경우
	// Diffuse 사용


	TLightInfo m_tInfo;
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

public:
	virtual void FinalUpdate() override;

public:
	void SetLightType(E_LightType _eType) { m_tInfo.eLightType = _eType; }
	void SetAngle(float _fDegree) { m_tInfo.fAngle_Radian = _fDegree * CMyMath::Deg2Rad(); }
	float GetAngle() { return m_tInfo.fAngle_Radian * CMyMath::Rad2Deg(); }
	void SetDiffColor(const Vector3& _vColor) { m_tInfo.tColor.vDiffuse = _vColor; }
	void SetRange(float _fRange);
	const TLightInfo& GetLightInfo() { return m_tInfo	; }
	TLightInfo& GetLightInfoRef() { return m_tInfo; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CLight2D);

	CLight2D();
	virtual ~CLight2D() override;
};