#pragma once
#include "GUI.h"
#include <Engine/CAnimator2D.h>
#include "ParamGUI.h"
#include <Engine\CAnimation2D.h>

#define IM_COL_WHITE IM_COL32(255,255,255,255)

class CGameObject;
class CCanvasGUI;

struct TSelectTexInfo {
	int col;
	int row;
	TRect rect;
	TAnimation2DDesc tAnim2DDesc;
};

class Animator2DEditorGUI : public GUI
{
public:
	enum class E_Tap {
		Editor,
		Fixed,
	};

private:
	CGameObject* m_pTargetObject;
	CAnimator2D* m_pAnimator2D;

	// temp
	char m_nameBuff[255];

	CTexture* m_pLoadedAtlasTexture;

private:
	vector<TSelectTexInfo> m_queMinorTexList; // 추가 할 텍스쳐 리스트

	int m_iSelectedIdx; // 추가 할 것들중에 선택 한 인덱스 번호

	vector<TSelectTexInfo> m_queResultTexList; // 수정까지 마친 텍스쳐 리스트

private:
	ImU32 m_bolorRectColor;

	enum class E_EditMode {
		Normal,
		SliceSprite_Grid,
		SliceSprite,
		TrimSlice,
	};

	E_EditMode m_eEditMode;
	bool m_bShowModifyPanel;

	float m_fPreviewAnimSpeed;
	E_Tap m_eTap;

public:
	virtual void Init();
	virtual void Update() override;

public:
	void SetTap(E_Tap _eTap) { m_eTap = _eTap; }

public:
	void SetTargetObject(CGameObject* _pTargetObj) { 
		m_pTargetObject = _pTargetObj; 
		if(m_pTargetObject->Animator2D())
			m_pAnimator2D = m_pTargetObject->Animator2D();
	}
	CGameObject* GetTargetObject() { return m_pTargetObject; }

	TSelectTexInfo& GetSelectedTexInfo() { return m_queResultTexList[m_iSelectedIdx]; }

	inline void SetAtlasTexture(CTexture* _pTex);
	inline CTexture* GetAtlasTexture();

private:
	void _SetAtlasTexture(DWORD_PTR _dw1, DWORD_PTR _dw);
	void _SelectLoadedAtlasTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

private:
	// function
	void _OnCreateAnimation();
	void _OnSaveAnimation();

	// minor panel
	void _OnLoadAtlasTexture(); // 아틀


private: // canvas properties 
	ImVector<ImVec2> points;
	ImVec2 scrolling; ///(0.0f, 0.0f);
	bool opt_enable_context_menu = true;
	ImVec2 canvas_p0;
	ImVec2 canvas_sz;
	ImVec2 canvas_p1;
	ImDrawList* draw_list;
	ImVec2 origin;
	Vector2 vAtlasSize;
	float fCanvasScale;
	bool m_isChangeCanvasScale;

private:
	// Panel
	void _CanvasTopPanel();
	void _CanvasDrawPanel();

	// Canvas Panel
	void _CanvasSliceMode();

	// 캔바스 내에 공통된 기능
	void _Canvas_DrawImage(ImDrawList* _pDrawList, CTexture* _pTexture, const ImVec2& _vLTPos, const ImVec2& _vRBPos);

	void _ShowSplitedSprite();
	void _ModifyAniationPanel();
	bool _FixedTextureEleToUVInList(int _iIdx, TTextureBtnInfo& _tTexBtnInfo_out, const ImVec2& _vImageSize = ImVec2(100.f, 100.f));

	// etc
	TRect _GetMinMaxRectFromColRow(int _gridStepWidth, int _gridStepHeight, int iCol, int iRow, const ImVec2& _vImageSize);
	TRect _GetRectFromPos(const ImVec2& _vPos1, const ImVec2& _vPos2, const ImVec2& _vImageSize = ImVec2(0.f, 0.f));


	void _DrawCross(const ImVec2& _vLTPos, const ImVec2& _vRBPos, ImDrawList* _draw_list);
	void _DrawAtlasOutline(const ImVec2& _canvas_p0, const ImVec2& _canvas_p1, float _fGridStepWidth, float _fGridStepHeight);

	void _DrawSelectedRect(); // canvas에서 선택된 영역 그리기
	void _DrawImageDeleteInResultQue(DWORD_PTR idx);


/////////////////// 애니메이션 수정 패널
private: // Fixed Animation Properties;
	vector<TAnimationFrame> m_vecFrame;
	// Animation Preview (애니메이션 미리보기
	float	m_fAccTime;
	int		m_iCurFrameIdx;
	bool	m_iAnimFinish;
	int		m_iMode;
	

private:
	void _FixedAnimationPanel();
public:
	void InitFixedAnimationPanel();
	
///////////////////


private:
	void _Clear();

	public:
		virtual void SetActive(bool _bIsActive) override;

public:
	Animator2DEditorGUI();
	virtual ~Animator2DEditorGUI() override;
};