#pragma once
#include "CShader.h"

class CStructuredBuffer;
class CComputeShader : public CShader
{
private:
	ComPtr<ID3DBlob> m_pCSBlob;		   // Compute Shader Blob
	ComPtr<ID3D11ComputeShader> m_pCS; // CS : ComputeShader

	// 그룹 당 스레드 개수
	UINT m_iGroupThreadNumX;
	UINT m_iGroupThreadNumY;
	UINT m_iGroupThreadNumZ;

protected:
	TMaterialParam  m_tInfo;

public:
	void CreateComputeShader(const tstring& _strRelativePath, const string& _strFuncName);

public:
	virtual void UpdateData() = 0;
	virtual void Excute() = 0;
	virtual void Clear() = 0;

public:
	void Dispatch(UINT _iGroupX, UINT _iGroupY, UINT _iGroupZ);
	UINT GetGroupPerThreadX() { return m_iGroupThreadNumX; }
	UINT GetGroupPerThreadY() { return m_iGroupThreadNumY; }
	UINT GetGroupPerThreadZ() { return m_iGroupThreadNumZ; }

	virtual int Load(const tstring& _strFilePath) override { return S_OK; }

	CLONE_DISABLE(CComputeShader);

public:
	CComputeShader(UINT _iThreadNumX, UINT _iThreadNumY, UINT _iThreadNumZ);
	virtual ~CComputeShader() override;
};