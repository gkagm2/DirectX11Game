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
	m_arrAtlasTileCnt{ 0,0 },

	m_iSelectedTileIdx(-1)
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

	// 창을 하나 연다
    if (!m_bGUIOpen)
        return;
	
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(STR_GUI_TileMapEditor, &m_bGUIOpen))
	{
		ImGui::Text("Tile Face Size: %d %d", m_pTileMap->GetCol(), m_pTileMap->GetRow());

		ImGui::InputInt2("##Tile Map Size", m_arrFaceTileCnt);
		m_arrFaceTileCnt[0] = CMyMath::Clamp(m_arrFaceTileCnt[0], 0, INT_MAX);
		m_arrFaceTileCnt[1] = CMyMath::Clamp(m_arrFaceTileCnt[1], 0, INT_MAX);

		ImGui::Spacing();

		ImGui::Text("Texture Tile Count");
		ImGui::InputInt2("##Texture Tile Count", m_arrAtlasTileCnt);
		m_arrAtlasTileCnt[0] = CMyMath::Clamp(m_arrAtlasTileCnt[0], 0, INT_MAX);
		m_arrAtlasTileCnt[1] = CMyMath::Clamp(m_arrAtlasTileCnt[1], 0, INT_MAX);
		ImGui::Spacing();

		// 타일 생성
		static bool bBlankCreate = true;
		ImGui::Checkbox("Blank Tile##TileMapEditorCreateInitType", &bBlankCreate);
		ImGui::SameLine();
		if (ImGui::Button("Create")) {
			if (0 != m_arrFaceTileCnt[0] &&
				0 != m_arrFaceTileCnt[1] &&
				0 != m_arrAtlasTileCnt[0] &&
				0 != m_arrAtlasTileCnt[1] &&
				nullptr != m_pTileMap->GetAtlasTexture().Get()) {
				m_pTileMap->SetAtlasTileCnt(m_arrAtlasTileCnt[0], m_arrAtlasTileCnt[1]);
				m_pTileMap->CreateTile(m_arrFaceTileCnt[0], m_arrFaceTileCnt[1], bBlankCreate);
			}
		}

		ImGui::Separator();

		// 아틀라스 텍스쳐 선택
		// ListView선택.

		// 아틀라스 텍스쳐 세팅
		CTexture* pAtlasTexture = m_pTileMap->GetAtlasTexture().Get();
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

		// canvas를 이용해서 하나 불러오기
		_RenderCanvas();


		// 텍스쳐 설정
		CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
		if (!pToolCam) {
			assert(pToolCam);
		}
		else {
			if (!ImGui::IsWindowFocused()) {
				// TODO (Jang) : 여기서부터
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

				// 마우스를 타일 내부에 클릭했는가?
				bool bIsTileClicked = false;

				if (InputKeyPress(E_Key::LBUTTON)) {
					if (vLTWorldPos.x < vMouseWorldPos.x && vRBPos.x > vMouseWorldPos.x &&
						vLTWorldPos.y > vMouseWorldPos.y && vRBPos.y < vMouseWorldPos.y) {
						bIsTileClicked = true;
					}
				}

				// 클릭했을 경우
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

	ImGui::Checkbox("Enable grid", &opt_enable_grid);
	ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
	ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

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

	// 왼쪽 버튼을 클릭했으면
	if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		// 타일의 위치 값 계산 후 선택한 타일의 인덱스를 가져온다.
		Vector2 vAtlasSize = m_pTileMap->GetAtlasTexture()->GetResolution();
		Vector2 vAtlasTileSize = m_pTileMap->GetAtlasTilePixelSize();
		Vector2 vTileIdx = Vector2(m_pTileMap->GetAtlasTileCol(), m_pTileMap->GetAtlasTileRow()) / vAtlasSize;

		// 타일을 클릭했는가?
		bool isTileClick = false;
		if (vAtlasSize.x > mouse_pos_in_canvas.x && vAtlasSize.y > mouse_pos_in_canvas.y &&
			0 < mouse_pos_in_canvas.x && 0 < mouse_pos_in_canvas.y)
			isTileClick = true;

		// 타일을 클릭하지 않았으면
		if (!isTileClick)
			m_iSelectedTileIdx = -1;
		else { // 타일을 클릭헀으면
			float iCol = mouse_pos_in_canvas.x / vAtlasTileSize.x;
			float iRow = mouse_pos_in_canvas.y / vAtlasTileSize.y;
			modf(iCol, &iCol);
			modf(iRow, &iRow);
			// 선택한 타일의 인덱스를 얻는다.
			m_iSelectedTileIdx = (int)iRow * m_pTileMap->GetAtlasTileRow() + (int)iCol;
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
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	}

	// Draw Atlas Texture
	if (m_pAtlasTileTex) {
		TTextureBtnInfo tTexBtnInfo = { };
		ImVec2 vAtlasTexResolution = ImVec2(origin.x + m_pAtlasTileTex->GetResolution().x, origin.y + m_pAtlasTileTex->GetResolution().y);
		draw_list->AddImage(m_pAtlasTileTex->GetSRV().Get(), origin, vAtlasTexResolution);
	}

	draw_list->PopClipRect();
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
	GetTargetObject()->TileMap()->SetTileAtlasTexture(pTex);
}

void TileMapEditorGUI::_Clear()
{
	m_pTargetObject = nullptr;
	m_pTileMap = nullptr;
	m_pAtlasTileTex = nullptr;
	m_arrFaceTileCnt[0] = m_arrFaceTileCnt[1] = 0;
	m_arrAtlasTileCnt[0] = m_arrAtlasTileCnt[1] = 0;
	m_iSelectedTileIdx = -1;
}