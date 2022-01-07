#pragma once
#include <Engine\CScript.h>
class CCameraFollowerScript : public CScript
{
private:
	CGameObject* m_pTargetObj;

	float m_fLerpSpeed;
	bool m_bEnableFollow;

	bool m_bEnableZoom;
	float m_fMouseScrollSpeed;


public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void Zoom();

public:
	CLONE(CCameraFollowerScript);
	CCameraFollowerScript();

	CCameraFollowerScript(const CCameraFollowerScript& _origin);
	virtual ~CCameraFollowerScript() override;
};