#pragma once
#include "GUI.h"
class CGameObject;
class CTileMap;

#define IMGUI_COLOR_GREEN IM_COL32(10, 240, 10, 128)
#define IMGUI_COLOR_RED IM_COL32(240, 10, 10, 128)

class TileMapEditorGUI : public GUI
{
private:
	CGameObject* m_pTargetObject;
	CTileMap* m_pTileMap;
	CTexture* m_pAtlasTileTex;


	int m_arrFaceTileCnt[2];
	int m_vAtlasTilePixelSize[2];

private:
	int m_iSelectedTileIdx; // ������ Ÿ���� �ε���

private:
	int m_iGridColor;


private:
	// Tile Canvas�� ���� ��� ����

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