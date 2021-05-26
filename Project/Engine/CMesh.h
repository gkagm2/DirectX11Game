#pragma once
#include "CResource.h"
class CMesh : public CResource
{
private:
	ComPtr<ID3D11Buffer> m_pVB;  // 정점 정보를 저장하는 버퍼 vertex buffer
	ComPtr<ID3D11Buffer> m_pIB;  // 인덱스 정보를 저장하는 버퍼 index buffer
	void* m_pVtxSys;			 //  VtxSys : Vertex System
	void* m_pIdxSys;			 //  IdxSys :: Index System

	D3D11_BUFFER_DESC m_tVtxDesc;
	D3D11_BUFFER_DESC m_tIdxDesc;

public:
	virtual void Load(const tstring _strFilePath) {};
	virtual void Create(void* _pVtxSys, UINT _iVtxBufferSize, void* _pIdxSys, UINT _iIdxBufferSize, D3D11_USAGE _eIdxUsage);
	virtual void UpdateData();

public:
	CMesh();
	virtual ~CMesh() override;
};