#pragma once
#include <Engine\CScript.h>


struct TCursorInfo {
	SharedPtr<CTexture> pCursorTex;
	Vector3 vScale;
	Vector3 vOffset;
};

class CCursorScript : public CScript
{
public:
	enum class E_Pivot {
		LeftTop,
		Middle,
	};
private:
	SharedPtr<CMaterial> m_pCursorMtrl;
	Vector2 m_vHotSpotOffset;
	E_Pivot m_ePivot;
	

public:
	virtual void Awake() override;
	virtual void LateUpdate() override;

	// _vHotSpotOffset : 좌상단을 원점으로 한 오프셋
	void SetCurCursor(SharedPtr<CTexture> _pTex, const Vector2& _vHotSpotOffset);
	void SetPivotPos(E_Pivot _ePivot) { m_ePivot = _ePivot; }

public:
	CLONE(CCursorScript);
	CCursorScript();
	virtual ~CCursorScript() override;
};