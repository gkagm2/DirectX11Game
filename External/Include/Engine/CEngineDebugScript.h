#pragma once
#include "CScript.h"
class CEngineDebugScript : public CScript
{
public:
	float m_fLifeTime;
	float m_fAccTime;
	float m_fAlpha;
	bool m_bUse;
	Vector4 m_vColor;

public:
	virtual void Update() override;

	bool IsUsed() { return m_bUse; }
	void SetUsed(bool _bUse) { m_bUse = _bUse; }
	const Vector4& GetColor() { return m_vColor; }
	float GetAlpha() { return m_fAlpha; }

	void SetColor(const Vector3& _vColor) { m_vColor.x = _vColor.x; m_vColor.y = _vColor.y, m_vColor.z = _vColor.z; }
	void SetColor(const Vector4& _vColor) { m_vColor = _vColor; }
	void SetAlpha(float _fAlpha) { m_vColor.w = _fAlpha; }
	
public:
	CLONE(CEngineDebugScript);
	CEngineDebugScript();
	virtual ~CEngineDebugScript() override;
};