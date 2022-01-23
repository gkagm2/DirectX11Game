#pragma once
#include "LightGUI.h"
class Light2DGUI : public LightGUI
{
private:
	CLight2D* m_pLight;
public:
	virtual void Update() override;
	
protected:
	virtual void _InitComboBoxList() override;

private:
	void _RenderDirection(TLightInfo& tLightInfoRef);
	void _RenderSpot(TLightInfo& tLightInfoRef);
	void _RenderPoint(TLightInfo& tLightInfoRef);
	
	// ¿ä¼Ò
	void _RenderParam_Range(TLightInfo& _tLightInfoRef);
	void _RenderParam_LightColor(TLightInfo& _tLightInfoRef);

	void _SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw);
	void _SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw);

public:
	Light2DGUI();
	virtual ~Light2DGUI() override;
};