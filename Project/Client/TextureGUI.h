#pragma once
#include "ResourceGUI.h"
typedef void (*CLICK_CALLBACK)();

enum class E_ImageMode {
	Image,
	ButtonImage,
};

class TextureGUI : public ResourceGUI
{
private:
	ImVec2 m_vSize;			// Size of the image
	ImVec2 m_vUvMin;		// Left-Top
	ImVec2 m_vUvMax;		// Right-Bottom
	ImVec4 m_v4BGColor;		// Background color
	ImVec4 m_vtintColor;
	ImVec4 m_v4BorderColor;

	E_ImageMode m_eImageMode;
	bool m_bZoomMode;

	// 버튼 클릭 시 콜백으로 받아오게 하기
	CLICK_CALLBACK m_clickCallBackFunc;

public:
	virtual void Update() override;

public:
	// Button Mode일 경우 callbackFunc 설정
	void SetButtonMode(E_ImageMode _eMode, CLICK_CALLBACK _callBackFunc = nullptr) {
		m_eImageMode = _eMode;
		m_clickCallBackFunc = _callBackFunc;
	}

	void SetActiveZoom(bool _bActive) { m_bZoomMode = _bActive; }

public:
	TextureGUI();
	virtual ~TextureGUI() override;
};