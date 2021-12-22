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
	bool m_bIsDefaultMtrl; // �������� �����ϴ� �⺻ ���͸���
	bool m_bIsCloneMtrlInnerEngine; // Default Material�� �ƴ����� Save, Load�� �ʿ� ���� ���.

public:
	virtual bool Save(const tstring& _strRelativePath) override;
private:
	virtual int Load(const tstring& _strFilePath) override;

public:
	virtual void UpdateData() override;

public:
	void SetShader(SharedPtr<CGraphicsShader>& _pShader);
	SharedPtr<CGraphicsShader> GetShader() { return m_pShader; }
	void SetData(E_ShaderParam _eParam, const void* _pData);
	void GetData(E_ShaderParam _eParam, void* _pOut);

	bool IsDefaultMaterial() { return m_bIsDefaultMtrl; }
	static void Clear();

private:
	void _SetCloneMtrlInnerEngineFlag() { m_bIsCloneMtrlInnerEngine = true; }
	
public:
	virtual CMaterial* Clone() override;
	virtual CMaterial* Clone_NoAddInResMgr();
	
	CMaterial();
	CMaterial(const CMaterial& _origin);
	CMaterial(bool _bIsDefaultMaterial);
	virtual ~CMaterial() override;
};