#pragma once
#include "GUI.h"
class CGameObject;
class CTileMap;
class TileMapEditorGUI : public GUI
{
private:
	int m_iFaceSize[2];
	int m_iAtlasTileColRowSize[2];

	CGameObject* m_pTargetObject;

public:
	virtual void Init();
	virtual void Update() override;

public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObject = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObject; }

public:
	void _SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _Clear();
public:
	TileMapEditorGUI();
	virtual ~TileMapEditorGUI() override;
};