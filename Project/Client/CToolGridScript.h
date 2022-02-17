#pragma once
#include <Engine\CScript.h>
class CToolGridScript : public CScript
{
private:
	CGameObject* m_pToolCamObj;
	Vector3 m_vToolCamPos;
	float m_fThickness;
	float m_fDistance;
	UINT m_iMaxStep;			// 그리드 간격 최대 단계
	float m_fAlpha;				// 그리드 투명도
	Vector4 m_vColor;			// 그리드 색상

public:
	virtual void Update() override;

public:
	void SetToolCamera(CGameObject* _pToolCam) { m_pToolCamObj = _pToolCam; }
	void SetGridColor(const Vector3& _vColor) { m_vColor = _vColor; }

private:
	void CalculateGrid();

public:
	CLONE(CToolGridScript);
	CToolGridScript();
	virtual ~CToolGridScript() override;
};