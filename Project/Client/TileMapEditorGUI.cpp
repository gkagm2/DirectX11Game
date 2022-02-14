#include "pch.h"
#include "TileMapEditorGUI.h"

#include "CImGuiManager.h"
#include <Engine\CTileMap.h>
#include <Engine\CGameObject.h>
#include <Engine\CResourceManager.h>
#include "ParamGUI.h"
#include <Engine\CRenderManager.h>
#include <Engine\CCamera.h>
#include "DebugGUI.h"
#include <Engine\CKeyManager.h>
#include <Engine\CTransform.h>
#include <Engine\CRenderManager.h>
#include <Engine\CObjectManager.h>
#include <Engine\CCursor.h>
#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>

TileMapEditorGUI::TileMapEditorGUI() :
	m_pTargetObject(nullptr),
	m_pTileMap{nullptr},
	m_pAtlasTileTex {nullptr},
	m_arrFaceTileCnt{ 0,0 },
	m_vAtlasTilePixelSize{ 0, 0},
	m_iBrushSize(0),
	m_bDeleteMode{ false },

	m_iSelectedTileIdx(-1),

	// canvas
	m_iGridColor(IMGUI_COLOR_GREEN) // green
{
}

TileMapEditorGUI::~TileMapEditorGUI()
{
}

void TileMapEditorGUI::Init()
{
	SetName(STR_GUI_TileMapEditor);
}

void TileMapEditorGUI::Update()
{
	if (!m_pTargetObject)
		return;
	if (m_pTargetObject->IsDead())
		return;
	if (!m_pTargetObject->TileMap())
		return;
	m_pTileMap = m_pTargetObject->TileMap();
	m_pAtlasTileTex = m_pTileMap->GetAtlasTexture().Get();



	// â�� �ϳ� ����
    if (!m_bGUIOpen)
        return;

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(STR_GUI_TileMapEditor, &m_bGUIOpen))
	{
		CCamera* pToolCamera = CRenderManager::GetInstance()->GetToolCamera();
		if (pToolCamera) {
			if (E_ProjectionType::Perspective == pToolCamera->GetProjectionType())
				pToolCamera->SetProjectionType(E_ProjectionType::Orthographic);
		}

		ImVec2 vAtlasTexResol = {};
		if (m_pTileMap->GetAtlasTexture().Get()) {
			ImVec2(m_pTileMap->GetAtlasTexture()->GetResolution().x, m_pTileMap->GetAtlasTexture()->GetResolution().y);
		}
		ImGui::Text("Atlas Texture Resolution [%d,%d]", vAtlasTexResol.x, vAtlasTexResol.x);

		ImGui::Text("Tile Face Size: %d %d", m_pTileMap->GetCol(), m_pTileMap->GetRow());

		ImGui::InputInt2("##Tile Map Size", m_arrFaceTileCnt);
		m_arrFaceTileCnt[0] = CMyMath::Clamp(m_arrFaceTileCnt[0], 0, INT_MAX);
		m_arrFaceTileCnt[1] = CMyMath::Clamp(m_arrFaceTileCnt[1], 0, INT_MAX);

		ImGui::Spacing();

		ImGui::Text("Texture Tile Size");
		ImGui::InputInt2("##Texture Tile Size", m_vAtlasTilePixelSize);
		m_vAtlasTilePixelSize[0] = CMyMath::Clamp(m_vAtlasTilePixelSize[0], 0, INT_MAX);
		m_vAtlasTilePixelSize[1] = CMyMath::Clamp(m_vAtlasTilePixelSize[1], 0, INT_MAX);
		ImGui::Spacing();

		// Ÿ�� ����
		static bool bBlankCreate = true;
		ImGui::Checkbox("Blank Tile##TileMapEditorCreateInitType", &bBlankCreate);
		ImGui::SameLine();
		if (ImGui::Button("Create")) {
			if (0 != m_arrFaceTileCnt[0] &&
				0 != m_arrFaceTileCnt[1] &&
				0 != m_vAtlasTilePixelSize[0] &&
				0 != m_vAtlasTilePixelSize[1] &&
				nullptr != m_pTileMap->GetAtlasTexture().Get()) {
				m_pTileMap->SetAtlasTilePixelSize(m_vAtlasTilePixelSize[0], m_vAtlasTilePixelSize[1]);
				m_pTileMap->CreateTile(m_arrFaceTileCnt[0], m_arrFaceTileCnt[1], bBlankCreate);
			}
		}

		ImGui::Separator();

		ImGui::Text("Mouse Right button clicking : delete mode");
		ImGui::Text("Brush Size : %d", m_iBrushSize + 1);
		ImGui::DragInt("##TileMap Brush Size", &m_iBrushSize, 1.f, 0, 10);

		// ��Ʋ�� �ؽ��� ����
		// ListView����.

		// ��Ʋ�� �ؽ��� ����
		CTexture* pAtlasTexture = m_pTileMap->GetAtlasTexture().Get();
		if (pAtlasTexture) {
			string strName{};
			tstring tstrName = pAtlasTexture->GetName();
			TStringToString(tstrName, strName);
			ParamGUI::Render_Texture(strName.c_str(), pAtlasTexture, this, (GUI_CALLBACK)&TileMapEditorGUI::_SelectTexture);
		}
		else {
			if (ImGui::Button("Atlas Texture Select")) {
				// ��� ����
				vector<tstring> vecName;
				CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecName);
				ListViewGUI* pListGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
				assert(pListGUI);
				if (pListGUI) {
					pListGUI->SetList(vecName, _T("Atlas Texture"));
					pListGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&TileMapEditorGUI::_SelectTexture);
					pListGUI->SetPreViewTexRender(true);
					pListGUI->SetActive(true);
				}
			}
		}

		// canvas�� �̿��ؼ� �ϳ� �ҷ�����
		_RenderCanvas();


		// �ؽ��� ����
		CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
		if (!pToolCam) {
			assert(pToolCam);
		}
		else {
			if (!ImGui::IsWindowFocused()) {
				m_bDeleteMode = false;
				// TODO(Jang) : Ÿ�� Ŭ�� �浹�ߴ��� ���ϴ� �ڵ带 CollisionManager�� �ֱ�
				Vector3 vMouseWorldPos = pToolCam->GetScreenToWorld2DPosition(MousePosition);

				Vector3 vObjWorldPos = GetTargetObject()->Transform()->GetPosition();
				Vector3 vScale = GetTargetObject()->Transform()->GetScale();
				Vector3 vHalfScale = vScale * 0.5f;
				vHalfScale.x = fabsf(vHalfScale.x);
				vHalfScale.y = fabsf(vHalfScale.y);

				Vector3 vLBWorldPos = {};
				vLBWorldPos.x = vObjWorldPos.x - vHalfScale.x;
				vLBWorldPos.y = vObjWorldPos.y - vHalfScale.y;

				Vector2 vOffsetLB = -Vector2(vLBWorldPos.x, vLBWorldPos.y);

				Vector3 vRTWorldPos = {};
				vRTWorldPos.x = vLBWorldPos.x + GetTargetObject()->Transform()->GetScale().x;
				vRTWorldPos.y = vLBWorldPos.y + GetTargetObject()->Transform()->GetScale().y;

				// ���콺�� Ÿ�� ���ο� Ŭ���ߴ°�?
				bool bIsTileClicked = false;

				if (InputKeyHold(E_Key::LBUTTON)) {
					if (vLBWorldPos.x < vMouseWorldPos.x && vRTWorldPos.x > vMouseWorldPos.x &&
						vLBWorldPos.y < vMouseWorldPos.y && vRTWorldPos.y > vMouseWorldPos.y) {
						bIsTileClicked = true;
					}

				}
				if (InputKeyHold(E_Key::LBUTTON) && InputKeyHold(E_Key::RBUTTON))
					m_bDeleteMode = true;

				// Ŭ������ ���
				if (bIsTileClicked) {
					vector<TTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
					Vector2 vOriginMousePos = Vector2(vMouseWorldPos.x + vOffsetLB.x, vScale.y - (vMouseWorldPos.y + vOffsetLB.y));

					Vector2 vOffsetScale = Vector2(m_pTileMap->GetCol() / vScale.x, m_pTileMap->GetRow() / vScale.y);

					vOriginMousePos *= vOffsetScale;

					int iClickX = (int)vOriginMousePos.x;
					int iClickY = (int)vOriginMousePos.y;

					// Brush�� �̿��Ͽ� �׸���
					int iMinX, iMinY, iMaxX, iMaxY;
					iMinX = iClickX - m_iBrushSize;
					iMaxX = iClickX + m_iBrushSize;
					iMinY = iClickY - m_iBrushSize;
					iMaxY = iClickY + m_iBrushSize;
					iMinX = max(0, iMinX);
					iMinY = max(0, iMinY);
					iMaxX = min(m_pTileMap->GetCol() - 1, iMaxX);
					iMaxY = min(m_pTileMap->GetRow() - 1, iMaxY);

					for (int y = iMinY; y <= iMaxY; ++y) {
						for (int x = iMinX; x <= iMaxX; ++x) {
							int idx = y * m_pTileMap->GetCol() + x;

							if (m_bDeleteMode)
								vecTiles[idx].idx = -1;
							else
								vecTiles[idx].idx = m_iSelectedTileIdx;
						}
					}
					/*int idx = iClickY * m_pTileMap->GetCol() + iClickX;
					vecTiles[idx].idx = m_iSelectedTileIdx;*/
				}
			}
		}

		if (ImGui::Button("CollderCreate##TileMap2D")) {
			CGameObject* pObj = CObjectManager::GetInstance()->CreateEmptyGameObject();
			pObj->SetName(_T("new Collders"));
			vector<TTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
			int iCol = m_pTileMap->GetCol();
			int iRow = m_pTileMap->GetRow();
			for (int y = 0; y < iRow; ++y) {
				for (int x = 0; x < iCol; ++x) {
					int idx = y * iCol + x;
					if (vecTiles[idx].idx >= 0) {
						// ����
						CGameObject* pColObj = CObjectManager::GetInstance()->CreateEmptyGameObject();
						pColObj->AddComponent<CCollider2D>();
						TCHAR strName[255];
						_stprintf_s(strName, 255, _T("Col[%d,%d]"), x, y);
						pColObj->SetName(strName);
						pColObj->Transform()->SetLocalPosition(Vector3(x + 0.5f, iRow - y - 0.5f, 0));
						CObject::AddChildGameObjectEvn(pObj, pColObj);
					}
				}
			}
		}
		if(ImGui::Button("OptimizeCollider##TileMap2D")) {
			_OptimizeCollisionArea();
		}

		ImGui::End();
    }
}

void TileMapEditorGUI::_SelectTileMap(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
}

void TileMapEditorGUI::_RenderCanvas()
{
	static ImVec2 scrolling(0.0f, 0.0f);
	static bool opt_enable_grid = true;
	static bool opt_enable_context_menu = true;
	static bool adding_line = false;

	static int arrGrid[2] = { 5, 5 };

	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
	CTexture* pTileTexture = m_pTileMap->GetAtlasTexture().Get();
	Vector2 vAtlasSize{};
	Vector2 vAtlasTileSize{};
	Vector2 vAtlasResol{};
	if (!m_pTileMap->GetAtlasTexture().Get())
		return;

	if (pTileTexture) {
		vAtlasSize = pTileTexture->GetResolution();
		vAtlasTileSize = m_pTileMap->GetAtlasTilePixelSize();
		vAtlasResol = m_pTileMap->GetAtlasTexture()->GetResolution();
	}

	// ���� ��ư�� Ŭ��������
	if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		// Ÿ���� ��ġ �� ��� �� ������ Ÿ���� �ε����� �����´�.

		// Ÿ���� Ŭ���ߴ°�?
		bool isTileClick = false;
		if (vAtlasSize.x > mouse_pos_in_canvas.x && vAtlasSize.y > mouse_pos_in_canvas.y &&
			0 < mouse_pos_in_canvas.x && 0 < mouse_pos_in_canvas.y)
			isTileClick = true;

		// Ÿ���� Ŭ������ �ʾ�����
		if (!isTileClick)
			m_iSelectedTileIdx = -1;
		else { // Ÿ���� Ŭ��������
			float iCol = mouse_pos_in_canvas.x / vAtlasTileSize.x;
			float iRow = mouse_pos_in_canvas.y / vAtlasTileSize.y;
			modf(iCol, &iCol);
			modf(iRow, &iRow);
			// ������ Ÿ���� �ε����� ��´�.
			m_iSelectedTileIdx = (int)iRow * m_pTileMap->GetAtlasTileYCnt() + (int)iCol;
		}
	}
	
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		Vector2 vGridSize = m_pTileMap->GetAtlasTilePixelSize();
		for (float x = fmodf(scrolling.x, vGridSize.x); x < canvas_sz.x; x += vGridSize.x)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), m_iGridColor);
		for (float y = fmodf(scrolling.y, vGridSize.y); y < canvas_sz.y; y += vGridSize.y)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), m_iGridColor);
	}

	// Draw Atlas Texture
	if (m_pAtlasTileTex) {
		TTextureBtnInfo tTexBtnInfo = { };
		ImVec2 vAtlasTexResolution = ImVec2(origin.x + m_pAtlasTileTex->GetResolution().x, origin.y + m_pAtlasTileTex->GetResolution().y);
		draw_list->AddImage(m_pAtlasTileTex->GetSRV().Get(), origin, vAtlasTexResolution);
	}

	// Ÿ���� ���� �Ǿ�����
	if (_IsTileSelectedInCanvas()) {
		Vector2 vAtlasTileSize = m_pTileMap->GetAtlasTilePixelSize();
		int iAtlasColCnt = m_pTileMap->GetAtlasTileXCnt();

		int iRow = m_iSelectedTileIdx / iAtlasColCnt;
		int iCol = m_iSelectedTileIdx % iAtlasColCnt;

		ImVec2 vLTPos = ImVec2(iCol * vAtlasTileSize.x, iRow * vAtlasTileSize.y);
		ImVec2  vRBPos = ImVec2((iCol + 1) * vAtlasTileSize.x, (iRow + 1) * vAtlasTileSize.y);

		vLTPos = ImVec2(vLTPos.x + origin.x, vLTPos.y + origin.y);
		vRBPos = ImVec2(vRBPos.x + origin.x, vRBPos.y + origin.y);
		draw_list->AddRect(vLTPos, vRBPos, IMGUI_COLOR_RED, 0.f, 0, 2.f);
	}

	draw_list->PopClipRect();
}

void TileMapEditorGUI::_SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
	// ������ �ؽ��ĸ� �˾Ƴ���.
	const char* pStrKey = (const char*)_strKey;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pTex = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pTex);
	GetTargetObject()->TileMap()->SetTileAtlasTexture(pTex);
}

void TileMapEditorGUI::_Clear()
{
	m_pTargetObject = nullptr;
	m_pTileMap = nullptr;
	m_pAtlasTileTex = nullptr;
	m_arrFaceTileCnt[0] = m_arrFaceTileCnt[1] = 0;
	m_vAtlasTilePixelSize[0] = m_vAtlasTilePixelSize[1] = 0;
	m_iSelectedTileIdx = -1;
}

void TileMapEditorGUI::_OptimizeCollisionArea()
{
	CGameObject* pObj = CObjectManager::GetInstance()->CreateEmptyGameObject();
	pObj->SetName(_T("new Collders"));
	const vector<TTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
	int iCol = m_pTileMap->GetCol();
	int iRow = m_pTileMap->GetRow();
	//for (int y = 0; y < iRow; ++y) {
	//	for (int x = 0; x < iCol; ++x) {
	//		int idx = y * iCol + x;
	//		if (vecTiles[idx].idx >= 0) {
	//			// ����
	//			CGameObject* pColObj = CObjectManager::GetInstance()->CreateEmptyGameObject();
	//			pColObj->AddComponent<CCollider2D>();
	//			TCHAR strName[255];
	//			_stprintf_s(strName, 255, _T("Col[%d,%d]"), x, y);
	//			pColObj->SetName(strName);
	//			pColObj->Transform()->SetLocalPosition(Vector3(x + 0.5f, iRow - y - 0.5f, 0));
	//			CObject::AddChildGameObjectEvn(pObj, pColObj);
	//		}
	//	}
	//}

	// ����ȭ�� �ϱ� ���ؼ� map ����

	// 2���� �迭 �����Ҵ�.
	int** grid = new int* [iRow];
	for (int i = 0; i < (int)iRow; ++i)
		grid[i] = new int[(int)iCol];

	for (int i = 0; i < (int)iRow; ++i)
		memset(grid[i], 0, sizeof(int) * iCol);

	// ���� ������ ������ ����

	// grid type -> 0 : not visited, 1: visited
	vector<std::pair<RECT, Vector2>> vecPos; // �ݶ��̴��� ������ų ���� ( second : ��ġ)
	// collider ���� ���.
	enum class E_VisitedState { not_visited, visited };
	int startPosX = 0;
	int startPosY = 0;
	int endPosX = 0;
	int endPosY = 0;
	for (int y = 0; y < (int)iRow; ++y) {
		for (int x = 0; x < (int)iCol; ++x) {
			endPosX = startPosX = x;
			endPosY = startPosY = y;

			
			const TTileInfo& pTile = vecTiles[startPosY * iCol + startPosX];
			if (( 0 < pTile.idx) && (int)E_VisitedState::not_visited == grid[startPosY][startPosX]) {
				GetEndIdxOfRectArea(grid, startPosX, startPosY, endPosX, endPosY);
				vecPos.push_back(std::make_pair(RECT{ startPosX,startPosY,endPosX,endPosY },Vector2(x,y)));
			}
			grid[startPosY][startPosX] = (int)E_VisitedState::visited;
		}
	}

	// �ݶ��̴� ����
	for (UINT i = 0; i < vecPos.size(); ++i) {
		RECT tRect = vecPos[i].first;
		Vector2 vPos = vecPos[i].second;

		const TTileInfo& pTile = vecTiles[tRect.top * iCol + tRect.left];


		// �ݶ��̴� ������Ʈ ����
		CGameObject* pColObj = CObjectManager::GetInstance()->CreateEmptyGameObject();
		pColObj->AddComponent<CCollider2D>();
		TCHAR strName[255];
		_stprintf_s(strName, 255, _T("Col[%d,%d]"), (int)vPos.x, (int)vPos.y);
		pColObj->SetName(strName);

		//Vector2 offset{ 0.5f, -y - 0.5f };
		//pColObj->Transform()->SetLocalPosition(Vector3(x + 0.5f, iRow - y - 0.5f, 0));
		CObject::AddChildGameObjectEvn(pObj, pColObj);

		float scaleX = (float)(tRect.right + 1 - tRect.left);
		float scaleY = (float)(tRect.bottom + 1 - tRect.top);

		pColObj->Collider2D()->SetOffsetScale(Vector2(scaleX, scaleY));
		Vector2 vOffsetPos = Vector2(scaleX * 0.5f, scaleY * 0.5f);
		float maxX = iCol * 0.5f;
		float maxY = iRow * 0.5f;
		vOffsetPos.x -= maxX;
		vOffsetPos.y -= maxY;
		Vector3 vResultPos = Vector3(vPos.x + vOffsetPos.x, vPos.y + vOffsetPos.y, 0.f);

		pColObj->Transform()->SetLocalPosition(vResultPos);
	}

	for (int i = 0; i < (int)iRow; ++i) {
		delete[] grid[i];
	}
	delete[] grid;
}

void TileMapEditorGUI::GetEndIdxOfRectArea(int** _grid, int _startX, int _startY, int& _endX, int& _endY)
{
	const vector<TTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
	int iCol = m_pTileMap->GetCol();
	int iRow = m_pTileMap->GetRow();
	const TTileInfo& tTile = vecTiles[_startY * iCol + _startX];

	// get min size of column and row
	int minX = (int)iCol - 1;
	int minY = (int)iRow - 1;
	int y = _startY;
	int x = _startX;
	for (; y < (int)iRow; ++y) {
		x = _startX;

		const TTileInfo& tTile = vecTiles[y * iCol + x];
		if (!(0 < tTile.idx) || (int)E_VisitedState::visited == _grid[y][x]) {
			minY = y - 1;
			break;
		}

		for (; x <= minX; ++x) {
			const TTileInfo& tTile = vecTiles[y * iCol + x];
			if (!(0 < tTile.idx) || (int)E_VisitedState::visited == _grid[y][x]) {
				if (minX > x - 1) {
					minX = x - 1;
				}
			}
		}
	}

	// ���� ������ �湮�ߴٰ� ǥ���Ѵ�.
	for (int y = _startY; y <= minY; ++y) {
		for (int x = _startX; x <= minX; ++x) {
			_grid[y][x] = 1;
		}
	}
	x = _startX;
	y = _startY;

	_endX = minX;
	_endY = minY;
}

