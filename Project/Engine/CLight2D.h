#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CLight2D : public CComponent
{
private:
	TLightInfo m_tInfo;
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

public:
	virtual void FinalUpdate() override;

public:
	void SetLightType(E_LightType _eType) { m_tInfo.eLightType = _eType; }
	void SetAngle(float _fDegree) { m_tInfo.fAngle = _fDegree * CMyMath::Deg2Rad(); }
	void SetDiffColor(const Vector3& _vColor) { m_tInfo.tColor.vDiffuse = _vColor; }
	void SetRange(float _fRange);
	const TLightInfo& GetLightInfo() { return m_tInfo; }
	TLightInfo& GetLightInfoRef() { return m_tInfo; }
	void SetLightPos(const Vector3& _vLightPos) { m_tInfo.vLightPos = Vector4(_vLightPos, 0.f); }
	void SetLightDir(const Vector3& _vLightDir) { m_tInfo.vLightDir = Vector4(_vLightDir, 0.f); }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CLight2D);

	CLight2D();
	virtual ~CLight2D() override;
};