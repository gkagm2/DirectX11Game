#pragma once
#include <Engine\CCamera.h>
#include "GUI.h"
class CCamera;
class CameraGUI;
class LayerCheckGUI : public GUI {
private:
	CameraGUI* m_pCamGUI;
	CCamera* m_pCam;

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void SetCamera(CCamera* _pCam, CameraGUI* _pCamGUI) { m_pCam = _pCam; m_pCamGUI = _pCamGUI; }

public:
	LayerCheckGUI();
	virtual ~LayerCheckGUI() override;
};