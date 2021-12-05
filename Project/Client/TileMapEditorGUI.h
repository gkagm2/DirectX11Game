#pragma once
#include "GUI.h"
class CGameObject;
class CTileMap;
class TileMapEditorGUI : public GUI
{
private:
	CGameObject* m_pTargetObject;
	CTileMap* m_pTileMap;
	CTexture* m_pAtlasTileTex;


	int m_arrFaceTileCnt[2];
	int m_arrAtlasTileCnt[2];

private:
	int m_iSelectedTileIdx; // 선택한 타일의 인덱스

private:
	// Tile Canvas를 위한 멤버 변수

public:
	virtual void Init();
	virtual void Update() override;

public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObject = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObject; }

private:
	void _RenderCanvas();

private:
	void _SelectTileMap(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _Clear();
public:
	TileMapEditorGUI();
	virtual ~TileMapEditorGUI() override;
};