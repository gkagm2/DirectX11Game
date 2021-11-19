#pragma once
#include <Engine\CScript.h>
class CPostProcessingScript : public CScript
{
	SharedPtr<CMaterial> m_pPostEffectMtrl;

public:
	float m_fScreenScale; // default 2 
	float m_fAmplitude; // 진폭
	float m_fCompression; // 압축 길이
	float m_fSpeed; // 출렁이는 속도

	int m_iPostProcessingType;

public:
	virtual void Start() override;
	virtual void Update() override;


public:


public:
	CLONE(CPostProcessingScript);
	CPostProcessingScript();
	virtual ~CPostProcessingScript() override;
};