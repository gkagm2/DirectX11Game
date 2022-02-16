#pragma once
#include "CResource.h"

#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_codec.h"

#ifdef ENVIRONMENT64
#ifdef _DEBUG
#pragma comment(lib, "FMOD/xx64/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/xx64/fmod64_vc.lib")
#endif
#endif

#ifdef ENVIRONMENT32
#ifdef _DEBUG
#pragma comment(lib, "FMOD/xx64/fmodL_vc.lib")
#else
#pragma comment(lib, "FMOD/xx64/fmod_vc.lib")
#endif
#endif

class CSound : public CResource
{
public:
	static FMOD::System* g_pFMOD;
private:
	FMOD::Sound* m_pSound;
	list<FMOD::Channel*>	m_listChannel;

public:
	// 0 (무한반복) 0 ~ 1(Volume)
	int Play(int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
	void Stop();

	// 0 ~ 1
	void SetVolume(float _f, int _iChannelIdx);

private:
	void RemoveChannel(FMOD::Channel* _pTargetChannel);
	friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2);

public:
	virtual bool Save(const tstring& _strRelativePath) override { return true; }
	virtual int Load(const tstring& _strFilePath) override;

public:
	CLONE_DISABLE(CSound);

public:
	CSound();
	virtual ~CSound() override;
};
