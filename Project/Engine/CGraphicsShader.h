#pragma once
#include "CShader.h"

class CGraphicsShader : public CShader 
{
private:
	ComPtr<ID3D11VertexShader>		m_pVS;
	ComPtr<ID3DBlob>				m_pVSBlob; // 컴파일 코드(Blob : 컴파일 된 바이너리 데이터)
	ComPtr<ID3D11HullShader>		m_pHS;
	ComPtr<ID3DBlob>				m_pHSBlob;
	ComPtr<ID3D11DomainShader>		m_pDS;
	ComPtr<ID3DBlob>				m_pDSBlob;
	ComPtr<ID3D11GeometryShader>	m_pGS;
	ComPtr<ID3DBlob>				m_pGSBlob;
	ComPtr<ID3D11PixelShader>		m_pPS;
	ComPtr<ID3DBlob>				m_pPSBlob;
	ComPtr<ID3DBlob>				m_pErrBlob; // 에러 메세지 저장 용도

	ComPtr<ID3D11InputLayout>		m_pLayout; // 입력 어셈블러 단계에 대한 입력 버퍼 데이터를 설명하는 입력 레이어웃 개체를 만든다.(GPU에 전달할 버텍스가 어떤 구성요소인지 알려주는 역할)
	D3D11_PRIMITIVE_TOPOLOGY		m_eTopology; 

public:
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _eTopology) { m_eTopology = _eTopology; }
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_eTopology; }

public:
	void CreateVertexShader(const tstring& _strRelativePath, const string& _strFuncName);
	//void CreateHullShader(const tstring& _strRelativePath, const string& _strFuncName);
	//void CreateDomainShader(const tstring& _strRelativePath, const string& _strFuncName);
	//void CreateGeometryShader(const tstring& _strRelativePath, const string& _strFuncName);
	void CreatePixelShader(const tstring& _strRelativePath, const string& _strFuncName);

public:
	virtual void Load(const tstring& _strFilePath) override {} ;
	virtual void UpdateData() override;

public:
	CGraphicsShader();
	virtual ~CGraphicsShader() override;
};