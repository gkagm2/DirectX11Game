#pragma once
#include <Engine\CScript.h>


struct TCursorInfo {
	SharedPtr<CTexture> pCursorTex;
	Vector3 vScale;
	Vector3 vOffset;
};

class CCursorScript : public CScript
{
	SharedPtr<CMaterial> m_pCursorMtrl;

public:
	virtual void Awake() override;
	virtual void LateUpdate() override;

	void SetCurCursor(SharedPtr<CTexture> _pTex, const Vector2 _vSize);

public:
	CLONE(CCursorScript);
	CCursorScript();
	virtual ~CCursorScript() override;
};