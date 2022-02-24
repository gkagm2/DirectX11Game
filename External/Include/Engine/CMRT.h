#pragma once
#include "CObject.h"

#define MAX_RENDER_TARGET_TEX_CNT 8

class CMRT : public CObject
{
private:
	SharedPtr<CTexture> m_arrRTTex[MAX_RENDER_TARGET_TEX_CNT]; // Render Target Texture
	SharedPtr<CTexture> m_pDSTex;		// Depth Stencil Texture
	Vector4 m_arrClearColor[MAX_RENDER_TARGET_TEX_CNT];

	ID3D11RenderTargetView* m_arrRTV[MAX_RENDER_TARGET_TEX_CNT];	// Render Target View
	 
	UINT m_iRTCount;	// Render Target Count
	bool m_bUsePrevDS;	// Previous Depth Stencil (이전의 MRT에서 깊이 스텐실 버퍼가 있다면 이어받음 여부)

public:
	virtual void UpdateData() override;
	void Clear();

	UINT GetRenderTargetCount() { return m_iRTCount; }
	bool DidUsePrevDS() { return m_bUsePrevDS; }
	SharedPtr<CTexture>* GetRenderTargetTextures() { return m_arrRTTex; }
	SharedPtr<CTexture> GetDepthStencilTexture() { return m_pDSTex; }
	Vector4* GetClearColor() { return m_arrClearColor; }
	

public:
	CLONE_DISABLE(CMRT);
	CMRT() = delete;
	CMRT(SharedPtr<CTexture>* _pArrRTTex, Vector4* _pArrClearColor, UINT _iRTCount, SharedPtr<CTexture> _pDSTex, bool _bUseDepthPrev = false);
	virtual ~CMRT() override;
};