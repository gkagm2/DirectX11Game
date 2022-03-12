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

	ComPtr<ID3D11InputLayout>		m_pLayout; // 입력 어셈블러 단계에 대한 입력 버퍼 데이터를 설명하는 입력 레이어웃 개체를 만든다.(GPU에 전달할 버텍스가 어떤 구성요소인지 알려주는 역할)

	E_RenderTimePoint						m_eRenderTimePoint; // 렌더링 시점
	D3D11_PRIMITIVE_TOPOLOGY		m_eTopology; 
	E_RasterizerState				m_eRSState; // Rasterizer의 상태
	E_BlendState					m_eBlendState;
	E_DepthStencilState				m_eDepthStencilState;

public:
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _eTopology) { m_eTopology = _eTopology; }
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_eTopology; }

	void SetRasterizerState(E_RasterizerState _eRasterizerState) { m_eRSState = _eRasterizerState; }
	E_RasterizerState GetRasterizerState() { return m_eRSState; }
	void SetBlendState(E_BlendState _eBlendState) { m_eBlendState = _eBlendState; }
	void SetDepthStencilState(E_DepthStencilState _eDepthStencilState) { m_eDepthStencilState = _eDepthStencilState; }

	E_RenderTimePoint GetRenderTimePoint() { return m_eRenderTimePoint; }

public:
	void CreateVertexShader(const tstring& _strRelativePath, const string& _strFuncName);
	void CreateHullShader(const tstring& _strRelativePath, const string& _strFuncName);
	void CreateDomainShader(const tstring& _strRelativePath, const string& _strFuncName);
	void CreateGeometryShader(const tstring& _strRelativePath, const string& _strFuncName);
	void CreatePixelShader(const tstring& _strRelativePath, const string& _strFuncName);

public:
	virtual int Load(const tstring& _strFilePath) override { return S_OK; }
	virtual void UpdateData() override;

	static void Clear();

private:
	CLONE_DISABLE(CGraphicsShader);

public:
	CGraphicsShader();
	CGraphicsShader(E_RenderTimePoint _eTimePoint);
	virtual ~CGraphicsShader() override;
};