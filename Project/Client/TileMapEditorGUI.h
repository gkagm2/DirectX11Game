#pragma once
#include "GUI.h"
class CGameObject;
class CTileMap;

#define IMGUI_COLOR_GREEN IM_COL32(10, 240, 10, 128)
#define IMGUI_COLOR_RED IM_COL32(240, 10, 10, 128)

enum class E_VisitedState { not_visited, visited }; // 최적화에 사용됨

class TileMapEditorGUI : public GUI
{
private:
	CGameObject* m_pTargetObject;
	CTileMap* m_pTileMap;
	CTexture* m_pAtlasTileTex;


	int m_arrFaceTileCnt[2];
	int m_vAtlasTilePixelSize[2];

	int m_iBrushSize;
	bool m_bDeleteMode;

private:
	int m_iSelectedTileIdx; // 선택한 타일의 인덱스

private:
	int m_iGridColor;
	


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
	bool _IsTileSelectedInCanvas() { return m_iSelectedTileIdx < 0 ? false : true; }

private:
	void _SelectTileMap(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _Clear();

	void _OptimizeCollisionArea(); // 충돌영역 타일 최적화하기
	void GetEndIdxOfRectArea(int** _grid, int _startX, int _startY, int& _endX, int& _endY);

public:
	TileMapEditorGUI();
	virtual ~TileMapEditorGUI() override;
};