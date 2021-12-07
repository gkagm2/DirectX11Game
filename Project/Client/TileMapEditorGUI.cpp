#include "pch.h"
#include "TileMapEditorGUI.h"

#include "CImGuiManager.h"
#include <Engine\CTileMap.h>
#include <Engine\CGameObject.h>
#include <Engine\CResourceManager.h>
#include "ParamGUI.h"
#include <Engine\CRenderManager.h>
#include <Engine\CCamera2D.h>
#include "DebugGUI.h"
#include <Engine\CKeyManager.h>
#include <Engine\CTransform.h>

TileMapEditorGUI::TileMapEditorGUI() :
	m_pTargetObject(nullptr),
	m_pTileMap{nullptr},
	m_pAtlasTileTex {nullptr},
	m_arrFaceTileCnt{ 0,0 },
	m_vAtlasTilePixelSize{ 0, 0},

	m_iSelectedTileIdx(-1),

	// canvas
	m_iGridColor(IMGUI_COLOR_GREEN) // green
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
	if (!m_pTargetObject->TileMap())
		return;
	m_pTileMap = m_pTargetObject->TileMap();
	m_pAtlasTileTex = m_pTileMap->GetAtlasTexture().Get();

	// â�� �ϳ� ����
    if (!m_bGUIOpen)
        return;
	
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(STR_GUI_TileMapEditor, &m_bGUIOpen))
	{
		ImGui::Text("Atlas Texture Resolution [%d,%d]", (int)m_pTileMap->GetAtlasTexture()->GetResolution().x, (int)m_pTileMap->GetAtlasTexture()->GetResolution().y);

		ImGui::Text("Tile Face Size: %d %d", m_pTileMap->GetCol(), m_pTileMap->GetRow());

		ImGui::InputInt2("##Tile Map Size", m_arrFaceTileCnt);
		m_arrFaceTileCnt[0] = CMyMath::Clamp(m_arrFaceTileCnt[0], 0, INT_MAX);
		m_arrFaceTileCnt[1] = CMyMath::Clamp(m_arrFaceTileCnt[1], 0, INT_MAX);

		ImGui::Spacing();

		ImGui::Text("Texture Tile Size");
		ImGui::InputInt2("##Texture Tile Size", m_vAtlasTilePixelSize);
		m_vAtlasTilePixelSize[0] = CMyMath::Clamp(m_vAtlasTilePixelSize[0], 0, INT_MAX);
		m_vAtlasTilePixelSize[1] = CMyMath::Clamp(m_vAtlasTilePixelSize[1], 0, INT_MAX);
		ImGui::Spacing();

		// Ÿ�� ����
		static bool bBlankCreate = true;
		ImGui::Checkbox("Blank Tile##TileMapEditorCreateInitType", &bBlankCreate);
		ImGui::SameLine();
		if (ImGui::Button("Create")) {
			if (0 != m_arrFaceTileCnt[0] &&
				0 != m_arrFaceTileCnt[1] &&
				0 != m_vAtlasTilePixelSize[0] &&
				0 != m_vAtlasTilePixelSize[1] &&
				nullptr != m_pTileMap->GetAtlasTexture().Get()) {
				m_pTileMap->SetAtlasTilePixelSize(m_vAtlasTilePixelSize[0], m_vAtlasTilePixelSize[1]);
				m_pTileMap->CreateTile(m_arrFaceTileCnt[0], m_arrFaceTileCnt[1], bBlankCreate);
			}
		}

		ImGui::Separator();

		// ��Ʋ�� �ؽ��� ����
		// ListView����.

		// ��Ʋ�� �ؽ��� ����
		CTexture* pAtlasTexture = m_pTileMap->GetAtlasTexture().Get();
		if (pAtlasTexture) {
			string strName{};
			tstring tstrName = pAtlasTexture->GetName();
			TStringToString(tstrName, strName);
			ParamGUI::Render_Texture(strName.c_str(), pAtlasTexture, this, (GUI_CALLBACK)&TileMapEditorGUI::_SelectTexture);
		}
		else {
			if (ImGui::Button("Atlas Texture Select")) {
				// ��� ����
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

		// canvas�� �̿��ؼ� �ϳ� �ҷ�����
		_RenderCanvas();


		// �ؽ��� ����
		CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
		if (!pToolCam) {
			assert(pToolCam);
		}
		else {
			if (!ImGui::IsWindowFocused()) {
				Vector3 vMouseWorldPos = pToolCam->GetScreenToWorld2DPosition(MousePosition);

				Vector3 vObjWorldPos = GetTargetObject()->Transform()->GetPosition();
				Vector3 vHalfScale = Vector3(GetTargetObject()->Transform()->GetScale() * 0.5f);
				Vector3 vLTWorldPos = {};
				vLTWorldPos.x = vObjWorldPos.x - vHalfScale.x;
				vLTWorldPos.y = vObjWorldPos.y + vHalfScale.y;

				Vector3 vOriginLTPos = {};
				vOriginLTPos.x = vObjWorldPos.x - vLTWorldPos.x;
				vOriginLTPos.y = vObjWorldPos.y + vLTWorldPos.y;

				Vector3 vRBPos = {};
				vRBPos.x = vLTWorldPos.x + GetTargetObject()->Transform()->GetScale().x;
				vRBPos.y = vLTWorldPos.y - GetTargetObject()->Transform()->GetScale().y;

				// ���콺�� Ÿ�� ���ο� Ŭ���ߴ°�?
				bool bIsTileClicked = false;

				if (InputKeyHold(E_Key::LBUTTON)) {
					if (vLTWorldPos.x < vMouseWorldPos.x && vRBPos.x > vMouseWorldPos.x &&
						vLTWorldPos.y > vMouseWorldPos.y && vRBPos.y < vMouseWorldPos.y) {
						bIsTileClicked = true;
					}
				}

				// Ŭ������ ���
				if (bIsTileClicked) {
					vector<TTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
					Vector2 vOriginMousePos = {};
					vOriginMousePos.x = vMouseWorldPos.x - vLTWorldPos.x;
					vOriginMousePos.y = vLTWorldPos.y - vMouseWorldPos.y;
					int iClickX = (int)vOriginMousePos.x;
					int iClickY = (int)vOriginMousePos.y;

					int idx = iClickY * m_pTileMap->GetCol() + iClickX;

					vecTiles[idx].idx = m_iSelectedTileIdx;
					DBug->Debug("clickX,Y, idx[%d, %d %d]", iClickX, iClickY, idx);
				}
			}
		}
		ImGui::End();
    }
}

void TileMapEditorGUI::_SelectTileMap(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
}

void TileMapEditorGUI::_RenderCanvas()
{
	static ImVec2 scrolling(0.0f, 0.0f);
	static bool opt_enable_grid = true;
	static bool opt_enable_context_menu = true;
	static bool adding_line = false;
	
	static int arrGrid[2] = { 5.f, 5.f };

	// Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
	// Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
	// To use a child window instead we could use, e.g:
	//      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
	//      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
	//      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_NoMove);
	//      ImGui::PopStyleColor();
	//      ImGui::PopStyleVar();
	//      [...]
	//      ImGui::EndChild();

	// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	// ���� ��ư�� Ŭ��������
	if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		// Ÿ���� ��ġ �� ��� �� ������ Ÿ���� �ε����� �����´�.
		Vector2 vAtlasSize = m_pTileMap->GetAtlasTexture()->GetResolution();
		Vector2 vAtlasTileSize = m_pTileMap->GetAtlasTilePixelSize();

		// Ÿ���� Ŭ���ߴ°�?
		bool isTileClick = false;
		if (vAtlasSize.x > mouse_pos_in_canvas.x && vAtlasSize.y > mouse_pos_in_canvas.y &&
			0 < mouse_pos_in_canvas.x && 0 < mouse_pos_in_canvas.y)
			isTileClick = true;

		// Ÿ���� Ŭ������ �ʾ�����
		if (!isTileClick)
			m_iSelectedTileIdx = -1;
		else { // Ÿ���� Ŭ��������
			float iCol = mouse_pos_in_canvas.x / vAtlasTileSize.x;
			float iRow = mouse_pos_in_canvas.y / vAtlasTileSize.y;
			modf(iCol, &iCol);
			modf(iRow, &iRow);
			// ������ Ÿ���� �ε����� ��´�.
			m_iSelectedTileIdx = (int)iRow * m_pTileMap->GetAtlasTileYCnt() + (int)iCol;
		}
	}
	
	// Pan (we use a zero mouse threshold when there's no context menu)
	// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		Vector2 vGridSize = m_pTileMap->GetAtlasTilePixelSize();
		for (float x = fmodf(scrolling.x, vGridSize.x); x < canvas_sz.x; x += vGridSize.x)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), m_iGridColor);
		for (float y = fmodf(scrolling.y, vGridSize.y); y < canvas_sz.y; y += vGridSize.y)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), m_iGridColor);
	}

	// Draw Atlas Texture
	if (m_pAtlasTileTex) {
		TTextureBtnInfo tTexBtnInfo = { };
		ImVec2 vAtlasTexResolution = ImVec2(origin.x + m_pAtlasTileTex->GetResolution().x, origin.y + m_pAtlasTileTex->GetResolution().y);
		draw_list->AddImage(m_pAtlasTileTex->GetSRV().Get(), origin, vAtlasTexResolution);
	}

	// Ÿ���� ���� �Ǿ�����
	if (_IsTileSelectedInCanvas()) {
		Vector2 vAtlasResol = m_pTileMap->GetAtlasTexture()->GetResolution();
		Vector2 vAtlasTileSize = m_pTileMap->GetAtlasTilePixelSize();
		int iAtlasColCnt = m_pTileMap->GetAtlasTileXCnt();

		int iRow = m_iSelectedTileIdx / iAtlasColCnt;
		int iCol = m_iSelectedTileIdx % iAtlasColCnt;

		ImVec2 vLTPos = ImVec2(iCol * vAtlasTileSize.x, iRow * vAtlasTileSize.y);
		ImVec2  vRBPos = ImVec2((iCol + 1) * vAtlasTileSize.x, (iRow + 1) * vAtlasTileSize.y);

		vLTPos = ImVec2(vLTPos.x + origin.x, vLTPos.y + origin.y);
		vRBPos = ImVec2(vRBPos.x + origin.x, vRBPos.y + origin.y);
		draw_list->AddRect(vLTPos, vRBPos, IMGUI_COLOR_RED, 0.f, 0, 2.f);
	}

	draw_list->PopClipRect();
}

void TileMapEditorGUI::_SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
	// ������ �ؽ��ĸ� �˾Ƴ���.
	const char* pStrKey = (const char*)_strKey;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pTex = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pTex);
	GetTargetObject()->TileMap()->SetTileAtlasTexture(pTex);
}

void TileMapEditorGUI::_Clear()
{
	m_pTargetObject = nullptr;
	m_pTileMap = nullptr;
	m_pAtlasTileTex = nullptr;
	m_arrFaceTileCnt[0] = m_arrFaceTileCnt[1] = 0;
	m_vAtlasTilePixelSize[0] = m_vAtlasTilePixelSize[1] = 0;
	m_iSelectedTileIdx = -1;
}