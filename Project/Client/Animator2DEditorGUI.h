#pragma once
#include "GUI.h"
#include <Engine/CAnimator2D.h>

#define IM_COL_WHITE IM_COL32(255,255,255,255)

class CGameObject;
class CCanvasGUI;
class CAnimator2DCanvasGUI;

struct TRect {
	ImVec2 lt;
	ImVec2 ltUV;
	ImVec2 rt;
	ImVec2 rtUV;
	ImVec2 lb;
	ImVec2 lbUV;
	ImVec2 rb;
	ImVec2 rbUV;
};

struct TSelectTexInfo {
	int col;
	int row;
	TRect rect;
	TAnimation2DDesc tAnim2DDesc;
};

class Animator2DEditorGUI : public GUI
{
private:
	CGameObject* m_pTargetObject;

	// temp
	char m_nameBuff[255];

private:
	vector<TSelectTexInfo> m_queMinorTexList; // �߰� �� �ؽ��� ����Ʈ

	TSelectTexInfo m_tSelectedTexInfo; // ������ �ؽ��� (������ ��Ʋ�� �ؽ���)
	int m_iSelectedIdx; // �߰� �� �͵��߿� ���� �� �ε��� ��ȣ

	vector<TSelectTexInfo> m_queResultTexList; // �������� ��ģ �ؽ��� ����Ʈ

	CAnimator2DCanvasGUI* m_pCanvasGUI;

private:
	ImU32 m_bolorRectColor;

	enum class E_EditMode {
		Normal,
		SliceSprite_Grid,
		SliceSprite,
		TrimSlice,
	};

	E_EditMode m_eEditMode;


public:
	virtual void Init();
	virtual void Update() override;

public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObject = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObject; }

	inline void SetAtlasTexture(CTexture* _pTex);
	inline CTexture* GetAtlasTexture();

private:
	void _SetAtlasTexture(DWORD_PTR _dw1, DWORD_PTR _dw);
	void _SelectTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);


private:
	// function
	void _CreateAnimation(CAnimator2D* _pAnimator2D);
	void _SaveAnimation(CAnimator2D* _pAnimator2D);
	void _DeleteAnimation(CAnimator2D* _pAnimator2D);

	// minor panel
	void _OnLoadAtlasTexture(); // ��Ʋ
	void _RenderAltasTexture();

private:
	// Panel
	void _CanvasDrawPanel(CAnimator2D* _pAnimator2D);

	void _CanvasTopPanel();





	TSelectTexInfo _FindMinorTexIdx(ImVec2 _mousPos, ImVec2 _canvasSize, int iCol, int iRow, const ImVec2& _vImageSize);
	TRect _GetMinMaxRectFromColRow(int _gridStepWidth, int _gridStepHeight, int iCol, int iRow, const ImVec2& _vImageSize);

	void _DrawingFixedTextureList(CAnimator2D* _pAnimator2D, int _iIdx);

	void _DrawCross(const ImVec2& _vLTPos, const ImVec2& _vRBPos, ImDrawList* _draw_list);

	void _CanvasGridSliceMode();


private:
	void _Clear();

public:
	Animator2DEditorGUI();
	virtual ~Animator2DEditorGUI() override;
};