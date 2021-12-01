#pragma once
#include "GUI.h"
#include <Engine/CAnimator2D.h>
class CGameObject;

struct TSelectTexInfo {
	int col;
	int row;
};

struct TRect {
	ImVec2 lt;
	ImVec2 rt;
	ImVec2 lb;
	ImVec2 rb;
};

class Animator2DEditorGUI : public GUI
{
	CGameObject* m_pTargetObject;
	TAnimation2DDesc m_tDescAnim;

	// temp
	char m_nameBuff[255];

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

	// Panel
	void _TextureModifyPanel(CAnimator2D* _pAnimator2D);

	// minor panel
	void _OnSelectAtlasTexture();
	void _RenderAltasTexture();

private:
	// Panel
	void _CanvasDrawPanel();

	TSelectTexInfo _FindMinorTexIdx(ImVec2 _mousPos, ImVec2 _canvasSize, int iCol, int iRow);
	TRect _GetMinMaxRectFromColRow(int _gridStepWidth, int _gridStepHeight, int iCol, int iRow);
	

private:
	void _Clear();

public:
	Animator2DEditorGUI();
	virtual ~Animator2DEditorGUI() override;
};