#pragma once
#include "CShader.h"

class CConstBuffer;
class CComputeShader : public CShader
{
protected:
	static const CConstBuffer* g_pMtrlBuffer;

private:
	ComPtr<ID3DBlob> m_pCSBlob;		   // Compute Shader Blob
	ComPtr<ID3D11ComputeShader> m_pCS; // CS : ComputeShader

	UINT m_iThreadNumX;
	UINT m_iThreadNumY;
	UINT m_iThreadNumZ;

protected:
	TMaterialParam  m_tInfo;

public:
	void CreateComputeShader(const tstring& _strRelativePath, const string& _strFuncName);

public:
	virtual void UpdateData() = 0;
	virtual void Clear() = 0;
	virtual void Excute() = 0;

public:
	void Dispatch(UINT _x, UINT _y, UINT _z);
	UINT GetThreadX() { return m_iThreadNumX; }
	UINT GetThreadY() { return m_iThreadNumY; }
	UINT GetThreadZ() { return m_iThreadNumZ; }

	virtual int Load(const tstring& _strFilePath) override { return S_OK; }

	CLONE_DISABLE(CComputeShader);

public:
	CComputeShader(UINT _iThreadNumX, UINT _iThreadNumY, UINT _iThreadNumZ);
	virtual ~CComputeShader() override;
};