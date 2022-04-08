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
	bool m_bIsCloneMtrlInnerEngine; // Default Material은 아니지만 Save, Load가 필요 없을 경우.

public:
	virtual bool Save(const tstring& _strRelativePath) override;
private:
	virtual int Load(const tstring& _strFilePath) override;

public:
	virtual void UpdateData() override;

public:
	//void SetShader(SharedPtr<CGraphicsShader>& _pShader);
	void SetShader(SharedPtr<CGraphicsShader> _pShader);
	SharedPtr<CGraphicsShader> GetShader() { return m_pShader; }
	void SetData(E_ShaderParam _eParam, const void* _pData);
	void GetData(E_ShaderParam _eParam, void* _pOut);

	void SetMaterialCoefficient(Vector4 _vDiff, Vector4 _vSpec, Vector4 _vAmb, Vector4 _vEmis)
	{
		m_tParam.mtrl.vDiffuse = _vDiff;
		m_tParam.mtrl.vAmbiant = _vAmb;
		m_tParam.mtrl.vSpecular = _vSpec;
		m_tParam.mtrl.vEmission = _vEmis;
	}


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