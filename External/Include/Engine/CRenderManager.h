#pragma once
#include "CSingleton.h"
#include "CCamera.h"
#include "CLight2D.h"
#include "CTileMap.h"

class CStructuredBuffer;
class CRenderManager : public CSingleton<CRenderManager>
{
	SINGLETON(CRenderManager)
private:
	vector<CCamera*> m_vecCam;
	vector<CLight2D*> m_vecLight2D;
	vector<CTileMap*> m_vecTileMap;

	vector<CCamera*> m_vecToolCam; // Tool용 카메라

	unique_ptr<CStructuredBuffer> m_pLight2DBuffer;
	SharedPtr<CTexture> m_pPostEffectTargetTex;
public:
	void Init();
	void Render(); 

private:
	void _RenderInGame(); // Play 시 렌더링
	void _RenderTool(); // Tool용 렌더링

public:
	// In Game Camera
	void RegisterCamera(CCamera* _pCamera) { m_vecCam.push_back(_pCamera); }
	void UnRegisterCamera() { m_vecCam.clear(); }
	

	void RegisterLight2D(CLight2D* _pLight2D) { m_vecLight2D.push_back(_pLight2D); }
	void RegisterTileMap(CTileMap* _pTileMap) { m_vecTileMap.push_back(_pTileMap); }

	// Tool Camera
	void RegisterToolCamera(CCamera* _pCamera) { m_vecToolCam.push_back(_pCamera); }
	void UnRegisterToolCamera() { m_vecToolCam.clear(); }

	//
	CCamera* GetMainCamera();
	CCamera* GetUICamera();

	
	CCamera* GetToolCamera(const tstring& _strObjName = STR_TOOL_OBJ_NAME_ToolCamera);
	CCamera* GetToolUICamera(const tstring& _strObjName = STR_TOOL_OBJ_NAME_ToolUICamera);
	const vector<CCamera*>& GetToolCameras() { return m_vecToolCam; }

private:
	void _CopyBackBuffer();

private:
	
	void _RenderInit_Light2D();
	void _RenderClear();

	friend void CCamera::_RenderPostEffect();
};