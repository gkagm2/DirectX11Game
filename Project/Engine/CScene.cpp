#include "pch.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"

CScene::CScene()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i) {
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_iLayerIdx = i;
	}
}

CScene::~CScene()
{
	Safe_Delete_Array(m_arrLayer);
}

void CScene::Awake()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->Awake();
}

void CScene::Start()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->Start();
}

void CScene::PrevUpdate()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->PrevUpdate();
}

void CScene::Update()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->Update();
}

void CScene::LateUpdate()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->LateUpdate();
}

void CScene::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->FinalUpdate();
}

void CScene::Render()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->Render();
}

void CScene::AddGameObject(CGameObject* _pObj, E_Layer _eLayer)
{
	m_arrLayer[(UINT)_eLayer]->AddGameObject(_pObj);
}

CGameObject* CScene::FindGameObject(const tstring& _strName, E_Layer _eLayer)
{
	if (E_Layer::End != _eLayer) {
		vector<CGameObject*>& vecGameObjects = m_arrLayer[(UINT)_eLayer]->GetGameObjects();

		for (UINT i = 0; i < vecGameObjects.size(); ++i) {
			if (_strName == vecGameObjects[i]->GetName())
				return vecGameObjects[i];
		}
	}
	else {
		for (UINT iLayerNum = 0; iLayerNum < (UINT)E_Layer::End; ++iLayerNum) {
			vector<CGameObject*>& vecGameObjects = m_arrLayer[iLayerNum]->GetGameObjects();

			for (UINT i = 0; i < vecGameObjects.size(); ++i) {
				if (_strName == vecGameObjects[i]->GetName())
					return vecGameObjects[i];
			}
		}
	}
	return nullptr;
}