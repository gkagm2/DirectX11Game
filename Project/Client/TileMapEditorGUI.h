#pragma once
#include "GUI.h"
class CGameObject;
class CTileMap;
class TileMapEditorGUI : public GUI
{
private:
	CGameObject* m_pTargetObject;
	CTileMap* m_pTileMap;


	int m_arrFaceTileCnt[2];
	int m_arrAtlasTileCnt[2];
	float fTileBtnSize;

public:
	virtual void Init();
	virtual void Update() override;

public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObject = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObject; }

private:
	void _SelectTileMap(DWORD_PTR _strKey, DWORD_PTR _NONE);
	bool _RenderTileMap(const string& _strName, CTexture* _pTex, GUI* pInst, GUI_CALLBACK _pFunc, TTileInfo& _tTileInfo);


	void _SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _Clear();
public:
	TileMapEditorGUI();
	virtual ~TileMapEditorGUI() override;
};