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
	bool m_bUsePrevDS;	// Previous Depth Stencil

public:
	virtual void UpdateData() override;
	void Clear();

public:
	CLONE_DISABLE(CMRT);
	CMRT() = delete;
	CMRT(SharedPtr<CTexture> _pArrRTTex[MAX_RENDER_TARGET_TEX_CNT], Vector4 _pArrClearColor[MAX_RENDER_TARGET_TEX_CNT], UINT _iRTCount, SharedPtr<CTexture> _pDSTex, bool _bUseDepthPrev = false);
	virtual ~CMRT() override;
};