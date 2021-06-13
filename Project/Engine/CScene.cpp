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

void CScene::Render()
{
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
		m_arrLayer[i]->Render();
}

void CScene::AddGameObject(CGameObject* _pObj, E_Layer _eLayer, bool _bChangeChildLayer)
{
	m_arrLayer[(UINT)_eLayer]->AddGameObject(_pObj, _bChangeChildLayer);
}

CGameObject* CScene::FindGameObject(const tstring& _strName, E_Layer _eLayer)
{
	if (E_Layer::End != _eLayer) {
		// ���̾��� ��� ��Ʈ ������Ʈ���� �����´�.
		const vector<CGameObject*>& vecGameObjects = m_arrLayer[(UINT)_eLayer]->GetRootGameObjects();

		for (UINT i = 0; i < vecGameObjects.size(); ++i) {
			// BFS�� �̿��Ͽ� �ڽ� ������Ʈ���� ��ȸ�ϸ� �̸��� ã�´�.
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
		// ��� ���̾ ��ȸ�Ѵ�.
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