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
	m_nameBuff{},
	m_queResultTexList{},
	m_tSelectedTexInfo{},
	m_iSelectedIdx{ -1 }

{
}


void Animator2DEditorGUI::_Clear()
{
	SetTargetObject(nullptr);
	m_tSelectedTexInfo = {};
	m_iSelectedIdx = -1;
	ZeroMemory(m_nameBuff, sizeof(m_nameBuff));
	m_queMinorTexList.clear();
	m_queResultTexList.clear();
}

Animator2DEditorGUI::~Animator2DEditorGUI()
{
}

void Animator2DEditorGUI::_OnSelectAtlasTexture()
{
	// ����Ʈ �並 �����ش�.
	ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
	assert(pListViewGUI);

	vector<tstring> vecNames;
	CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecNames);
	pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Texture));
	pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&Animator2DEditorGUI::_SelectTexture);
	pListViewGUI->SetActive(true);
}

void Animator2DEditorGUI::_RenderAltasTexture()
{
	// ��ü ����� �����ִ� �ؽ���
	
	TTextureInfo tTexInfo = {};
	tTexInfo.vImageSize = ImVec2(400.f, 400.f);

	ParamGUI::Render_Texture("atlas texture##animator2D", m_tSelectedTexInfo.tAnim2DDesc.pAtlas.Get(), nullptr, nullptr, false, tTexInfo);
}


void Animator2DEditorGUI::Init()
{
	SetName(STR_GUI_Animator2DEditor);
}

void Animator2DEditorGUI::Update()
{
	// â�� �ϳ� ����
	if (!m_bGUIOpen) {
		_Clear();
		return;
	}

	if (!m_pTargetObject)
		return;

	CAnimator2D* pAnimator2D = m_pTargetObject->Animator2D();
	if (!pAnimator2D) {
		assert(nullptr);
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(STR_GUI_Animator2DEditor, &m_bGUIOpen))
	{
		if (ImGui::BeginTabBar("##Animator2DTabBar"))
		{
			if (ImGui::BeginTabItem("Modify##Animator2D")) {
				_TextureModifyPanel(pAnimator2D);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Canvas##Animator2D")) {
				_CanvasDrawPanel(pAnimator2D);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}


inline void Animator2DEditorGUI::SetAtlasTexture(CTexture* _pTex)
{
	m_tSelectedTexInfo.tAnim2DDesc.pAtlas = _pTex;
}

inline CTexture* Animator2DEditorGUI::GetAtlasTexture()
{
	return m_tSelectedTexInfo.tAnim2DDesc.pAtlas.Get();
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

void Animator2DEditorGUI::_CreateAnimation(CAnimator2D* _pAnimator2D)
{
	if (m_tSelectedTexInfo.tAnim2DDesc.strName.empty())
		return;
	CAnimation2D* pAnim = _pAnimator2D->FindAnimation(m_tSelectedTexInfo.tAnim2DDesc.strName);
	if (!pAnim)
		_pAnimator2D->CreateAnimation(m_tSelectedTexInfo.tAnim2DDesc);
}

void Animator2DEditorGUI::_SaveAnimation(CAnimator2D* _pAnimator2D)
{
	CAnimation2D* pAnim2D = _pAnimator2D->FindAnimation(m_tSelectedTexInfo.tAnim2DDesc.strName);
	tstring strAnimFileName = m_tSelectedTexInfo.tAnim2DDesc.strName + STR_EXTENSION_Anim;
	pAnim2D->Save(STR_DIR_PATH_Anim, strAnimFileName);
}

void Animator2DEditorGUI::_DeleteAnimation(CAnimator2D* _pAnimator2D)
{
	// animation list �����ֱ�
	vector<tstring> vectNames;
	GetTargetObject()->Animator2D()->GetAnimationNamesFromList(vectNames);
	vector<string> vecNames_o;
	TStringToStringVec(vectNames, vecNames_o);
	vector<char> vecComboName;
	ParamGUI::Make_ComboBoxList(vecNames_o, vecComboName);
	static int iCurAnimIdx = 0;
	tstring strAnimName = vectNames[iCurAnimIdx];
	if (ParamGUI::Render_ComboBox("Animation List##animator2D", &iCurAnimIdx, vecComboName)) {
		tstring strAnimName = vectNames[iCurAnimIdx];
	}

	if (ImGui::Button("Delete animation")) {
		// Error ����
		/*if (_T("") != strAnimName)
			_pAnimator2D->DeleteAnimation(strAnimName);*/
	}
}

void Animator2DEditorGUI::_SelectTexture(DWORD_PTR _pStr, DWORD_PTR _NONE)
{
	const char* pStrKey = (const char*)_pStr;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pImageTexture = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pImageTexture);
	m_tSelectedTexInfo.tAnim2DDesc.pAtlas = pImageTexture;
}

void Animator2DEditorGUI::_CanvasDrawPanel(CAnimator2D* _pAnimator2D)
{
	Vector2 vAtlasSize = Vector2(500.f, 500.f);
	if (m_tSelectedTexInfo.tAnim2DDesc.pAtlas.Get())
		vAtlasSize = m_tSelectedTexInfo.tAnim2DDesc.pAtlas->GetDimension();

	static int grids[2] = { 1.f, 1.f };

	// Left Panel
	{
		{
			ImGui::Text("Left Panel");
			ImGui::BeginChild("canvas panel", ImVec2(vAtlasSize.x, vAtlasSize.y), true, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

			static ImVector<ImVec2> points;
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
			ImVec2 canvas_sz; //= ImGui::GetContentRegionAvail();   // Resize canvas to what's available

			canvas_sz = ImVec2(vAtlasSize.x, vAtlasSize.y);
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

			const float GRID_STEP = 0.f;
			float fGridStepWidth = canvas_sz.x / grids[0];
			float fGridStepHeight = canvas_sz.y / grids[1];

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			// Draw border and background color
			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(255, 50, 255, 255));
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));
			draw_list->AddText(canvas_p0, IM_COL32(0, 0, 0, 255), "Hello WOrld");


			// Test code

#pragma region Draw Middlef line
			ImVec2 p3 = ImVec2(canvas_p0.x + canvas_p1.x, canvas_p0.y + canvas_p1.y);
			ImVec2 midle = p3;
			ImVec2 wp1 = ImVec2(canvas_p0.x, midle.y);
			ImVec2 wp2 = ImVec2(canvas_p1.x, midle.y);

			ImVec2 hp1 = ImVec2(midle.x, canvas_p0.y);
			ImVec2 hp2 = ImVec2(midle.x, canvas_p1.y);

			draw_list->AddLine(wp1, wp2, IM_COL32(255, 0, 0, 255));
			draw_list->AddLine(hp1, hp2, IM_COL32(0, 0, 255, 255));
#pragma endregion

			ImTextureID tex_id = 0;
			if (m_tSelectedTexInfo.tAnim2DDesc.pAtlas.Get()) {
				tex_id = (ImTextureID)(m_tSelectedTexInfo.tAnim2DDesc.pAtlas->GetSRV().Get());
				draw_list->AddImage(tex_id, canvas_p0, canvas_p1);
			}

			// canvs���� 
			// This will catch our interactions
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

			static bool bSelectArea = false;
			static TSelectTexInfo tSelTexInfo = {};
			// ���� ���콺�� Ŭ������ �� Ŭ��������
			if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				// Ŭ���� ���� ��ġ���� �ε��������� ��ȯ�Ͽ� ��ġ���� �˾Ƴ���.
				tSelTexInfo = _FindMinorTexIdx(mouse_pos_in_canvas, canvas_sz, grids[0], grids[1], canvas_sz);

				tSelTexInfo.tAnim2DDesc = m_tSelectedTexInfo.tAnim2DDesc;

				// �����Ѱ͵� �ʱ�ȭ
				tSelTexInfo.tAnim2DDesc.vBaseSize = Vector2(fGridStepWidth, fGridStepHeight);
				tSelTexInfo.tAnim2DDesc.vFrameSize = Vector2(fGridStepWidth, fGridStepHeight);
				tSelTexInfo.tAnim2DDesc.vLeftTop = Vector2(tSelTexInfo.rect.lt.x, tSelTexInfo.rect.lt.y);
				bSelectArea = true;
			}

			// ���콺�� ���� ��
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

			/*const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
			if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
			{
				scrolling.x += io.MouseDelta.x;
				scrolling.y += io.MouseDelta.y;
			}*/

			// Context menu (under default mouse threshold)
			/*ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
			if (opt_enable_context_menu && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
				ImGui::OpenPopupOnItemClick("context");*/

			if (ImGui::BeginPopup("context"))
			{
				if (adding_line)
					points.resize(points.size() - 2);
				adding_line = false;
				if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
				if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
				ImGui::EndPopup();
			}

			// Draw grid + all lines in the canvas
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);

			if (opt_enable_grid)
			{
				for (float x = fmodf(scrolling.x, fGridStepWidth); x < canvas_sz.x; x += fGridStepWidth)
					draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(25, 200, 25, 180));
				for (float y = fmodf(scrolling.y, fGridStepHeight); y < canvas_sz.y; y += fGridStepHeight)
					draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(25, 200, 25, 180));
			}
			/*for (int n = 0; n < points.Size; n += 2)
				draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);*/
			draw_list->PopClipRect();

			// ������ ���� draw�ϱ�
			for (int i = 0; i < m_queMinorTexList.size(); ++i) {
				TSelectTexInfo info = m_queMinorTexList[i];
				// ���� ���� rectfill�� ä���
				
				TRect tRect = m_queMinorTexList[i].rect;

				tRect.lt = ImVec2(tRect.lt.x + canvas_p0.x, tRect.lt.y + canvas_p0.y);
				tRect.rb = ImVec2(tRect.rb.x + canvas_p0.x, tRect.rb.y + canvas_p0.y);
				draw_list->AddRectFilled(tRect.lt, tRect.rb, IM_COL32(50, 50, 50, 100));
				ImVec2 vMiddle = ImVec2((tRect.lt.x + tRect.rb.x) * 0.5f, (tRect.lt.y + tRect.rb.y) * 0.5f);
				string strNum = std::to_string(i);
				ImFont* font_current = ImGui::GetFont();
				draw_list->AddText(font_current, 20, vMiddle, IM_COL32(20, 240, 20, 240), strNum.c_str(),0);
			}
		}
		
		ImGui::EndChild();
	}
	ImGui::SameLine();

	// Right Panel
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("Atlas Texture view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

		{
			ImGui::InputInt2("Divide Grid##animator2d", grids);


			if (ImGui::Button("Selected Image Clear##Animator2DSelect")) {
				m_queMinorTexList.clear();
				m_iSelectedIdx = -1;
			}

			// ���õ� �ִϸ��̼� �ؽ��ĸ� �߰��ϱ�
			if (ImGui::Button("Add Animation Texture")) {

				m_queResultTexList.clear();
				// ������� �ؽ��ĸ� ���� �ֱ�
				for (int i = 0; i < m_queMinorTexList.size(); ++i)
					m_queResultTexList.push_back(m_queMinorTexList[i]);
			}
		}

		ImGui::EndChild();


		ImGui::BeginChild("Texture setting", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

		ImGui::Text("texture setting");
		ImGui::EndChild();
		ImGui::EndGroup();
	}


	
	// Bottom
	bool bImageBtnSelect = false;
	{
		TSelectTexInfo tSelTexInfo = {};
		// canvas setting �ϴ� �̹����� �غ���.
		for (int i = 0; i < m_queResultTexList.size(); ++i) {
			string strNum = std::to_string(i) + "##animation2DImageBtn";
			TTextureBtnInfo tTexInfo = {};
			tTexInfo.bg_col = ImVec4(1.0f, 0.f, 1.0f, 1.0f);
			tTexInfo.uv_min = m_queResultTexList[i].rect.ltUV;
			tTexInfo.uv_max = m_queResultTexList[i].rect.rbUV;
			tTexInfo.vImageSize = ImVec2(100.f, 100.f);
			ImGui::SameLine();
			if (ParamGUI::Render_TextureBtn(strNum.c_str(), m_queResultTexList[i].tAnim2DDesc.pAtlas.Get(), tTexInfo)) {
				m_iSelectedIdx = i;
				bImageBtnSelect = true;
				tSelTexInfo = m_queResultTexList[i];
			}
		}
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		if(m_iSelectedIdx >= 0 )
			ImGui::Text("==[selected %d image]==", m_iSelectedIdx);

		// ���� ������
		if (bImageBtnSelect) {
			m_tSelectedTexInfo = tSelTexInfo;
		}
	}

	{
		// �ִϸ��̼� ����� ���� ���� �ֱ�
		if (m_iSelectedIdx >= 0) {

			// name (����)
			if (ImGui::InputText("name##animator2D", m_nameBuff, 255)) {
				// �̸� �ٲٱ�
				tstring strName;
				StringToTString(m_nameBuff, strName);
				for (int i = 0; i < m_queResultTexList.size(); ++i)
					m_queResultTexList[i].tAnim2DDesc.strName = strName;
			}


			int iFrameCount = m_queResultTexList.size();
			for (int i = 0; i < m_queResultTexList.size(); ++i)
				m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.iFrameCount = iFrameCount;

			// frame count (����)
			ImGui::Text("animation frame count %d##animator2D", iFrameCount);


			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			// duration
			ImGui::InputFloat("speed##animator2D", &m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.fDuration);

			// base size
			ImGui::InputFloat2("base size##animator2D", (float*)&m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vBaseSize);

			// frame size
			ImGui::InputFloat2("frame size##aniator2D", (float*)&m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vFrameSize);

			// left top
			ImGui::InputFloat2("left top##animator2D", (float*)&m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vLeftTop);

			// offset position
			ImGui::InputFloat2("Offset##anmiator2D", (float*)&m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vOffsetPos);


			// uv ���� ���غ��ڰ�
			if (_pAnimator2D && nullptr != &m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.pAtlas) {

				// PreView Texture�� �־�� �� ��
				TTextureInfo tTexInfo = {};

				TAnimationFrame tAnimFrame = {};
				tAnimFrame.fDuration = m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.fDuration;
				tAnimFrame.vFrameSizeUV = m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vFrameSize / m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.pAtlas->GetDimension();
				tAnimFrame.vBaseSizeUV = m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vBaseSize / m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.pAtlas->GetDimension();
				tAnimFrame.fDuration = m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.fDuration;
				tAnimFrame.vLeftTopUV = m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vLeftTop / m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.pAtlas->GetDimension();
				tAnimFrame.vOffsetPosUV = m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.vOffsetPos / m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.pAtlas->GetDimension();

				/*	CAnimation2D* pCurAnim = pAnimator2D->GetCurAnimation();
					const TAnimationFrame& tAnimFrame = pCurAnim->GetCurAnimationFrame();*/

				Vector2 vFinalLT_Vec = tAnimFrame.vLeftTopUV - ((tAnimFrame.vBaseSizeUV * 0.5f) - (tAnimFrame.vFrameSizeUV * 0.5f)) - tAnimFrame.vOffsetPosUV;

				tTexInfo.uv_min = ImVec2(vFinalLT_Vec.x, vFinalLT_Vec.y);
				tTexInfo.uv_max = ImVec2(vFinalLT_Vec.x + tAnimFrame.vBaseSizeUV.x, vFinalLT_Vec.y + tAnimFrame.vBaseSizeUV.y);

				ParamGUI::Render_Texture("PreView", m_queResultTexList[m_iSelectedIdx].tAnim2DDesc.pAtlas.Get(), nullptr, nullptr, false, tTexInfo);
			}
		}

		
		if (ImGui::Button("Create##animation2d")) {

		}
	}
}


void Animator2DEditorGUI::_TextureModifyPanel(CAnimator2D* _pAnimator2D)
{
	// ��Ʋ�� �ؽ��� ���� ��ư
	if (ImGui::Button("Select Texture##AtlasTexture"))
		_OnSelectAtlasTexture();

	// Left Panel
	{
		ImGui::Text("Left Panel");
		ImGui::BeginChild("left pane", ImVec2(400.f, 400.f), true);

		// Rendering AltasTexture
		_RenderAltasTexture();

		ImGui::EndChild();
		ImGui::BeginChild("left2 pane", ImVec2(250, 0), true);

		ImGui::EndChild();
	}
	ImGui::SameLine();

	// Right Panel
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("Atlas Texture view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		ImGui::Text("atlas texture view ");
		ImGui::EndChild();

		ImGui::BeginChild("Texture setting", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		ImGui::Text("texture setting");
		ImGui::EndChild();
		ImGui::EndGroup();
	}

	// ��Ʋ�� �ؽ��� ��ü ������
	Vector2 vTexSize = {};
	if (m_tSelectedTexInfo.tAnim2DDesc.pAtlas.Get())
		vTexSize = m_tSelectedTexInfo.tAnim2DDesc.pAtlas.Get()->GetDimension();
	ImGui::Text("Atlas Texture Size [%.2f, %.2f]", vTexSize.x, vTexSize.y);

	// ����ϱ�
	ImGui::Text("--Calculate Animation Size--");


	static int arrMaxFrameColRowCnt[2] = {};
	ImGui::InputInt2("Max Frame Col, Row Count", arrMaxFrameColRowCnt);


	// ������ ������ ����ϱ�
	static int vFrameSize[2] = {};
	vFrameSize[0] = arrMaxFrameColRowCnt[0] == 0 ? 0 : (int)(vTexSize.x / arrMaxFrameColRowCnt[0]);
	vFrameSize[1] = arrMaxFrameColRowCnt[1] == 0 ? 0 : (int)(vTexSize.y / arrMaxFrameColRowCnt[1]);
	ImGui::Text("Frame Size [%d, %d] ", vFrameSize[0], vFrameSize[1]);
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	// �»�� ��ǥ ����ϱ�
	ImGui::Text("--Get Left Top Pixel--");
	static int arrColRow[2];
	ImGui::InputInt2("Input Col, Row", arrColRow);
	int arrLeftTop[2] = {};
	arrLeftTop[0] = (arrColRow[0] - 1) * vFrameSize[0];
	arrLeftTop[1] = (arrColRow[1] - 1) * vFrameSize[1];
	ImGui::Text("Found Left Top pixel pos : [%d, %d] ", arrLeftTop[0], arrLeftTop[1]);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	//// �ִϸ��̼� ����� ���� ���� �ֱ�

	//// name 
	//ImGui::InputText("name##animator2D", m_nameBuff, 255);
	//StringToTString(m_nameBuff, m_tDescAnim.strName);

	//// duration
	//ImGui::InputFloat("speed##animator2D", &m_tDescAnim.fDuration);

	//// frame count
	//ImGui::InputInt("animation count##animator2D", &m_tDescAnim.iFrameCount);

	//// texture
	//m_tDescAnim.pAtlas = m_tDescAnim.pAtlas.Get();

	//// base size
	//ImGui::InputFloat2("base size##animator2D", (float*)&m_tDescAnim.vBaseSize);

	//// frame size
	//ImGui::InputFloat2("frame size##aniator2D", (float*)&m_tDescAnim.vFrameSize);

	//// left top
	//ImGui::InputFloat2("left top##animator2D", (float*)&m_tDescAnim.vLeftTop);

	//// offset position
	//ImGui::InputFloat2("Offset##anmiator2D", (float*)&m_tDescAnim.vOffsetPos);


	//// uv ���� ���غ��ڰ�
	//if (_pAnimator2D && nullptr != m_tDescAnim.pAtlas) {

	//	// PreView Texture�� �־�� �� ��
	//	TTextureInfo tTexInfo = {};

	//	TAnimationFrame tAnimFrame = {};
	//	tAnimFrame.fDuration = m_tDescAnim.fDuration;
	//	tAnimFrame.vFrameSizeUV = m_tDescAnim.vFrameSize / m_tDescAnim.pAtlas->GetDimension();
	//	tAnimFrame.vBaseSizeUV = m_tDescAnim.vBaseSize / m_tDescAnim.pAtlas->GetDimension();
	//	tAnimFrame.fDuration = m_tDescAnim.fDuration;
	//	tAnimFrame.vLeftTopUV = m_tDescAnim.vLeftTop / m_tDescAnim.pAtlas->GetDimension();
	//	tAnimFrame.vOffsetPosUV = m_tDescAnim.vOffsetPos / m_tDescAnim.pAtlas->GetDimension();

	//	/*	CAnimation2D* pCurAnim = pAnimator2D->GetCurAnimation();
	//		const TAnimationFrame& tAnimFrame = pCurAnim->GetCurAnimationFrame();*/

	//	Vector2 vFinalLT_Vec = tAnimFrame.vLeftTopUV - ((tAnimFrame.vBaseSizeUV * 0.5f) - (tAnimFrame.vFrameSizeUV * 0.5f)) - tAnimFrame.vOffsetPosUV;

	//	tTexInfo.uv_min = ImVec2(vFinalLT_Vec.x, vFinalLT_Vec.y);
	//	tTexInfo.uv_max = ImVec2(vFinalLT_Vec.x + tAnimFrame.vBaseSizeUV.x, vFinalLT_Vec.y + tAnimFrame.vBaseSizeUV.y);

	//	ParamGUI::Render_Texture("PreView", m_tDescAnim.pAtlas.Get(), nullptr, nullptr, false, tTexInfo);
	//}

	// animation create button
	if (ImGui::Button("Create Animation##animator2D")) {
		_CreateAnimation(_pAnimator2D);
	}

	// save animation in GameObject
	if (ImGui::Button("Save Animation")) {
		_CreateAnimation(_pAnimator2D);
		_SaveAnimation(_pAnimator2D);
	}

	// load animation
	if (ImGui::Button("Load Animation")) {
		// TODO (Jang) : �ִϸ��̼� �ε�
	}


	// �ִϸ��̼� ����Ʈ ��Ÿ����

	// ������� �ִϸ��̼� �����ϱ�
	_DeleteAnimation(_pAnimator2D);
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