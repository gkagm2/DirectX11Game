#pragma once
#include "CResource.h"
#include "CFBXLoader.h"

class CStructuredBuffer;

struct tIndexInfo
{
	ComPtr<ID3D11Buffer>    pIB;
	D3D11_BUFFER_DESC       tIBDesc;
	UINT				    iIdxCount;
	void* pIdxSysMem; // Index System
};



class CMesh : public CResource
{
private:
	ComPtr<ID3D11Buffer> m_pVB;  // 정점 정보를 저장하는 버퍼 vertex buffer
	D3D11_BUFFER_DESC m_tVtxDesc;
	UINT m_iVtxCount;
	void* m_pVtxSys;			 //  VtxSys : Vertex System (VTX*)

	// 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
	vector<tIndexInfo>		m_vecIdxInfo;

	// Animation3D 정보
	vector<tMTAnimClip>		m_vecAnimClip;
	vector<tMTBone>			m_vecBones;

	CStructuredBuffer* m_pBoneFrameData;   // 전체 본 프레임 정보
	CStructuredBuffer* m_pBoneOffset;	    // 각 뼈의 offset 행렬

public:
	void* GetVtxSysMem() { return m_pVtxSys; }

private:
	virtual int Load(const tstring& _strFilePath) { return S_OK; }
	
public:
	virtual void Create(void* _pVtxSys, UINT _iVtxBufferSize, void* _pIdxSys, UINT _iIdxBufferSize, D3D11_USAGE _eIdxUsage);
	
private:
	//virtual void UpdateData() override;
	virtual void UpdateData(UINT _iSubSet);

public:
	void Render(UINT _iSubset);
	void Render_Particle(UINT _iCount);
	void Reset();

public:
	static CMesh* CreateFromContainer(CFBXLoader& _loader);
	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); } // index buffer count
	const vector<tMTBone>* GetBones() { return &m_vecBones; }
	UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
	const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
	bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

	CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // 전체 본 프레임 정보
	CStructuredBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // 각 뼈의 offset 행렬	

private:
	CLONE_DISABLE(CMesh);

public:
	CMesh();
	virtual ~CMesh() override;
};