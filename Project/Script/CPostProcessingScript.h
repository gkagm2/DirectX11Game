#pragma once
#include <Engine\CScript.h>
class CPostProcessingScript : public CScript
{
	SharedPtr<CMaterial> m_pPostEffectMtrl;

public:
	float m_fScreenScale; // default 2 
	float m_fAmplitude; // ����
	float m_fCompression; // ���� ����
	float m_fSpeed; // �ⷷ�̴� �ӵ�

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