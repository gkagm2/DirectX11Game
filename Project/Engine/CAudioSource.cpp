#include "pch.h"
#include "CAudioSource.h"
#include "CSound.h"
CAudioSource::CAudioSource() :
	CComponent(E_ComponentType::AudioSource),
	m_bMute(false),
	m_bOverlap(false),
	m_fVolume(0.5f),
	m_bLoop(true),
	m_iLoopCnt(1.f)

{
}

CAudioSource::~CAudioSource()
{
}

void CAudioSource::Play()
{
	if (m_bLoop)
		GetSound()->Play(0, m_fVolume, m_bOverlap);
	else {
		if (0 == m_iLoopCnt)
			assert(nullptr);
		GetSound()->Play(m_iLoopCnt, m_fVolume, m_bOverlap);
	}
}

void CAudioSource::Stop()
{
	GetSound()->Stop();
}

void CAudioSource::SetVolume(float _f, int _iChannelIdx)
{
	_f = CMyMath::Clamp(_f, 0.f, 1.f);
	GetSound()->SetVolume(_f, _iChannelIdx);
}

bool CAudioSource::SaveToScene(FILE* _pFile)
{
	FWrite(m_pSound, _pFile);
	FWrite(m_bLoop, _pFile);
	FWrite(m_fVolume, _pFile);
	return true;
}

bool CAudioSource::LoadFromScene(FILE* _pFile)
{
	FRead(m_pSound, _pFile);
	FRead(m_bLoop, _pFile);
	FRead(m_fVolume, _pFile);
	return true;
}