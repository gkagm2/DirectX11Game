#pragma once
#include <Engine\CScript.h>
class CToolGridScript : public CScript
{
private:
	CGameObject* m_pToolCamObj;
	Vector3 m_vToolCamPos;
	float m_fThickness;
	float m_fDistance;
	UINT m_iMaxStep;			// �׸��� ���� �ִ� �ܰ�
	float m_fAlpha;				// �׸��� ����
	Vector4 m_vColor;			// �׸��� ����

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