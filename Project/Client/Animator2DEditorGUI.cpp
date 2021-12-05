#include "pch.h"
#include "Animator2DEditorGUI.h"
#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>
#include <Engine\CGameObject.h>
#include "CImGuiManager.h"
#include <Engine\CResourceManager.h>
#include <Engine\CTexture.h>
#include "ParamGUI.h"
#include "TextureGUI.h"
#include <Engine\CMeshRenderer.h>

Animator2DEditorGUI::Animator2DEditorGUI() :
	m_pTargetObject(nullptr),
	m_pAnimator2D(nullptr),
	m_pLoadedAtlasTexture(nullptr),
	m_nameBuff{},
	m_queResultTexList{},
	m_iSelectedIdx{ -1 },
	m_bolorRectColor{ IM_COL_WHITE },
	m_eEditMode{E_EditMode::SliceSprite_Grid},
	m_bShowModifyPanel(false),

	// canvas properties
	vAtlasSize{ 500.f, 500.f },
	points{},
	opt_enable_grid{ true },
	opt_enable_context_menu{ true },
	canvas_p0{},
	canvas_sz{},
	canvas_p1{},
	draw_list{ nullptr }
{
}

void Animator2DEditorGUI::_Clear()
{
	SetTargetObject(nullptr);
	m_iSelectedIdx = -1;
	ZeroMemory(m_nameBuff, sizeof(m_nameBuff));
	m_queMinorTexList.clear();
	m_queResultTexList.clear();
}
Animator2DEditorGUI::~Animator2DEditorGUI()
{
}

void Animator2DEditorGUI::_OnLoadAtlasTexture()
{
	// 리스트 뷰를 보여준다.
	ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
	assert(pListViewGUI);

	vector<tstring> vecNames;
	CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecNames);
	pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Texture));
	pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&Animator2DEditorGUI::_SelectLoadedAtlasTexture);
	pListViewGUI->SetActive(true);
}

void Animator2DEditorGUI::Init()
{
	SetName(STR_GUI_Animator2DEditor);
	SetActive(false);
}

void Animator2DEditorGUI::Update()
{
	// 창을 하나 연다
	if (!m_bGUIOpen) {
		_Clear();
		return;
	}

	if (m_pTargetObject)
		m_pAnimator2D = m_pTargetObject->Animator2D();


	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(STR_GUI_Animator2DEditor, &m_bGUIOpen))
	{
		if (ImGui::BeginTabBar("##Animator2DTabBar"))
		{
			if (ImGui::BeginTabItem("Animator2D##Animator2D")) {
				_CanvasDrawPanel();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}


inline void Animator2DEditorGUI::SetAtlasTexture(CTexture* _pTex)
{
	m_pLoadedAtlasTexture = _pTex;
}

inline CTexture* Animator2DEditorGUI::GetAtlasTexture()
{
	return m_pLoadedAtlasTexture;
}

void Animator2DEditorGUI::_SetAtlasTexture(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	string strKey = (const char*)_dw1;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CTexture> pTex = CResourceManager::GetInstance()->FindRes<CTexture>(tstrKey);
	assert(pTex.Get());
	SetAtlasTexture(pTex.Get());
}


void Animator2DEditorGUI::_SelectLoadedAtlasTexture(DWORD_PTR _pStr, DWORD_PTR _NONE)
{
	const char* pStrKey = (const char*)_pStr;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pImageTexture = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pImageTexture);
	SetAtlasTexture(pImageTexture);
}

void Animator2DEditorGUI::_CanvasDrawPanel()
{
	Vector2 vAtlasSize = Vector2(500.f, 500.f);
	if (m_pLoadedAtlasTexture)
		vAtlasSize = m_pLoadedAtlasTexture->GetResolution();

	static int grids[2] = { 1.f, 1.f };

	// Left Top Menu
	ImGui::Text("Texture Border");
	_CanvasTopPanel(); // Top Panel 

	//ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (E_EditMode::SliceSprite_Grid == m_eEditMode) {
		_CanvasGridSliceMode();
	}
	else if (E_EditMode::SliceSprite == m_eEditMode) {
		_CanvasSliceMode();
	}

	// Right Panel

	// 분할된 이미지 보여주기
	_ShowSplitedSprite();
	// 애니메이션 이미지 수정하기
	if (m_bShowModifyPanel)
		_ModifyAniationPanel();
}

void Animator2DEditorGUI::_CanvasTopPanel()
{
	// Text Border tab buttons
	if (ImGui::Button("Load Texture##animator2DEditor")) {
		_OnLoadAtlasTexture();
	}
	ImGui::SameLine();
	if (ImGui::Button("Slice Sprite Grid##animator2DEditor")) {
		m_eEditMode = E_EditMode::SliceSprite_Grid;
	}
	ImGui::SameLine();
	if (ImGui::Button("Slice Sprite##animator2DEditor")) {
		m_eEditMode = E_EditMode::SliceSprite;
	}
	ImGui::SameLine();
	if (ImGui::Button("Trim Slice##animator2DEditor")) {
		m_eEditMode = E_EditMode::TrimSlice;
	}
}

TSelectTexInfo Animator2DEditorGUI::_FindMinorTexIdx(ImVec2 _mousPos, ImVec2 _canvasSize, int iCol, int iRow, const ImVec2& _vImageSize)
{
	float fGridStepWidth = _canvasSize.x / iCol;
	float fGridStepHeight = _canvasSize.y / iRow;

	int selCol = (int)(_mousPos.x / fGridStepWidth);
	int selRow = (int)(_mousPos.y / fGridStepHeight);

	TSelectTexInfo tSelInfo = {};
	tSelInfo.col = selCol;
	tSelInfo.row = selRow;

	tSelInfo.rect = _GetMinMaxRectFromColRow(fGridStepWidth, fGridStepHeight, tSelInfo.col, tSelInfo.row, _vImageSize);

	return tSelInfo;
}

TRect Animator2DEditorGUI::_GetMinMaxRectFromColRow(int _gridStepWidth, int _gridStepHeight, int iCol, int iRow, const ImVec2& _vImageSize)
{
	TRect tRect = {};
	tRect.rb = ImVec2((iCol + 1) * _gridStepWidth, (iRow + 1) * _gridStepHeight);
	tRect.rt = ImVec2(tRect.rb.x, tRect.rb.y - _gridStepHeight);
	tRect.lb = ImVec2(tRect.rb.x - _gridStepWidth, tRect.rb.y);
	tRect.lt = ImVec2(tRect.lb.x, tRect.rt.y);

	tRect.rbUV.x = tRect.rb.x / _vImageSize.x;
	tRect.rbUV.y = tRect.rb.y / _vImageSize.y;

	tRect.rtUV.x = tRect.rt.x / _vImageSize.x;
	tRect.rtUV.y = tRect.rt.y / _vImageSize.y;

	tRect.lbUV.x = tRect.lb.x / _vImageSize.x;
	tRect.lbUV.y = tRect.lb.y / _vImageSize.y;

	tRect.ltUV.x = tRect.lt.x / _vImageSize.x;
	tRect.ltUV.y = tRect.lt.y / _vImageSize.y;

	return tRect;
}

TRect Animator2DEditorGUI::_GetRectFromPos(const ImVec2& _vPos1, const ImVec2& _vPos2, const ImVec2& _vImageSize)
{
	if (_vPos1.x < 0 || _vPos1.y < 0 || _vPos2.x < 0 || _vPos2.y < 0 ||
		_vPos1.x >_vImageSize.x || _vPos1.y > _vImageSize.y ||
		_vPos2.x >_vImageSize.x || _vPos1.y > _vImageSize.y) {
		//assert(nullptr);
	}

	ImVec2 vLTPos = _vPos1;
	ImVec2 vRBPos = _vPos2;

	if (vRBPos.x < vLTPos.x && vRBPos.y < vLTPos.y) {
		ImVec2 vTempPos = vLTPos;
		vLTPos = vRBPos;
		vRBPos = vTempPos;
	}

	TRect tRect = {};
	tRect.rb = vRBPos;
	tRect.lt = vLTPos;
	tRect.rt = ImVec2(tRect.rb.x, tRect.rb.y);
	tRect.lb = ImVec2(tRect.lt.x, tRect.rb.y);

	if (_vImageSize.x > 0.f && _vImageSize.y > 0.f) {
		tRect.rbUV.x = tRect.rb.x / _vImageSize.x;
		tRect.rbUV.y = tRect.rb.y / _vImageSize.y;

		tRect.rtUV.x = tRect.rt.x / _vImageSize.x;
		tRect.rtUV.y = tRect.rt.y / _vImageSize.y;

		tRect.lbUV.x = tRect.lb.x / _vImageSize.x;
		tRect.lbUV.y = tRect.lb.y / _vImageSize.y;

		tRect.ltUV.x = tRect.lt.x / _vImageSize.x;
		tRect.ltUV.y = tRect.lt.y / _vImageSize.y;
	}
	
	return tRect;
}


bool Animator2DEditorGUI::_FixedTextureEleToUVInList(int _iIdx, TTextureBtnInfo& _tTexBtnInfo_out, const ImVec2& _vImageSize)
{
	if (m_queResultTexList.size() <= _iIdx || 0 > _iIdx) {
		assert(nullptr);
		return false;
	}

	// uv 값을 구해보자고
	if (nullptr != m_queResultTexList[_iIdx].tAnim2DDesc.pAtlas.Get()) {

		// PreView Texture가 있어야 될 듯
		TAnimationFrame tAnimFrame = {};
		tAnimFrame.fDuration = m_queResultTexList[_iIdx].tAnim2DDesc.fDuration;
		tAnimFrame.vFrameSizeUV = m_queResultTexList[_iIdx].tAnim2DDesc.vFrameSize / m_queResultTexList[_iIdx].tAnim2DDesc.pAtlas->GetResolution();
		tAnimFrame.vBaseSizeUV = m_queResultTexList[_iIdx].tAnim2DDesc.vBaseSize / m_queResultTexList[_iIdx].tAnim2DDesc.pAtlas->GetResolution();
		tAnimFrame.fDuration = m_queResultTexList[_iIdx].tAnim2DDesc.fDuration;
		tAnimFrame.vLeftTopUV = m_queResultTexList[_iIdx].tAnim2DDesc.vLeftTop / m_queResultTexList[_iIdx].tAnim2DDesc.pAtlas->GetResolution();
		tAnimFrame.vOffsetPosUV = m_queResultTexList[_iIdx].tAnim2DDesc.vOffsetPos / m_queResultTexList[_iIdx].tAnim2DDesc.pAtlas->GetResolution();

		Vector2 vFinalLT_Vec = tAnimFrame.vLeftTopUV - ((tAnimFrame.vBaseSizeUV * 0.5f) - (tAnimFrame.vFrameSizeUV * 0.5f)) - tAnimFrame.vOffsetPosUV;

		_tTexBtnInfo_out.uv_min = ImVec2(vFinalLT_Vec.x, vFinalLT_Vec.y);
		_tTexBtnInfo_out.uv_max = ImVec2(vFinalLT_Vec.x + tAnimFrame.vBaseSizeUV.x, vFinalLT_Vec.y + tAnimFrame.vBaseSizeUV.y);
		_tTexBtnInfo_out.vImageSize = _vImageSize;
		return true;
	}
	return false;
}

void Animator2DEditorGUI::_DrawCross(const ImVec2& _vLTPos, const ImVec2& _vRBPos, ImDrawList* _draw_list)
{
	ImVec2 p3 = ImVec2(_vLTPos.x + _vRBPos.x, _vLTPos.y + _vRBPos.y);
	ImVec2 midle = p3;
	ImVec2 wp1 = ImVec2(_vLTPos.x, midle.y);
	ImVec2 wp2 = ImVec2(_vRBPos.x, midle.y);

	ImVec2 hp1 = ImVec2(midle.x, _vLTPos.y);
	ImVec2 hp2 = ImVec2(midle.x, _vRBPos.y);

	_draw_list->AddLine(wp1, wp2, IM_COL32(255, 0, 0, 255));
	_draw_list->AddLine(hp1, hp2, IM_COL32(0, 0, 255, 255));
}

void Animator2DEditorGUI::_ShowSplitedSprite()
{
	ImGui::Separator();
	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	{
		TSelectTexInfo tSelTexInfo = {};
		ImVec2 vSpriteSize = ImVec2(100.f, 100.f);

		// scroll test
		static int track_item = 50;
		static bool enable_track = true;
		static bool enable_extra_decorations = false;
		static float scroll_to_off_px = 0.0f;
		static float scroll_to_pos_px = 200.0f;
		ImGui::Checkbox("Track", &enable_track);

		bool scroll_to_off = ImGui::Button("Scroll Offset");
		ImGui::SameLine(140); scroll_to_off |= ImGui::DragFloat("##off", &scroll_to_off_px, 1.00f, 0, FLT_MAX, "+%.0f px");

		bool scroll_to_pos = ImGui::Button("Scroll To Pos");
		ImGui::SameLine(140); scroll_to_pos |= ImGui::DragFloat("##pos", &scroll_to_pos_px, 1.00f, -10, FLT_MAX, "X/Y = %.0f px");

		ImGuiStyle& style = ImGui::GetStyle();
		float child_w = (ImGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
		if (child_w < 1.0f)
			child_w = 1.0f;


		float child_height = vSpriteSize.y + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
		ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0);
		ImGuiID child_id = ImGui::GetID((void*)CImGuiManager::GetInstance()->GetWidgetID());
		bool child_is_visible = ImGui::BeginChild(child_id, ImVec2(-100, child_height), true, child_flags);

		//ImGui::BeginChild("scrolling", scrolling_child_size, true, ImGuiWindowFlags_HorizontalScrollbar);


		if (scroll_to_off)
			ImGui::SetScrollX(scroll_to_off_px);
		if (scroll_to_pos)
			ImGui::SetScrollFromPosX(ImGui::GetCursorStartPos().x + scroll_to_pos_px, 0.25f);
		ImVec2 scrolling_child_size = ImVec2(100.f, 100.f);

		if (child_is_visible) // Avoid calling SetScrollHereY when running with culled items
		{
			// 이미지를 넣기
			// Image Button 
			{
				// canvas setting 일단 이미지로 해본다.
				for (int i = 0; i < m_queResultTexList.size(); ++i) {
		
					ImGui::PushID(i);

					TTextureBtnInfo tTexBtnInfo = {};

					if (_FixedTextureEleToUVInList(i, tTexBtnInfo)) {
						if (ParamGUI::Render_TextureBtn("", m_queResultTexList[i].tAnim2DDesc.pAtlas.Get(), tTexBtnInfo)) {
							m_bShowModifyPanel = true;
							m_iSelectedIdx = i;
						}
					}

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
						ImGui::SetDragDropPayload("SwapImage##Animator2DEditor", &i, sizeof(int));
						ImGui::Text("SWAP %d", i);
						ImGui::EndDragDropSource();
					}

					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SwapImage##Animator2DEditor")) {
							IM_ASSERT(payload->DataSize == sizeof(int));
							int payload_i = *(const int*)payload->Data;

							// 교체
							TSelectTexInfo tTemp = m_queResultTexList[i];
							m_queResultTexList[i] = m_queResultTexList[payload_i];
							m_queResultTexList[payload_i] = tTemp;
						}

						ImGui::EndDragDropTarget();
					}

					ImGui::PopID();

					ImGui::SameLine();
				}
			}
		}
		float scroll_x = ImGui::GetScrollX();
		float scroll_max_x = ImGui::GetScrollMaxX();
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::Text("%s\n%.0f/%.0f", "Left", scroll_x, scroll_max_x);
		ImGui::Spacing();
		// scroll test end


		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		if (m_iSelectedIdx >= 0)
			ImGui::Text("==[selected %d image]==", m_iSelectedIdx);
	}
	ImGui::PopID();
}

void Animator2DEditorGUI::_ModifyAniationPanel()
{
	// 애니메이션 만들기 위한 설정 넣기

	if (0 <= m_iSelectedIdx) {
		if (ImGui::Button("Delete##Animation2DEditor Image Delete")) {
			_DrawImageDeleteInResultQue((DWORD_PTR)m_iSelectedIdx);
			m_iSelectedIdx = -1;
		}
	}

	if (0 <= m_iSelectedIdx) {
		// name (공통)
		if (ImGui::InputText("name##animator2D", m_nameBuff, 255)) {
			// 이름 바꾸기
			tstring strName;
			StringToTString(m_nameBuff, strName);
			for (int i = 0; i < m_queResultTexList.size(); ++i)
				m_queResultTexList[i].tAnim2DDesc.strName = strName;
		}


		int iFrameCount = m_queResultTexList.size();
		for (int i = 0; i < m_queResultTexList.size(); ++i)
			m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.iFrameCount = iFrameCount;

		// frame count (공통)
		ImGui::Text("animation frame count %d", iFrameCount);

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		// duration
		ImGui::InputFloat("speed##animator2D", &m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.fDuration);
		ImGui::SameLine();
		if (ImGui::Button("common apply##animator2Dspeed")) {
			for (int i = 0; i < m_queResultTexList.size(); ++i) {
				m_queResultTexList[i].tAnim2DDesc.fDuration = m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.fDuration;
			}
		}

		// base size
		ImGui::InputFloat2("base size##animator2D", (float*)&m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vBaseSize);
		ImGui::SameLine();
		if (ImGui::Button("common apply##animator2DbaseSize")) {
			for (int i = 0; i < m_queResultTexList.size(); ++i) {
				m_queResultTexList[i].tAnim2DDesc.vBaseSize = m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vBaseSize;
			}
		}

		// frame size
		ImGui::InputFloat2("frame size##aniator2D", (float*)&m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vFrameSize);

		// left top
		ImGui::InputFloat2("left top##animator2D", (float*)&m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vLeftTop);

		// offset position

		ImGui::DragFloat2("Offset##anmiator2D", (float*)&m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vOffsetPos, 0.1f, FLOAT_MIN, FLOAT_MIN, "%.2f", ImGuiSliderFlags_None);
	}
	ImGui::Separator();

	// 현재 수정하고있는 텍스쳐를 그리기
	if (0 <= m_iSelectedIdx) {
		TTextureBtnInfo tTexBtnInfo = {};

		if (_FixedTextureEleToUVInList(m_iSelectedIdx, tTexBtnInfo, ImVec2(150.f, 150.f))) {
			ParamGUI::Render_TextureBtn("", m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.pAtlas.Get(), tTexBtnInfo);
		}
	}

	if (ImGui::Button("Create##animation2d"))
		_OnCreateAnimation();

	if (ImGui::Button("Save##animation2d"))
		_OnSaveAnimation();
}

void Animator2DEditorGUI::_CanvasGridSliceMode() {
	if (m_pLoadedAtlasTexture)
		vAtlasSize = m_pLoadedAtlasTexture->GetResolution();

	static int grids[2] = { 1.f, 1.f };

	// Texture canvas
	if (ImGui::BeginChild("texture canvas panel", ImVec2(500, 500), true)) {
		//  opeiton


		canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
		canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available

		canvas_sz = ImVec2(vAtlasSize.x, vAtlasSize.y);
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

		// ---------------- grid 요소
		const float GRID_STEP = 0.f;
		float fGridStepWidth = vAtlasSize.x / grids[0];
		float fGridStepHeight = vAtlasSize.y / grids[1];
		// ---------------------------

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		// Draw border and background color
		ImGuiIO& io = ImGui::GetIO();
		draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(20, 20, 20, 255));
		draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(10, 10, 10, 255));


		// image drawing
		ImVec2 can0 = ImVec2(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);
		ImVec2 can1 = ImVec2(canvas_p1.x + scrolling.x, canvas_p1.y + scrolling.y);
		_Canvas_DrawImage(draw_list, m_pLoadedAtlasTexture, can0, can1);

		// canvs에서 
		// 마우스 왼쪽과 오른쪽 버튼을 canvas에서 사용 할 것임.
		ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		const bool is_hovered = ImGui::IsItemHovered(); // Hovered
		const bool is_active = ImGui::IsItemActive();   // Held
		const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
		const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);


		static bool bSelectArea = false;
		static TSelectTexInfo tSelTexInfo = {};
		// 왼쪽 마우스를 클릭했을 때 클릭했으면
		if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			// 클릭한 곳의 위치값을 인덱스값으로 변환하여 위치값을 알아내기.
			tSelTexInfo = _FindMinorTexIdx(mouse_pos_in_canvas, canvas_sz, grids[0], grids[1], canvas_sz);

			// 자잘한것들 초기화
			tSelTexInfo.tAnim2DDesc = {};
			tSelTexInfo.tAnim2DDesc.pAtlas = m_pLoadedAtlasTexture;
			tSelTexInfo.tAnim2DDesc.vBaseSize = Vector2(fGridStepWidth, fGridStepHeight);
			tSelTexInfo.tAnim2DDesc.vFrameSize = Vector2(fGridStepWidth, fGridStepHeight);
			tSelTexInfo.tAnim2DDesc.vLeftTop = Vector2(tSelTexInfo.rect.lt.x, tSelTexInfo.rect.lt.y);
			bSelectArea = true;
		}

		if (is_hovered && ImGui::IsMouseDragging(ImGuiButtonFlags_MouseButtonMiddle)) {

		}

		// 마우스를 뗐을 때
		if (is_hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			if (bSelectArea)
				m_queMinorTexList.push_back(tSelTexInfo);
			bSelectArea = false;
		}

		//// Add first and second point
		//if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		//{
		//	points.push_back(mouse_pos_in_canvas);
		//	points.push_back(mouse_pos_in_canvas);
		//	adding_line = true;
		//}
		//if (adding_line)
		//{
		//	points.back() = mouse_pos_in_canvas;
		//	if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
		//		adding_line = false;
		//}

		// Pan (we use a zero mouse threshold when there's no context menu)
		// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.

		const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
		{
			scrolling.x += io.MouseDelta.x;
			scrolling.y += io.MouseDelta.y;
		}

		// Context menu (under default mouse threshold)
		/*ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
		if (opt_enable_context_menu && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
			ImGui::OpenPopupOnItemClick("context");*/

		if (ImGui::BeginPopup("context"))
		{
			/*if (adding_line)
				points.resize(points.size() - 2);
			adding_line = false;
			if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
			if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
			*/
			ImGui::EndPopup();
		}

		// Draw grid + all lines in the canvas
		draw_list->PushClipRect(canvas_p0, canvas_p1, true);

		if (opt_enable_grid)
		{
			_DrawAtlasOutline(canvas_p0, canvas_p1, fGridStepWidth, fGridStepHeight);
		}
		/*for (int n = 0; n < points.Size; n += 2)
			draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);*/
		draw_list->PopClipRect();

		// 선택 영역 rect draw하기
		_DrawSelectedRect();

		ImGui::EndChild();
	}

	ImGui::SameLine();
	// Right Panel
	if (ImGui::BeginChild("Atlas Texture view", ImVec2(300, 500))) { // Leave room for 1 line below us
		{
			ImGui::InputInt2("Divide Grid##animator2d", grids);

			if (ImGui::Button("Clear##Animator2DSelectImage")) {
				m_queMinorTexList.clear();
				m_iSelectedIdx = -1;
			}

			// 선택된 애니메이션 텍스쳐를 추가하기
			if (ImGui::Button("Add Animations##animation2DEditor")) {
				m_queResultTexList.clear();
				// 순서대로 텍스쳐를 보여 주기
				for (int i = 0; i < m_queMinorTexList.size(); ++i)
					m_queResultTexList.push_back(m_queMinorTexList[i]);
			}
		}
		ImGui::EndChild();
	}
}

void Animator2DEditorGUI::_CanvasSliceMode()
{
	Vector2 vAtlasSize = Vector2(500.f, 500.f);
	if (m_pLoadedAtlasTexture)
		vAtlasSize = m_pLoadedAtlasTexture->GetResolution();

	// Texture canvas
	if (ImGui::BeginChild("texture canvas panel", ImVec2(500, 500), true)) {

		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available

		canvas_sz = ImVec2(vAtlasSize.x, vAtlasSize.y);
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		// Draw border and background color
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(20, 20, 20, 255));
		draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(10, 10, 10, 255));


		// Drawing Cross
		_DrawCross(canvas_p0, canvas_p1, draw_list);

		// image drawing
		ImVec2 can0 = ImVec2(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);
		ImVec2 can1 = ImVec2(canvas_p1.x + scrolling.x, canvas_p1.y + scrolling.y);
		_Canvas_DrawImage(draw_list, m_pLoadedAtlasTexture, can0, can1);

		// canvs에서 
		// 마우스 왼쪽과 오른쪽 버튼을 canvas에서 사용 할 것임.
		ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		const bool is_hovered = ImGui::IsItemHovered(); // Hovered
		const bool is_active = ImGui::IsItemActive();   // Held
		const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
		const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
		const ImVec2 mouse_pos = io.MousePos;
		ImGui::Text("mouse pos : %.2f %.2f", mouse_pos.x, mouse_pos.y);

		
		//// Add first and second point
		//if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		//{
		//	points.push_back(mouse_pos_in_canvas);
		//	points.push_back(mouse_pos_in_canvas);
		//	adding_line = true;
		//}
		//if (adding_line)
		//{
		//	points.back() = mouse_pos_in_canvas;
		//	if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
		//		adding_line = false;
		//}

		// Pan (we use a zero mouse threshold when there's no context menu)
		// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.

		// Context menu (under default mouse threshold)
		/*ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
		if (opt_enable_context_menu && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
			ImGui::OpenPopupOnItemClick("context");*/

		if (ImGui::BeginPopup("context"))
		{
			/*if (adding_line)
				points.resize(points.size() - 2);
			adding_line = false;
			if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
			if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
			*/
			ImGui::EndPopup();
		}

		static TSelectTexInfo tSelTexInfo = {};
		static ImVec2 vFirstClickedPosInCanavas = {};
		static ImVec2 vFirstMousePos = {};

		static bool bDrag = false;
		// 왼쪽 마우스를 클릭했을 때 클릭했으면
		if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			// 클릭한 곳의 위치값을 인덱스값으로 변환하여 위치값을 알아내기.
			vFirstClickedPosInCanavas = mouse_pos_in_canvas;
			vFirstMousePos = ImVec2(mouse_pos.x + scrolling.x, mouse_pos.y + scrolling.y);
			// 자잘한것들 초기화
			bDrag = true;
		}

		ImVec2 vFClickedPos = {};
		ImVec2 vSClickedPos = {};
		// 마우스 드래그를 했을 때.
		if (bDrag) {
			//if (is_hovered && ImGui::IsMouseDragging(ImGuiButtonFlags_MouseButtonLeft)) {
				// 선을 보여준다.
			vFClickedPos = ImVec2(vFirstMousePos.x - scrolling.x, vFirstMousePos.y - scrolling.y);
			vSClickedPos = ImVec2(mouse_pos.x, mouse_pos.y);

			draw_list->AddRect(vFClickedPos, vSClickedPos, IM_COL32(0, 255, 0, 255), 0.f, 0, 2.f);
		}

		// 마우스를 뗐을 때
		if (is_hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			TRect tRect = _GetRectFromPos(vFirstClickedPosInCanavas, mouse_pos_in_canvas, ImVec2(vAtlasSize.x, vAtlasSize.y));
			tSelTexInfo.rect = tRect;

			tSelTexInfo.tAnim2DDesc = {};
			tSelTexInfo.tAnim2DDesc.pAtlas = m_pLoadedAtlasTexture;


			Vector2 vRectSize = Vector2(tRect.rb.x - tRect.lt.x, tRect.rb.y - tRect.lt.y);
			tSelTexInfo.tAnim2DDesc.vBaseSize = vRectSize;
			tSelTexInfo.tAnim2DDesc.vFrameSize = vRectSize;
			tSelTexInfo.tAnim2DDesc.vLeftTop = Vector2(tSelTexInfo.rect.lt.x, tSelTexInfo.rect.lt.y);
			m_queMinorTexList.push_back(tSelTexInfo);
			bDrag = false;
		}


		const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;

		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
		{
			if (false == bDrag) {
				scrolling.x += io.MouseDelta.x;
				scrolling.y += io.MouseDelta.y;
			}
		}



		// Draw grid + all lines in the canvas
		draw_list->PushClipRect(canvas_p0, canvas_p1, true);

		if (opt_enable_grid)
		{
			float fGridStepWidth = vAtlasSize.x / 1.f;
			float fGridStepHeight = vAtlasSize.y / 1.f;
			_DrawAtlasOutline(canvas_p0, canvas_p1, fGridStepWidth, fGridStepHeight);

		}
		/*for (int n = 0; n < points.Size; n += 2)
			draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);*/
		draw_list->PopClipRect();

		// 선택 영역 rect draw하기
		_DrawSelectedRect();

		ImGui::EndChild();
	}

	ImGui::SameLine();
	// Right Panel
	if (ImGui::BeginChild("Atlas Texture view", ImVec2(300, 500))) { // Leave room for 1 line below us
		{
			if (ImGui::Button("Clear##Animator2DSelectImage")) {
				m_queMinorTexList.clear();
				m_iSelectedIdx = -1;
			}

			// 선택된 애니메이션 텍스쳐를 추가하기
			if (ImGui::Button("Add Animations##animation2DEditor")) {
				m_queResultTexList.clear();
				// 순서대로 텍스쳐를 보여 주기
				for (int i = 0; i < m_queMinorTexList.size(); ++i)
					m_queResultTexList.push_back(m_queMinorTexList[i]);
			}
		}
		ImGui::EndChild();
	}
}

void Animator2DEditorGUI::_Canvas_DrawImage(ImDrawList* _pDrawList, CTexture* _pTexture, const ImVec2& _vLTPos, const ImVec2& _vRBPos)
{
	ImTextureID tex_id = 0;
	if (m_pLoadedAtlasTexture) {
		tex_id = (ImTextureID)(m_pLoadedAtlasTexture->GetSRV().Get());
		_pDrawList->AddImage(tex_id, _vLTPos, _vRBPos);
	}
}



void Animator2DEditorGUI::_OnCreateAnimation()
{
	if (!m_pAnimator2D) {
		assert(nullptr);
		return;
	}

	vector<TAnimation2DDesc> vecAnim2DDesc;
	for (int i = 0; i < m_queResultTexList.size(); ++i)
		vecAnim2DDesc.push_back(m_queResultTexList[i].tAnim2DDesc);
	m_pAnimator2D->CreateAnimation(vecAnim2DDesc);
}

void Animator2DEditorGUI::_OnSaveAnimation()
{
	// TODO (Jang) : 저장하기	
	// 원도우 창을 띄워서 저장하게 하자.
}

void Animator2DEditorGUI::_DrawAtlasOutline(const ImVec2& _canvas_p0, const ImVec2& _canvas_p1, float _fGridStepWidth, float _fGridStepHeight)
{
	for (float x = fmodf(scrolling.x, _fGridStepWidth); x < canvas_sz.x; x += _fGridStepWidth)
		draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, _canvas_p1.y), IM_COL32(25, 200, 25, 180));
	for (float y = fmodf(scrolling.y, _fGridStepHeight); y < canvas_sz.y; y += _fGridStepHeight)
		draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(_canvas_p1.x, canvas_p0.y + y), IM_COL32(25, 200, 25, 180));
}

void Animator2DEditorGUI::_DrawSelectedRect()
{
	ImU32 iRectFillColor = IM_COL32(50, 50, 50, 100);
	ImU32 iTextColor = IM_COL32(240, 50, 50, 240);
	ImU32 iRectOutlineColor = IM_COL32(0, 255, 0, 255);

	// 선택한 영역 draw하기
	for (int i = 0; i < m_queMinorTexList.size(); ++i) {
		TSelectTexInfo info = m_queMinorTexList[i];
		// 선택 영역 rectfill로 채우기

		TRect tRect = m_queMinorTexList[i].rect;

		tRect.lt = ImVec2(tRect.lt.x + canvas_p0.x, tRect.lt.y + canvas_p0.y);
		tRect.rb = ImVec2(tRect.rb.x + canvas_p0.x, tRect.rb.y + canvas_p0.y);

		ImVec2 vResultLT = ImVec2(tRect.lt.x + scrolling.x, tRect.lt.y + scrolling.y);
		ImVec2 vResultRB = ImVec2(tRect.rb.x + scrolling.x, tRect.rb.y + scrolling.y);

		draw_list->AddRectFilled(
			vResultLT, vResultRB, iRectFillColor);
		ImVec2 vMiddle = ImVec2((tRect.lt.x + tRect.rb.x) * 0.5f, (tRect.lt.y + tRect.rb.y) * 0.5f);
		vMiddle = ImVec2(vMiddle.x + scrolling.x, vMiddle.y + scrolling.y);
		string strNum = std::to_string(i);
		ImFont* font_current = ImGui::GetFont();
		draw_list->AddText(font_current, 20, vMiddle, iTextColor, strNum.c_str(), 0);

		draw_list->AddRect(vResultLT, vResultRB, iRectOutlineColor);
	}
}

void Animator2DEditorGUI::_DrawImageDeleteInResultQue(DWORD_PTR idx)
{
	int i = (int)idx;
	auto iter = m_queResultTexList.begin();
	int iterIdx = 0;
	for (iter; iter != m_queResultTexList.end();) {
		if (iterIdx == i) {
			break;
		}
		else {
			++iter;
		}
		++iterIdx;
	}
	m_queResultTexList.erase(iter);
}
