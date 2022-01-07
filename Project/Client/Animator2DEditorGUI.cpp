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
#include <Engine\CTimeManager.h>
#include "DebugGUI.h"
#include <Engine\CCore.h>

Animator2DEditorGUI::Animator2DEditorGUI() :
	m_eTap(E_Tap::Editor),
	m_pTargetObject(nullptr),
	m_pAnimator2D(nullptr),
	m_pLoadedAtlasTexture(nullptr),
	m_nameBuff{},
	m_queResultTexList{},
	m_iSelectedIdx{ -1 },
	m_bolorRectColor{ IM_COL_WHITE },
	m_eEditMode{ E_EditMode::SliceSprite_Grid },
	m_bShowModifyPanel(false),

	// canvas properties
	vAtlasSize{ 500.f, 500.f },
	points{},
	opt_enable_context_menu{ true },
	canvas_p0{},
	canvas_sz{},
	canvas_p1{},
	origin{},
	draw_list{ nullptr },
	fCanvasScale{ 1.f },

	m_fPreviewAnimSpeed(1.f),
	m_isChangeCanvasScale(false),

	// Fixed Animation Properties
	m_vecFrame{},
	m_fAccTime{ 0.f },
	m_iCurFrameIdx{ 0 },
	m_iAnimFinish{ false },
	m_iMode{ 0 }
{
}

void Animator2DEditorGUI::SetActive(bool _bIsActive)
{
	GUI::SetActive(_bIsActive);
	m_eTap = E_Tap::Editor;
}

void Animator2DEditorGUI::_Clear()
{
	m_pTargetObject = nullptr;
	m_pAnimator2D = nullptr;
	m_iSelectedIdx = -1;
	ZeroMemory(m_nameBuff, sizeof(m_nameBuff));
	m_queMinorTexList.clear();
	m_queResultTexList.clear();

	m_fPreviewAnimSpeed = 1.f;

	// FIxed Panel의 프로퍼티
	m_vecFrame.clear();
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
	pListViewGUI->SetPreViewTexRender(true);
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

	ImGui::SetNextWindowSize(ImVec2(550, 550), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(STR_GUI_Animator2DEditor, &m_bGUIOpen))
	{
		if (ImGui::BeginTabBar("##Animator2DTabBar"))
		{
			if (E_Tap::Editor == m_eTap)
			{
				if (ImGui::BeginTabItem("Editor##Animator2DTap")) {
					_CanvasDrawPanel();
					ImGui::EndTabItem();
				}
			}
			else if (E_Tap::Fixed == m_eTap) {
				if (ImGui::BeginTabItem("Animation Fixed##Animator2DTap")) {
					_FixedAnimationPanel();
					ImGui::EndTabItem();
				}
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
	// Left Top Menu
	ImGui::Text("Texture Border");
	_CanvasTopPanel(); // Top Panel 

	_CanvasSliceMode();
	m_isChangeCanvasScale = false;
	if(ImGui::SliderFloat("Scale", &fCanvasScale, 0.1f, 25.f, "%.01f", 1.f))
		m_isChangeCanvasScale = true;

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

TRect Animator2DEditorGUI::_GetRectFromPos(const ImVec2& _vPos1, const ImVec2& _vPos2, const ImVec2& _vImageSize)
{
	/*if (_vPos1.x < 0 || _vPos1.y < 0 || _vPos2.x < 0 || _vPos2.y < 0 ||
		_vPos1.x >_vImageSize.x || _vPos1.y > _vImageSize.y ||
		_vPos2.x >_vImageSize.x || _vPos1.y > _vImageSize.y) {
		assert(nullptr);
	}*/

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
		static bool enable_extra_decorations = false;

		ImGuiStyle& style = ImGui::GetStyle();
		float child_w = (ImGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
		if (child_w < 1.0f)
			child_w = 1.0f;


		float child_height = vSpriteSize.y + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
		ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0);
		ImGuiID child_id = ImGui::GetID((void*)CImGuiManager::GetInstance()->GetWidgetID());
		bool child_is_visible = ImGui::BeginChild(child_id, ImVec2(-100, child_height), true, child_flags);

		//ImGui::BeginChild("scrolling", scrolling_child_size, true, ImGuiWindowFlags_HorizontalScrollbar);


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
		ImGui::EndChild();
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		if (m_iSelectedIdx >= 0)
			ImGui::Text("==[selected %d image]==", m_iSelectedIdx);
	}
	ImGui::PopID();
}

// TODO (Jang) : 리펙토링 하기
void Animator2DEditorGUI::_ModifyAniationPanel()
{
	if (0 <= m_iSelectedIdx) {
		if (ImGui::Button("Delete##Animation2DEditor Image Delete")) {
			_DrawImageDeleteInResultQue((DWORD_PTR)m_iSelectedIdx);
			m_iSelectedIdx = -1;
		}
	}

	if (0 <= m_iSelectedIdx) {
		// name (공통)
		if (ImGui::InputText("name##animator2D", m_nameBuff, 255)) {
			tstring strName;
			StringToTString(m_nameBuff, strName);
			for (int i = 0; i < m_queResultTexList.size(); ++i)
				m_queResultTexList[i].tAnim2DDesc.strName = strName;
		}


		int iFrameCount = (int)m_queResultTexList.size();
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

	// Animation Preview (애니메이션 미리보기)
	static float m_fAccTime = 0.f;
	static int m_iCurFrameIdx = 0;
	static bool m_iAnimFinish = false;

	static int m_iMode;
	ImGui::SliderInt("Mode", &m_iMode, 0, 1);

	if (m_iMode == 0) {
		ImGui::SliderInt("Frame Idx", &m_iCurFrameIdx, 0, max(0, (int)(m_queResultTexList.size() - 1)));
	}
	else  if (m_iMode == 1) {
		m_fAccTime += DT;
		if (m_iCurFrameIdx < 0) {
			if (0 <= m_queResultTexList.size())
				m_iCurFrameIdx = 0;
		}
		else if (m_iCurFrameIdx >= m_queResultTexList.size()) {
			if (m_queResultTexList.size() == 0) // 존재하지 않으면
				m_iCurFrameIdx = -1; // -1로 바꿈.
			else if (m_iCurFrameIdx > 0) m_iCurFrameIdx = 0; // 넘치면 0부터 다시 시작
		}
		else {
			if (m_iAnimFinish) {
				m_iAnimFinish = false;
				m_iCurFrameIdx = 0;
			}
			while (m_fAccTime > m_queResultTexList[m_iCurFrameIdx].tAnim2DDesc.fDuration) {
				// 누적시간이 현재 프레임 유지시간을 초과한 양을 구함
				m_fAccTime = m_fAccTime - m_queResultTexList[m_iCurFrameIdx].tAnim2DDesc.fDuration;
				++m_iCurFrameIdx;

				// 최대 프레임에 도달하게 되면, 애니메이션 재생상태를 완료로 둠.
				if (m_iCurFrameIdx == m_queResultTexList.size()) {
					--m_iCurFrameIdx;


					m_iAnimFinish = true;
					break;
				}
			}
		}
		m_iCurFrameIdx = max(0, m_iCurFrameIdx);
	}

	CTexture* pPreviewTex = nullptr;
	TTextureInfo tTextureInfo = {};

	if (m_queResultTexList.size() && 0 <= m_iCurFrameIdx) {
		pPreviewTex = m_queResultTexList[m_iCurFrameIdx].tAnim2DDesc.pAtlas.Get();
		tTextureInfo.uv_min = m_queResultTexList[m_iCurFrameIdx].rect.ltUV;
		tTextureInfo.uv_max = m_queResultTexList[m_iCurFrameIdx].rect.rbUV;
	}
	ImGui::Text("curIdx %d", m_iCurFrameIdx);

	ParamGUI::Render_Texture("", pPreviewTex, nullptr, nullptr, false, tTextureInfo);


	ImGui::Spacing();

	if (ImGui::Button("Create##animation2d"))
		_OnSaveAnimation();
}

void Animator2DEditorGUI::_FixedAnimationPanel()
{
	if (!m_pAnimator2D)
		return;

	CAnimation2D* pCurAnim = m_pAnimator2D->GetCurAnimation();
	if (!pCurAnim)
		return;

	vector<TAnimationFrame>& vecAnimFrm = pCurAnim->GetAnimationFrame();
	

	// frame 조절
	ImGui::SliderInt("Frame## FixedAnimator2D", &m_iCurFrameIdx, 0, max(0, vecAnimFrm.size() - 1));

	CTexture* pCurTex = pCurAnim->GetTextures()[m_iCurFrameIdx].Get();

	// PreView Texture
	TTextureInfo tTexInfo = {};

	// 복사된것중 하나를 가져옴
	TAnimationFrame& curFrame = m_vecFrame[m_iCurFrameIdx];

	Vector2 vFinalLT_Vec = curFrame.vLeftTopUV - curFrame.vOffsetPosUV;
	Vector2 vFinalRB_Vec = curFrame.vLeftTopUV + curFrame.vFrameSizeUV - curFrame.vOffsetPosUV;

	tTexInfo.uv_min = ImVec2(vFinalLT_Vec.x, vFinalLT_Vec.y);
	tTexInfo.uv_max = ImVec2(vFinalRB_Vec.x, vFinalRB_Vec.y);

	ParamGUI::Render_Texture("PreView", pCurTex, nullptr, nullptr, false, tTexInfo);

	string strAnimName;
	TStringToString(pCurAnim->GetName(), strAnimName);
	ImGui::Text("cur animation name : %s", strAnimName.c_str());

	static char szName[255] = {};
	ImGui::Text("Change animation name");
	ImGui::InputText("name##animationFixedName", szName, 255);


	// frame count
	int iFrameCnt = (int)pCurAnim->GetAnimationFrame().size();
	ImGui::Text("animation frame count %d", iFrameCnt);
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	// duration
	float fDuration = curFrame.fDuration;
	if (ImGui::InputFloat("speed##animator2DFixed", &fDuration))
		curFrame.fDuration = fDuration;
	ImGui::SameLine();
	// 공통 적용
	if (ImGui::Button("common apply##animator2DspeedFixed")) {
		for (int i = 0; i < (int)vecAnimFrm.size(); ++i) {
			m_vecFrame[i].fDuration = fDuration;
		}
	}

	// base size
	Vector2 vBaseSize = curFrame.vBaseSizeUV * pCurTex->GetResolution();
	if (ImGui::InputFloat2("base size##animator2DFixed", (float*)&vBaseSize))
		curFrame.vBaseSizeUV = vBaseSize / pCurTex->GetResolution();
	ImGui::SameLine();
	// 공통 적용
	if (ImGui::Button("common apply##animator2DbaseSizeFixed")) {
		for (int i = 0; i < (int)vecAnimFrm.size(); ++i) {
			m_vecFrame[i].vBaseSizeUV = vBaseSize / pCurTex->GetResolution();
		}
	}

	// frame size
	Vector2 vFrameSize = curFrame.vFrameSizeUV * pCurTex->GetResolution();
	if (ImGui::InputFloat2("frame size UV##aniator2DFixed", (float*)&vFrameSize))
		curFrame.vFrameSizeUV = vFrameSize / pCurTex->GetResolution();

	// left top
	Vector2 vLeftTop = curFrame.vLeftTopUV * pCurTex->GetResolution();
	if (ImGui::InputFloat2("left top UV##animator2DFixed", (float*)&vLeftTop))
		curFrame.vLeftTopUV = vLeftTop / pCurTex->GetResolution();


	// offset position
	Vector2 vOffsetPos = curFrame.vOffsetPosUV * pCurTex->GetResolution();
	if (ImGui::DragFloat2("Offset UV##anmiator2DFixed", (float*)&vOffsetPos, 0.1f, FLOAT_MIN, FLOAT_MIN, "%.2f", ImGuiSliderFlags_None))
		curFrame.vOffsetPosUV = vOffsetPos / pCurTex->GetResolution();

	if (ImGui::Button("Apply##animator2DFixed")) {
		// 현재 애니메이션으로 복사한다.
		for (size_t i = 0; i < m_vecFrame.size(); ++i) {
			vecAnimFrm[i] = m_vecFrame[i];
		}
	}
}

void Animator2DEditorGUI::InitFixedAnimationPanel()
{
	if (!m_pAnimator2D)
		return;
	CAnimation2D* pCurAnim = m_pAnimator2D->GetCurAnimation();
	if (!pCurAnim)
		return;
	pCurAnim = m_pAnimator2D->GetCurAnimation();
	vector<TAnimationFrame>& vecAnimFrm = pCurAnim->GetAnimationFrame();
	size_t iSize = vecAnimFrm.size();
	m_vecFrame.clear();
	m_vecFrame.resize(iSize);
	for (size_t i = 0; i < iSize; ++i)
		m_vecFrame[i] = vecAnimFrm[i];
}

void Animator2DEditorGUI::_CanvasSliceMode()
{
	if (m_pLoadedAtlasTexture)
		vAtlasSize = m_pLoadedAtlasTexture->GetResolution();
	ImVec2 vAtlasSizeInCanvas = ImVec2(vAtlasSize.x * fCanvasScale, vAtlasSize.y * fCanvasScale);

	static int grids[2] = { 1.f,1.f };

	ImVec2 vImageSize = ImVec2(vAtlasSize.x, vAtlasSize.y);

	canvas_p0 = ImGui::GetCursorScreenPos();
	//canvas_sz = ImGui::GetContentRegionAvail();
	canvas_sz = ImVec2(500.f, 500.f);
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	draw_list = ImGui::GetWindowDrawList();
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(20, 20, 20, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(10, 10, 10, 255));

	// image drawing
	ImVec2 vImageLT_inCanvas = (canvas_p0 + scrolling);
	ImVec2 vImageRB_inCanvas = (canvas_p0 + scrolling) + vAtlasSizeInCanvas;

	_Canvas_DrawImage(draw_list, m_pLoadedAtlasTexture, vImageLT_inCanvas, vImageRB_inCanvas);

	// canvs에서 
	// 마우스 왼쪽과 오른쪽 버튼을 canvas에서 사용 할 것임.
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	origin = ImVec2(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
	mouse_pos_in_canvas /= fCanvasScale;
	ImVec2 mouse_pos(io.MousePos.x, io.MousePos.y);

	static bool bDrag = false;
	static TSelectTexInfo tSelTexInfo = {};
	static ImVec2 vFirstClickedPosInCanavas = {};
	static ImVec2 vFirstClickPos = {};

	// 왼쪽 마우스를 클릭했을 때 클릭했으면
	if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		vFirstClickedPosInCanavas = mouse_pos_in_canvas;
		vFirstClickPos = mouse_pos_in_canvas;
		bDrag = true;
	}
	//DBug->Debug("MousePos : %.2f, %.2f", (mouse_pos_in_canvas.x), (mouse_pos_in_canvas.y));

	// 마우스를 뗐을 때
	if (is_hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		if (E_EditMode::SliceSprite == m_eEditMode) {
			ImVec2 vCurPos = mouse_pos_in_canvas;
			ImVec2 vLTPos = vFirstClickPos;
			ImVec2 vRBPos = vCurPos;

			// drag 영역을 구한다.
			if (vRBPos.x < vLTPos.x && vRBPos.y < vLTPos.y) {
				ImVec2 vTempPos = vLTPos;
				vLTPos = vRBPos;
				vRBPos = vTempPos;
			}

			// Min Max값 설정
			TRect tRect = {};
			tRect.rb = vRBPos;
			tRect.lt = vLTPos;
			tRect.rt = ImVec2(tRect.rb.x, tRect.rb.y);
			tRect.lb = ImVec2(tRect.lt.x, tRect.rb.y);

			if (vAtlasSize.x > 0.f && vAtlasSize.y > 0.f) {
				tRect.rbUV.x = tRect.rb.x / vAtlasSize.x;
				tRect.rbUV.y = tRect.rb.y / vAtlasSize.y;

				tRect.rtUV.x = tRect.rt.x / vAtlasSize.x;
				tRect.rtUV.y = tRect.rt.y / vAtlasSize.y;

				tRect.lbUV.x = tRect.lb.x / vAtlasSize.x;
				tRect.lbUV.y = tRect.lb.y / vAtlasSize.y;

				tRect.ltUV.x = tRect.lt.x / vAtlasSize.x;
				tRect.ltUV.y = tRect.lt.y / vAtlasSize.y;
			}

			tSelTexInfo.tAnim2DDesc = {};
			tSelTexInfo.tAnim2DDesc.pAtlas = m_pLoadedAtlasTexture;
			tSelTexInfo.rect = tRect;

			// 자잘한것들 초기화
			tSelTexInfo.tAnim2DDesc.vBaseSize = Vector2(tRect.rb.x - tRect.lt.x, tRect.rb.y - tRect.lt.y);
			tSelTexInfo.tAnim2DDesc.vFrameSize = Vector2(tRect.rb.x - tRect.lt.x, tRect.rb.y - tRect.lt.y);
			tSelTexInfo.tAnim2DDesc.vLeftTop = Vector2(tSelTexInfo.rect.lt.x, tSelTexInfo.rect.lt.y);

			m_queMinorTexList.push_back(tSelTexInfo);
		}
		bDrag = false;
	}
	// 왼쪽 마우스를 클릭했을 때 클릭했으면
	else if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		if (E_EditMode::SliceSprite_Grid == m_eEditMode) {
			Vector2 vBaseSize{};
			Vector2 vFrameSize{};

			// 클릭한 곳의 위치값을 인덱스값으로 변환하여 위치값을 알아내기.
			
			float fGridStepWidth = vImageSize.x / grids[0];
			float fGridStepHeight = vImageSize.y / grids[1];

			int selCol = (int)(mouse_pos_in_canvas.x / fGridStepWidth);
			int selRow = (int)(mouse_pos_in_canvas.y / fGridStepHeight);

			tSelTexInfo.col = selCol;
			tSelTexInfo.row = selRow;

			// Min Max 값 설정
			TRect tRect = {};
			tRect.rb = ImVec2(float((selCol + 1) * fGridStepWidth), float((selRow + 1) * fGridStepHeight));
			tRect.rt = ImVec2(tRect.rb.x, tRect.rb.y - fGridStepHeight);
			tRect.lb = ImVec2(tRect.rb.x - fGridStepWidth, tRect.rb.y);
			tRect.lt = ImVec2(tRect.lb.x, tRect.rt.y);

			tRect.rbUV.x = tRect.rb.x / vImageSize.x;
			tRect.rbUV.y = tRect.rb.y / vImageSize.y;

			tRect.rtUV.x = tRect.rt.x / vImageSize.x;
			tRect.rtUV.y = tRect.rt.y / vImageSize.y;

			tRect.lbUV.x = tRect.lb.x / vImageSize.x;
			tRect.lbUV.y = tRect.lb.y / vImageSize.y;

			tRect.ltUV.x = tRect.lt.x / vImageSize.x;
			tRect.ltUV.y = tRect.lt.y / vImageSize.y;
			tSelTexInfo.rect = tRect;

			vBaseSize = Vector2(vAtlasSize.x / grids[0], vAtlasSize.y / grids[1]);
			vFrameSize = Vector2(vAtlasSize.x / grids[0], vAtlasSize.y / grids[1]);


			tSelTexInfo.tAnim2DDesc = {};
			tSelTexInfo.tAnim2DDesc.pAtlas = m_pLoadedAtlasTexture;
			// 자잘한것들 초기화
			tSelTexInfo.tAnim2DDesc.vBaseSize = vBaseSize;
			tSelTexInfo.tAnim2DDesc.vFrameSize = vFrameSize;
			tSelTexInfo.tAnim2DDesc.vLeftTop = Vector2(tSelTexInfo.rect.lt.x, tSelTexInfo.rect.lt.y);

			m_queMinorTexList.push_back(tSelTexInfo);
		}
	}
	
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		if (E_EditMode::SliceSprite == m_eEditMode) {
			if (false == bDrag) {
				scrolling.x += io.MouseDelta.x;
				scrolling.y += io.MouseDelta.y;
			}
		}
		else {
			scrolling.x += io.MouseDelta.x;
			scrolling.y += io.MouseDelta.y;
		}
	}

	float fGridStepWidth = vAtlasSize.x / grids[0] * fCanvasScale;
	float fGridStepHeight = vAtlasSize.y / grids[1] * fCanvasScale;
	_DrawAtlasOutline(canvas_p0, canvas_p1, fGridStepWidth, fGridStepHeight);

	_DrawSelectedRect();
	draw_list->PopClipRect();

	{
		if (E_EditMode::SliceSprite_Grid == m_eEditMode) {
			if (ImGui::InputInt2("Divide Grid##animator2d", grids)) {
				if (grids[0] == 0) grids[0] = 1;
				if (grids[1] == 0) grids[1] = 1;
			}
		}
			
		else
			grids[0] = grids[1] = 1;

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
	
	OPENFILENAME ofn;
	wchar_t strMaxPath[MAX_PATH] = L"";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInstance()->GetWndHandle();
	ofn.lpstrFilter = _T(".anim");
	ofn.lpstrFile = strMaxPath;
	ofn.nMaxFile = MAX_PATH;

	if (0 != GetSaveFileName(&ofn)) {
		tstring animName = m_queResultTexList[0].tAnim2DDesc.strName;
		tstring animFileName = m_queResultTexList[0].tAnim2DDesc.strName + STR_EXTENSION_Anim;

		_OnCreateAnimation();
		CAnimation2D* pAnim2D = m_pAnimator2D->FindAnimation(animName);
		pAnim2D->Save(STR_DIR_PATH_Anim, animFileName);
	}
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

		tRect.lt = ImVec2(tRect.lt.x, tRect.lt.y);
		tRect.rb = ImVec2(tRect.rb.x, tRect.rb.y);

		ImVec2 vResultLT = ImVec2(tRect.lt.x, tRect.lt.y);
		ImVec2 vResultRB = ImVec2(tRect.rb.x, tRect.rb.y);
		vResultLT = origin + ( vResultLT ) * fCanvasScale;
		vResultRB = origin + ( vResultRB) * fCanvasScale;

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