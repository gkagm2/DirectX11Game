#pragma once
#include "LightGUI.h"
class Light3DGUI : public LightGUI
{
private:
	CLight3D* m_pLight;
public:
	virtual void Update() override;

protected:
	virtual void _InitComboBoxList() override;

private:
	void _RenderDirection(TLightInfo& _tLightInfoRef);
	void _RenderSpot(TLightInfo& _tLightInfoRef);
	void _RenderPoint(TLightInfo& _tLightInfoRef);

	// ¿ä¼Ò
	void _RenderParam_Range(TLightInfo& _tLightInfoRef);
	void _RenderParam_LightColor(TLightInfo& _tLightInfoRef);

public:
	Light3DGUI();
	virtual ~Light3DGUI() override;
};