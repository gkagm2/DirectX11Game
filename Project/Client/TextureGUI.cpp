#include "pch.h"
#include "TextureGUI.h"
#include <Engine\CTexture.h>



TextureGUI::TextureGUI() :
	ResourceGUI(E_ResourceType::Texture),
	m_vSize(110.f,110.f),
	m_vUvMin(0.f,0.f),
	m_vUvMax(1.0f,1.0f),
	m_v4BGColor(0.0f,0.0f,0.0f,1.0f),
	m_vtintColor(1.0f,1.0f,1.0f,1.0f), // No tint
	m_v4BorderColor(1.0f,1.0f,1.0f,0.5f),// 50% opaque white,
	m_eImageMode(E_ImageMode::Image),
	m_bZoomMode(false),
	m_clickCallBackFunc(nullptr)
{
}

TextureGUI::~TextureGUI()
{
}

void TextureGUI::Update()
{
	if(false == Start())
		return;

	CTexture* pTexture = (CTexture*)GetTargetResource();

	string strKey;
	TStringToString(pTexture->GetKey(), strKey);
	ImGui::PushID(strKey.c_str());
	/*m_vUvMax = ImVec2(m_vSize.x / pTexture->GetDimension().x, m_vSize.y / pTexture->GetDimension().y);*/
	m_vUvMax = ImVec2(1.f, 1.f);
	ImTextureID texId = (ImTextureID)pTexture->GetSRV().Get();

	if (E_ImageMode::ButtonImage ==  m_eImageMode) {
		int iFramePadding = -1;
		if (ImGui::ImageButton(texId, m_vSize, m_vUvMin, m_vUvMax, iFramePadding, m_v4BGColor, m_vtintColor)) {
			if(m_clickCallBackFunc)
				m_clickCallBackFunc(); // 버튼 클릭 시 콜백 함수로 설정
		}
	}
	else if(E_ImageMode::Image == m_eImageMode) {
		ImGui::Image(texId, m_vSize, m_vUvMin, m_vUvMax, m_vtintColor, m_v4BorderColor);
	}

	/*static ImGuiComboFlags flags = (int)m_bZoomMode;
	ImGui::CheckboxFlags("Zoom", &flags, ImGuiComboFlags_PopupAlignLeft);
	m_bZoomMode = (bool)flags;*/

	// TODO (Jang) : Zoom할 때 영역 설정 문제
	// Zooom 
	if (m_bZoomMode) {
		if (ImGui::IsItemHovered())
		{
			ImGuiIO& io = ImGui::GetIO();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			float my_tex_w = pTexture->GetDimension().x;
			float my_tex_h = pTexture->GetDimension().y;

			ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);

			ImGui::BeginTooltip();
			float region_sz = m_vSize.x;
			float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
			float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
			float zoom = 4.0f;
			if (region_x < 0.0f) { region_x = 0.0f; }
			else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
			if (region_y < 0.0f) { region_y = 0.0f; }
			else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
			ImGui::Text("pos: (%.2f, %.2f)", pos.x, pos.y);
			ImGui::Text("isPos: (%.2f, %.2f)", io.MousePos.x, io.MousePos.y);
			ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
			ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
			ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
			ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
			ImGui::Image(texId, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, m_vtintColor, m_v4BorderColor);
			ImGui::EndTooltip();
		}
	}

	//if(ImGui::ImageButton())
	ImGui::PopID();

	End();
}