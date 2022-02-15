#pragma once
#include "CComponent.h"

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
	virtual bool LoadFromScene(FILE* _pFile) override;
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual void FinalUpdate() override;

public:
	void SetLightType(E_LightType _eType) { m_tInfo.eLightType = _eType; }
	void SetAngle(float _fDegree) { m_tInfo.fAngle_Radian = _fDegree * CMyMath::Deg2Rad(); }
	float GetAngle() { return m_tInfo.fAngle_Radian * CMyMath::Rad2Deg(); }
	void SetDiffColor(const Vector3& _vColor) { m_tInfo.tColor.vDiffuse = _vColor; }
	const Vector4& GetDiffColor() { return m_tInfo.tColor.vDiffuse; }
	void SetSpecular(const Vector4& _vSpecular) { m_tInfo.tColor.vSpecular = _vSpecular; }
	const Vector4& GetSpecular() { return m_tInfo.tColor.vSpecular; }

	void SetEmbient(const Vector4& _vEmbient) { m_tInfo.tColor.vEmbient = _vEmbient; }
	const Vector4& GetEmbient() { return m_tInfo.tColor.vEmbient; }
	void SetRange(float _fRange);
	float GetRange() { return m_tInfo.fRange; }
	const TLightInfo& GetLightInfo() { return m_tInfo; }
	TLightInfo& GetLightInfoRef() { return m_tInfo; }

public:
	CLONE(CLight2D);
	CLight2D();
	virtual ~CLight2D() override;
};