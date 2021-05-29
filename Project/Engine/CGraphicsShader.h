#pragma once
#include "CShader.h"

class CGraphicsShader : public CShader 
{
private:
	ComPtr<ID3D11VertexShader>		m_pVS;
	ComPtr<ID3DBlob>				m_pVSBlob; // ������ �ڵ�(Blob : ������ �� ���̳ʸ� ������)
	ComPtr<ID3D11HullShader>		m_pHS;
	ComPtr<ID3DBlob>				m_pHSBlob;
	ComPtr<ID3D11DomainShader>		m_pDS;
	ComPtr<ID3DBlob>				m_pDSBlob;
	ComPtr<ID3D11GeometryShader>	m_pGS;
	ComPtr<ID3DBlob>				m_pGSBlob;
	ComPtr<ID3D11PixelShader>		m_pPS;
	ComPtr<ID3DBlob>				m_pPSBlob;
	ComPtr<ID3DBlob>				m_pErrBlob; // ���� �޼��� ���� �뵵

	ComPtr<ID3D11InputLayout>		m_pLayout; // �Է� ����� �ܰ迡 ���� �Է� ���� �����͸� �����ϴ� �Է� ���̾�� ��ü�� �����.(GPU�� ������ ���ؽ��� � ����������� �˷��ִ� ����)
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