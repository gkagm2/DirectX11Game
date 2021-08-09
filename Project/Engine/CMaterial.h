#pragma once
#include "CResource.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

class CMaterial : public CResource
{
private:
	SharedPtr<CGraphicsShader> m_pShader;

	TMaterialParam m_tParam;
	SharedPtr<CTexture> m_arrTexture[(UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0];
	bool m_bIsDefaultMtrl; // 엔진에서 제공하는 기본 메터리얼

private:
	virtual bool Save(const tstring& _strRelativePath) override;
	virtual int Load(const tstring& _strFilePath) override;

public:
	virtual void UpdateData() override;

public:
	void SetShader(SharedPtr<CGraphicsShader>& _pShader) { m_pShader = _pShader; }
	SharedPtr<CGraphicsShader> GetShader() { return m_pShader; }
	void SetData(E_ShaderParam _eParam, void* _pData);
	void GetData(E_ShaderParam _eParam, void* _pOut);

	static void Clear();
	
public:
	virtual CMaterial* Clone() override;
	CMaterial();
	CMaterial(bool _bIsDefaultMaterial);
	virtual ~CMaterial() override;
};