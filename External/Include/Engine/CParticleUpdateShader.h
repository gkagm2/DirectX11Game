#pragma once
#include "CComputeShader.h"
class CParticleUpdateShader : public CComputeShader
{
private:
	CStructuredBuffer* m_pParticleBuffer;

	float m_fStartSpeed;
	float m_fEndSpeed;

public:
	virtual void UpdateData() override;
	virtual void Clear() override;
	virtual void Excute() override;

public:
	void SetParticleBuffer(CStructuredBuffer* _pBuffer) { m_pParticleBuffer = _pBuffer; }
	void SetStartSpeed(float _fSpeed) { m_fStartSpeed = _fSpeed; }
	void SetEndSpeed(float _fSpeed) { m_fEndSpeed = _fSpeed; }

public:
	CParticleUpdateShader();
	virtual ~CParticleUpdateShader() override;
};