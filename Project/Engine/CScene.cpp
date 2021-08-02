#include "pch.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"

CScene::CScene()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i) {
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_eLayer = (E_Layer)i;
		m_arrLayer[i]->m_pOwnScene = this;
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

void CScene::_AddGameObject(CGameObject* _pObj, E_Layer _eLayer, bool _bChangeChildLayer)
{
	m_arrLayer[(UINT)_eLayer]->AddGameObject(_pObj, _bChangeChildLayer);
}

CGameObject* CScene::FindGameObject(const tstring& _strName, E_Layer _eLayer)
{
	if (E_Layer::End != _eLayer) {
		// 레이어의 모든 루트 오브젝트들을 가져온다.
		const vector<CGameObject*>& vecGameObjects = m_arrLayer[(UINT)_eLayer]->GetRootGameObjects();

		for (UINT i = 0; i < vecGameObjects.size(); ++i) {
			// BFS를 이용하여 자식 오브젝트들을 순회하며 이름을 찾는다.
			list<CGameObject*> que;
			que.push_back(vecGameObjects[i]);
			
			while (!que.empty()) {
				CGameObject* pObj = que.front();
				que.pop_front();

				if (_strName == pObj->GetName())
					return pObj;

				const vector<CGameObject*>& vecChildsObj = pObj->GetChildsObject();
				for (UINT j = 0; j < vecChildsObj.size(); ++j)
					que.push_back(vecChildsObj[j]);
			}
		}
	}
	else {
		// 모든 레이어를 순회한다.
		for (UINT iLayerNum = 0; iLayerNum < (UINT)E_Layer::End; ++iLayerNum) {
			const vector<CGameObject*>& vecGameObjects = m_arrLayer[iLayerNum]->GetRootGameObjects();

			for (UINT i = 0; i < vecGameObjects.size(); ++i) {
				list<CGameObject*> que;
				que.push_back(vecGameObjects[i]);

				while (!que.empty()) {
					CGameObject* pObj = que.front();
					que.pop_front();

					if (_strName == pObj->GetName())
						return pObj;

					const vector<CGameObject*>& vecChildsObj = pObj->GetChildsObject();
					for (UINT j = 0; j < vecChildsObj.size(); ++j)
						que.push_back(vecChildsObj[j]);
				}
			}
		}
	}
	return nullptr;
}

void CScene::_UnRegisterAllObjects()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->m_vecObj.clear();
}

bool CScene::SaveToScene(FILE* _pFile)
{
	CObject::SaveToScene(_pFile);

	// Layer 정보
 	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->SaveToScene(_pFile);

	return true;
}

bool CScene::LoadFromScene(FILE* _pFile)
{
	CObject::LoadFromScene(_pFile);

	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->LoadFromScene(_pFile);

	return true;
}