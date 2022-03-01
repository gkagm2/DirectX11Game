#pragma once
#include "CComponent.h"

class CLight3D : public CComponent
{
private:
	// Spot Light일 경우
	// Diffuse 사용
	// Range 사용
	// Angle 사용

	// Point Light일 경우
	// Diffuse 사용
	TLightInfo m_tInfo;
	SharedPtr<CMesh> m_pMesh;		// 광원의 범위를 나타낼 볼륨 메쉬
	SharedPtr<CMaterial> m_pMtrl;   // 광원 MRT 에 광원을 랜더링 하는 재질

	CGameObject* m_pLightCamObj; // Light 내부 카메라

	Matrix m_matLightWorld;

public:
	virtual void Render() override;
	virtual bool LoadFromScene(FILE* _pFile) override;
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual void FinalUpdate() override;
	//virtual void UpdateData() override;

public:
	void SetLightType(E_LightType _eType);
	E_LightType GetLightType() { return m_tInfo.eLightType; }
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
	CCamera* GetLight3DCam() { return m_pLightCamObj->Camera(); }

	SharedPtr<CMesh> GetLightMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetLightMaterial() { return m_pMtrl; }

private:
	void _CreateLight3DCamera();

public:
	CLONE(CLight3D);
	CLight3D();
	CLight3D(const CLight3D& _origin);
	virtual ~CLight3D() override;
};