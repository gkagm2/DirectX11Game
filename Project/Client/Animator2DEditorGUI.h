#pragma once
#include "GUI.h"
#include <Engine/CAnimator2D.h>
class CGameObject;
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
	void _CreateAnimation(CAnimator2D* _pAnimator2D);
	void _SaveAnimation(CAnimator2D* _pAnimator2D);
	void _DeleteAnimation(CAnimator2D* _pAnimator2D);

	void _Clear();
public:
	Animator2DEditorGUI();
	virtual ~Animator2DEditorGUI() override;
};