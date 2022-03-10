#pragma once
#include "CObject.h"

enum class E_FaceType {
	Near,
	Far,
	Up,
	Down,
	Left,
	Right,
	End
};

class CCamera;
class CMesh;
class CMaterial;
class CFrustum : public CObject
{
private:
	static const Vector3 g_arrProjPos[8];
private:
	CCamera* m_pOwner;
	Vector4 m_arrFace[(UINT)E_FaceType::End];
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMaterial;
	
public:
	virtual void UpdateData() override;
	virtual void Render();

	void FinalUpdate();
	bool CheckPoint(const Vector3& _vPos);
	bool CheckSphere(const Vector3& _vCenter, float _fRadius);

public:
	CLONE(CFrustum);
	CFrustum();
	virtual ~CFrustum() override;
	friend class CCamera;
};