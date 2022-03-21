#pragma once
#include "CSingleton.h"
#include "CCamera.h"
#include "CLight2D.h"
#include "CLight3D.h"
#include "CTileMap.h"

enum class E_MRTType {
	SwapChain,
	Deferred,
	Light,
	ShadowDepth,
	Decal,
	End
};
tstring MRTTypeToStr(E_MRTType _eType);

class CStructuredBuffer;
class CMRT;
class CRenderManager : public CSingleton<CRenderManager>
{
	SINGLETON(CRenderManager)
private:
	vector<CCamera*> m_vecCam;
	vector<CLight2D*> m_vecLight2D;
	vector<CLight3D*> m_vecLight3D;
	// TODO (Jang) : vector로 바꾸기 (directional light)
	CLight3D* m_pMainDirLight; // 방향성 광원

	vector<CCamera*> m_vecToolCam; // Tool용 카메라

	unique_ptr<CStructuredBuffer> m_pLight2DBuffer;
	unique_ptr<CStructuredBuffer> m_pLight3DBuffer;
	SharedPtr<CTexture> m_pPostEffectTargetTex;

	CMRT* m_arrMRT[(UINT)E_MRTType::End];

	// Debug용 렌더링
	vector<CGameObject*> m_vecDebugObjPool; // 실제 원본을 가리키고 있음
	list<CGameObject*> m_listDebugObj;  // Rendering 용도
	SharedPtr<CGraphicsShader> m_pDebugShader;
	SharedPtr<CMaterial> m_pDebugMtrl;

public:
	void Init();
	void Update();
	void Render(); 

private:
	void _RenderInGame(); // Play 시 렌더링
	void _RenderTool(); // Tool용 렌더링
	void _RenderDebug(); // Debug용 렌더링

public:
	// In Game Camera
	void RegisterCamera(CCamera* _pCamera) { m_vecCam.push_back(_pCamera); }
	void UnRegisterCamera() { m_vecCam.clear(); }
	

	int RegisterLight2D(CLight2D* _pLight2D) { 
		m_vecLight2D.push_back(_pLight2D);
		return (int)(m_vecLight2D.size() - 1);
	}
	int RegisterLight3D(CLight3D* _pLight3D);

	// Tool Camera
	void RegisterToolCamera(CCamera* _pCamera) { m_vecToolCam.push_back(_pCamera); }
	void UnRegisterToolCamera() { m_vecToolCam.clear(); }

	CCamera* GetMainCamera();
	CCamera* GetUICamera();
	CCamera* GetInGameMainCamera();
	
	CCamera* GetToolCamera(const tstring& _strObjName = STR_TOOL_OBJ_NAME_ToolCamera);
	CCamera* GetToolUICamera(const tstring& _strObjName = STR_TOOL_OBJ_NAME_ToolUICamera);
	const vector<CCamera*>& GetToolCameras() { return m_vecToolCam; }

	CMRT* GetMultipleRenderTargets(E_MRTType _eType) { return m_arrMRT[(UINT)_eType]; }

public:
	void RenderDebugSphere(const Vector3& _vWorldPos, const Vector3& _vLocalRot, const Vector3& _vColor, float _fRadius, float _fLifeTime);
private:
	CGameObject* _GetDebugGameObject();
	CGameObject* _CreateDebugGameObject();

private:
	void _CopyBackBufferToPostEffectBuffer();
	void _CreateMultpleRenderTargets();
private:
	void _UpdateData_Light2D();
	void _UpdateData_Light3D();
	void _Update_GlobalData();
	void _RenderClear();
	void _Render_Dynamic_ShadowDepth();


	friend void CCamera::_RenderPostEffect();
};