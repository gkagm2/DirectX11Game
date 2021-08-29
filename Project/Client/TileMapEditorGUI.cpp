#include "pch.h"
#include "TileMapEditorGUI.h"

#include "CImGuiManager.h"
#include <Engine\CTileMap.h>
#include <Engine\CGameObject.h>
#include <Engine\CResourceManager.h>
#include "ParamGUI.h"

TileMapEditorGUI::TileMapEditorGUI() :
	m_pTargetObject(nullptr),
	m_iFaceSize{},
	m_iAtlasTileColRowSize{}
{
}

TileMapEditorGUI::~TileMapEditorGUI()
{
}

void TileMapEditorGUI::Init()
{
	SetName(STR_GUI_TileMapEditor);
}

void TileMapEditorGUI::Update()
{
	if (!m_pTargetObject)
		return;

	CTileMap* pTileMap = m_pTargetObject->TileMap();
	if (!pTileMap) {
		assert(nullptr);
		return;
	}
	// 창을 하나 연다
    if (!m_bGUIOpen)
        return;

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(STR_GUI_TileMapEditor, &m_bGUIOpen))
    {
		ImGui::Text("Face Size : %d %d", pTileMap->GetCol(), pTileMap->GetRow());
		ImGui::InputInt2("##Tile Map Size", m_iFaceSize);
		m_iFaceSize[0] = CMyMath::Clamp(m_iFaceSize[0], 0, INT_MAX);
		m_iFaceSize[1] = CMyMath::Clamp(m_iFaceSize[1], 0, INT_MAX);

		ImGui::Spacing();

		ImGui::Text("Texture Tile Count");
		ImGui::InputInt2("##Texture Tile Count", m_iAtlasTileColRowSize);
		m_iAtlasTileColRowSize[0] = CMyMath::Clamp(m_iAtlasTileColRowSize[0], 0, INT_MAX);
		m_iAtlasTileColRowSize[1] = CMyMath::Clamp(m_iAtlasTileColRowSize[1], 0, INT_MAX);
		ImGui::Spacing();


		if (ImGui::Button("Create")) {
			if (0 != m_iFaceSize[0] &&
				0 != m_iFaceSize[1] &&
				0 != m_iAtlasTileColRowSize[0] &&
				0 != m_iAtlasTileColRowSize[1] &&
				nullptr != pTileMap->GetAtlasTexture().Get()) {
				// 생성 할 타일 사이즈를 설정
				pTileMap->SetTileFaceSize(m_iFaceSize[0], m_iFaceSize[0]);

				// 아틀라스 텍스쳐의 가로 세로 개수
				pTileMap->SetAtlasTileColRowSize(m_iAtlasTileColRowSize[0], m_iAtlasTileColRowSize[1]);

				// 타일 분리
				pTileMap->SaperateTile();
			}
		}

		// 아틀라스 텍스쳐 선택
		// ListView선택.


		// 아틀라스 텍스쳐 세팅
		CTexture* pAtlasTexture = pTileMap->GetAtlasTexture().Get();
		if (pAtlasTexture) {
			string strName{};
			tstring tstrName = pAtlasTexture->GetName();
			TStringToString(tstrName, strName);
			ParamGUI::Render_Texture(strName.c_str(), pAtlasTexture, this, (GUI_CALLBACK)&TileMapEditorGUI::_SelectTexture);
		}
		else {
			if (ImGui::Button("Atlas Texture Select")) {
				// 목록 전달
				vector<tstring> vecName;
				CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecName);
				ListViewGUI* pListGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
				assert(pListGUI);
				if (pListGUI) {
					pListGUI->SetList(vecName, _T("Atlas Texture"));
					pListGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&TileMapEditorGUI::_SelectTexture);
					pListGUI->SetActive(true);
				}
			}
		}

		// 탭을 만들어서 하나는 타일맵의 텍스쳐를 수정하고
		// 세팅하는것.
		// 다른 탭은 그 텍스쳐를 이용해서 타일을 까는 것




		ImGui::PushItemWidth(-ImGui::GetFontSize() * 15);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		// Draw gradients
		// (note that those are currently exacerbating our sRGB/Linear issues)
		// Calling ImGui::GetColorU32() multiplies the given colors by the current Style Alpha, but you may pass the IM_COL32() directly as well..
		ImGui::Text("Gradients");
		ImVec2 gradient_size = ImVec2(ImGui::CalcItemWidth(), ImGui::GetFrameHeight());
		{
			ImVec2 p0 = ImGui::GetCursorScreenPos();
			ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
			ImU32 col_a = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));
			ImU32 col_b = ImGui::GetColorU32(IM_COL32(255, 255, 255, 255));
			draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
			ImGui::InvisibleButton("##gradient1", gradient_size);
		}
		{
			ImVec2 p0 = ImGui::GetCursorScreenPos();
			ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
			ImU32 col_a = ImGui::GetColorU32(IM_COL32(0, 255, 0, 255));
			ImU32 col_b = ImGui::GetColorU32(IM_COL32(255, 0, 0, 255));
			draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
			ImGui::InvisibleButton("##gradient2", gradient_size);
		}

		// Draw a bunch of primitives
		ImGui::Text("All primitives");
		static float sz = 36.0f;
		static float thickness = 3.0f;
		static int ngon_sides = 6;
		static bool circle_segments_override = false;
		static int circle_segments_override_v = 12;
		static bool curve_segments_override = false;
		static int curve_segments_override_v = 8;
		static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
		ImGui::DragFloat("Size", &sz, 0.2f, 2.0f, 100.0f, "%.0f");
		ImGui::DragFloat("Thickness", &thickness, 0.05f, 1.0f, 8.0f, "%.02f");
		ImGui::SliderInt("N-gon sides", &ngon_sides, 3, 12);
		ImGui::Checkbox("##circlesegmentoverride", &circle_segments_override);
		ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		circle_segments_override |= ImGui::SliderInt("Circle segments override", &circle_segments_override_v, 3, 40);
		ImGui::Checkbox("##curvessegmentoverride", &curve_segments_override);
		ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		curve_segments_override |= ImGui::SliderInt("Curves segments override", &curve_segments_override_v, 3, 40);
		ImGui::ColorEdit4("Color", &colf.x);

		const ImVec2 p = ImGui::GetCursorScreenPos();
		const ImU32 col = ImColor(colf);
		const float spacing = 10.0f;
		const ImDrawFlags corners_tl_br = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomRight;
		const float rounding = sz / 5.0f;
		const int circle_segments = circle_segments_override ? circle_segments_override_v : 0;
		const int curve_segments = curve_segments_override ? curve_segments_override_v : 0;
		float x = p.x + 4.0f;
		float y = p.y + 4.0f;
		for (int n = 0; n < 2; n++)
		{
			// First line uses a thickness of 1.0f, second line uses the configurable thickness
			float th = (n == 0) ? 1.0f : thickness;
			draw_list->AddNgon(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides, th);                 x += sz + spacing;  // N-gon
			draw_list->AddCircle(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments, th);          x += sz + spacing;  // Circle
			draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 0.0f, ImDrawFlags_None, th);          x += sz + spacing;  // Square
			draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, rounding, ImDrawFlags_None, th);      x += sz + spacing;  // Square with all rounded corners
			draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, rounding, corners_tl_br, th);         x += sz + spacing;  // Square with two rounded corners
			draw_list->AddTriangle(ImVec2(x + sz * 0.5f, y), ImVec2(x + sz, y + sz - 0.5f), ImVec2(x, y + sz - 0.5f), col, th); x += sz + spacing;  // Triangle
			//draw_list->AddTriangle(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col, th);x+= sz*0.4f + spacing; // Thin triangle
			draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y), col, th);                                       x += sz + spacing;  // Horizontal line (note: drawing a filled rectangle will be faster!)
			draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + sz), col, th);                                       x += spacing;       // Vertical line (note: drawing a filled rectangle will be faster!)
			draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y + sz), col, th);                                  x += sz + spacing;  // Diagonal line

			// Quadratic Bezier Curve (3 control points)
			ImVec2 cp3[3] = { ImVec2(x, y + sz * 0.6f), ImVec2(x + sz * 0.5f, y - sz * 0.4f), ImVec2(x + sz, y + sz) };
			draw_list->AddBezierQuadratic(cp3[0], cp3[1], cp3[2], col, th, curve_segments); x += sz + spacing;

			// Cubic Bezier Curve (4 control points)
			ImVec2 cp4[4] = { ImVec2(x, y), ImVec2(x + sz * 1.3f, y + sz * 0.3f), ImVec2(x + sz - sz * 1.3f, y + sz - sz * 0.3f), ImVec2(x + sz, y + sz) };
			draw_list->AddBezierCubic(cp4[0], cp4[1], cp4[2], cp4[3], col, th, curve_segments);

			x = p.x + 4;
			y += sz + spacing;
		}
		draw_list->AddNgonFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides);               x += sz + spacing;  // N-gon
		draw_list->AddCircleFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments);            x += sz + spacing;  // Circle
		draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);                                    x += sz + spacing;  // Square
		draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f);                             x += sz + spacing;  // Square with all rounded corners
		draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f, corners_tl_br);              x += sz + spacing;  // Square with two rounded corners
		draw_list->AddTriangleFilled(ImVec2(x + sz * 0.5f, y), ImVec2(x + sz, y + sz - 0.5f), ImVec2(x, y + sz - 0.5f), col);  x += sz + spacing;  // Triangle
		//draw_list->AddTriangleFilled(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col); x += sz*0.4f + spacing; // Thin triangle
		draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + thickness), col);                             x += sz + spacing;  // Horizontal line (faster than AddLine, but only handle integer thickness)
		draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + thickness, y + sz), col);                             x += spacing * 2.0f;// Vertical line (faster than AddLine, but only handle integer thickness)
		draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + 1, y + 1), col);                                      x += sz;            // Pixel (faster than AddLine)
		draw_list->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + sz, y + sz), IM_COL32(0, 0, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255), IM_COL32(0, 255, 0, 255));

		ImGui::Dummy(ImVec2((sz + spacing) * 10.2f, (sz + spacing) * 3.0f));
		ImGui::PopItemWidth();
    }
    ImGui::End();

    
}

void TileMapEditorGUI::_SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
	// 선택한 텍스쳐를 알아낸다.
	const char* pStrKey = (const char*)_strKey;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pTex = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pTex);
	GetTargetObject()->TileMap()->SetTileAtlas(pTex);
}

void TileMapEditorGUI::_Clear()
{
	m_iFaceSize[0] = m_iFaceSize[1] = 0;
	m_iAtlasTileColRowSize[0] = m_iAtlasTileColRowSize[1] = 0;
}
