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

	float m_bIsShaking;
	float m_fShakingTime;
	float m_fMaxShakingTime;
	bool m_bIsRandomShake;
	float m_fShakeRange;
	Vector3 m_vShakeDir;

public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void Zoom();
	//Random true 시 ForceDir 방향을 입력할 필요 없음.
	void Shaking(bool _bIsRandom, float _fRange, float _fTime, const Vector3& _vShakeDir = Vector3::Zero);

public:
	CLONE(CCameraFollowerScript);
	CCameraFollowerScript();

	CCameraFollowerScript(const CCameraFollowerScript& _origin);
	virtual ~CCameraFollowerScript() override;
};