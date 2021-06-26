#pragma once
#include "CSingleton.h"
#include "CCamera.h"
#include "CLight2D.h"

class CStructuredBuffer;
class CRenderManager : public CSingleton<CRenderManager>
{
	SINGLETON(CRenderManager)
private:
	vector<CCamera*> m_vecCam;
	vector<CLight2D*> m_vecLight2D;

	CStructuredBuffer* m_pLight2DBuffer;
public:
	void Init();
	void Render();

public:
	void RegisterCamera(CCamera* _pCamera) { m_vecCam.push_back(_pCamera); }
	void RegisterLight2D(CLight2D* _pLight2D) { m_vecLight2D.push_back(_pLight2D); }

private:
	void _RenderInit();
};