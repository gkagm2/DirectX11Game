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

	vector<CCamera*> m_vecToolCam; // Tool�� ī�޶�

	unique_ptr<CStructuredBuffer> m_pLight2DBuffer;
	SharedPtr<CTexture> m_pPostEffectTargetTex;
public:
	void Init();
	void Render(); 

private:
	void _RenderInGame(); // Play �� ������
	void _RenderTool(); // Tool�� ������

public:
	// In Game Camera
	void RegisterCamera(CCamera* _pCamera) { m_vecCam.push_back(_pCamera); }
	void UnRegisterCamera() { m_vecCam.clear(); }
	

	int RegisterLight2D(CLight2D* _pLight2D) { 
		m_vecLight2D.push_back(_pLight2D);
		return (int)(m_vecLight2D.size() - 1);
	}

	// Tool Camera
	void RegisterToolCamera(CCamera* _pCamera) { m_vecToolCam.push_back(_pCamera); }
	void UnRegisterToolCamera() { m_vecToolCam.clear(); }

	CCamera* GetMainCamera();
	CCamera* GetUICamera();

	
	CCamera* GetToolCamera(const tstring& _strObjName = STR_TOOL_OBJ_NAME_ToolCamera);
	CCamera* GetToolUICamera(const tstring& _strObjName = STR_TOOL_OBJ_NAME_ToolUICamera);
	const vector<CCamera*>& GetToolCameras() { return m_vecToolCam; }

private:
	void _CopyBackBufferToPostEffectBuffer();

private:
	
	void _UpdateData_Light2D();
	void _RenderClear();

	friend void CCamera::_RenderPostEffect();
};