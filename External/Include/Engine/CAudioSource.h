#pragma once
#include "CComponent.h"
class CSound;
class CAudioSource : public CComponent
{
	SharedPtr<CSound> m_pSound;

	bool m_bMute;
	bool m_bOverlap;
	float m_fVolume;

	bool m_bLoop;
	int m_iLoopCnt; // loop 가 true면 loop count를 지정해줌.

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	SharedPtr<CSound> GetSound() { return m_pSound; }
	void SetSound(SharedPtr<CSound> _pSound) { m_pSound = _pSound; }

	void Play();
	void Stop();
	void SetVolume(float _f, int _iChannelIdx); // 0 ~ 1

public:
	CLONE(CAudioSource);
	CAudioSource();
	virtual ~CAudioSource() override;
};