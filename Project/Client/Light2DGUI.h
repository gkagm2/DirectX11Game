#pragma once
#include "LightGUI.h"
class Light2DGUI : public LightGUI
{
private:
	CLight2D* m_pLight;
public:
	virtual void Update() override;

private:
	void _RenderDirection(TLightInfo& _tLightInfoRef);
	void _RenderSpot(TLightInfo& _tLightInfoRef);
	void _RenderPoint(TLightInfo& _tLightInfoRef);
	
	// ¿ä¼Ò
	void _RenderParam_Range(TLightInfo& _tLightInfoRef);
	void _RenderParam_LightColor(TLightInfo& _tLightInfoRef);



public:
	Light2DGUI();
	virtual ~Light2DGUI() override;
};