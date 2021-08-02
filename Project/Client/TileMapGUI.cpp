#include "pch.h"
#include "TileMapGUI.h"
#include <Engine\CTileMap.h>

TileMapGUI::TileMapGUI() :
	ComponentGUI(E_ComponentType::TileMap)
{
}

TileMapGUI::~TileMapGUI()
{
}

void TileMapGUI::Update()
{
	if (false == Start())
		return;

	CTileMap* pTileMap = GetTargetObject()->TileMap();
	
	End();
}