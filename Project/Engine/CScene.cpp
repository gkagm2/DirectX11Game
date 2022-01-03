#include "pch.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"

CScene::CScene()
{
	for (UINT i = 0; i < MAX_SIZE_LAYER; ++i) {
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_iLayer = i;
		m_arrLayer[i]->m_pOwnScene = this;
	}
}

CScene::~CScene()
{
	vector<CGameObject*> vecObjs;
	GetRootGameObjects(vecObjs);
	Safe_Delete_Vector(vecObjs);

	Safe_Delete_Array(m_arrLayer);
}

void CScene::Awake()
{
	for (UINT i = 0; i < m_vecRootObjs.size(); ++i)
		m_vecRootObjs[i]->Awake();
}

void CScene::Start()
{
	for (UINT i = 0; i < m_vecRootObjs.size(); ++i)
		m_vecRootObjs[i]->Start();
}

void CScene::PrevUpdate()
{
	for (UINT i = 0; i < m_vecRootObjs.size(); ++i)
		m_vecRootObjs[i]->PrevUpdate();
}

void CScene::Update()
{
	for (UINT i = 0; i < m_vecRootObjs.size(); ++i)
		m_vecRootObjs[i]->Update();
}

void CScene::LateUpdate()
{
	for (UINT i = 0; i < m_vecRootObjs.size(); ++i)
		m_vecRootObjs[i]->LateUpdate();
}

void CScene::FinalUpdate()
{
	GetRootGameObjects(m_vecRootObjs);

	auto iter = m_vecRootObjs.begin();

	while (iter != m_vecRootObjs.end()) {
		(*iter)->FinalUpdate();
		if ((*iter)->IsDead())
			iter = m_vecRootObjs.erase(iter);
		else
			++iter;
	}
}

void CScene::_AddGameObject(CGameObject* _pObj, UINT _iLayer, bool _bChangeChildLayer)
{
	m_arrLayer[_iLayer]->AddGameObject(_pObj, _bChangeChildLayer);
}

CGameObject* CScene::FindGameObject(const tstring& _strName, UINT _iLayer)
{
	return _FindGameObject(_strName, _iLayer);
}

CGameObject* CScene::FindGameObject(const uuid& _id, UINT _iLayer)
{
	return _FindGameObject(_id, _iLayer);
}

template<typename T>
T* CScene::FindComponent()
{
	vector<CGameObject*> vecRootObjs;
	GetRootGameObjects(vecRootObjs);

	// 모든 루트 오브젝트부터 순회한다.
	for (UINT i = 0; i < vecRootObjs.size(); ++i) {
		CGameObject* pObj = vecRootObjs[i];

		T* pComponent = pObj->GetComponent<T>();
		if (pComponent)
			return pComponent;

		pComponent = pObj->FindComponentInChilds<T>();
		if (pComponent)
			return pComponent;
	}
	return nullptr;
}

template<typename T>
CGameObject* CScene::_FindGameObject(const T& _Type, UINT _iLayer)
{
	if (MAX_SIZE_LAYER != _iLayer) {
		// 레이어의 모든 루트 오브젝트들을 가져온다.
		const vector<CGameObject*>& vecGameObjects = m_arrLayer[_iLayer]->GetRootGameObjects();

		for (UINT i = 0; i < vecGameObjects.size(); ++i) {
			// BFS를 이용하여 자식 오브젝트들을 순회하며 이름을 찾는다.
			list<CGameObject*> que;

			que.push_back(vecGameObjects[i]);

			while (!que.empty()) {
				CGameObject* pObj = que.front();
				que.pop_front();

				if (_CompareGameObject(_Type, pObj))
					return pObj;
				
				const vector<CGameObject*>& vecChildsObj = pObj->GetChildsObject();
				for (UINT j = 0; j < vecChildsObj.size(); ++j) {
					if (_iLayer == vecChildsObj[j]->GetLayer())
						que.push_back(vecChildsObj[j]);
				}
			}
		}
	}
	else {
		vector<CGameObject*> vecRootObjs;
		GetRootGameObjects(vecRootObjs);

		// 모든 루트 오브젝트부터 순회한다.
		for (UINT i = 0; i < vecRootObjs.size(); ++i) {
			list<CGameObject*> que;
			que.push_back(vecRootObjs[i]);

			while (!que.empty()) {
				CGameObject* pObj = que.front();
				que.pop_front();

				if (_CompareGameObject(_Type, pObj))
					return pObj;

				const vector<CGameObject*>& vecChildsObj = pObj->GetChildsObject();
				for (UINT j = 0; j < vecChildsObj.size(); ++j)
					que.push_back(vecChildsObj[j]);
			}
		}
	}
	return nullptr;
}

template<typename T>
bool CScene::_CompareGameObject(const T& _Type, CGameObject* _pObj)
{
	bool bIsFind = false;
	const type_info& info = typeid(_Type);

	if (&info == &typeid(uuid)) {
		if ((const uuid&)_Type == _pObj->GetUUID())
			bIsFind = true;
	}
	else if (&info == &typeid(tstring)) {
		if ((const tstring&)_Type == _pObj->GetName())
			bIsFind = true;
	}
	return bIsFind;
}

void CScene::GetRootGameObjects(vector<CGameObject*>& _vecRootObjs, UINT _iLayer)
{
	_vecRootObjs.clear();

	// Scene중에 최상위 오브젝트를 가져옴
	for (UINT i = 0; i < MAX_SIZE_LAYER; ++i) {
		CLayer* pLayer = GetLayer(i);
		const vector<CGameObject*>& vecLayerRootObj = pLayer->GetRootGameObjects();
		for (UINT j = 0; j < vecLayerRootObj.size(); ++j) {
			if (nullptr == vecLayerRootObj[j]->GetParentObject()) {
				if(MAX_SIZE_LAYER == _iLayer)
					_vecRootObjs.push_back(vecLayerRootObj[j]);
				else if (_iLayer == vecLayerRootObj[j]->GetLayer())
					_vecRootObjs.push_back(vecLayerRootObj[j]);
			}
		}
	}
}

// 전체 검색
void CScene::GetGameObjects(vector<CGameObject*>& _vecObjs, UINT _iLayer)
{
	// BFS 이요
	_vecObjs.clear();
	vector<CGameObject*> vecRootObjs;
	GetRootGameObjects(vecRootObjs);

	list<CGameObject*> que;
	for (UINT i = 0; i < vecRootObjs.size(); ++i)
		que.push_back(vecRootObjs[i]);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();

		if (_iLayer == MAX_SIZE_LAYER)
			_vecObjs.push_back(pObj);
		else {
			if(_iLayer == pObj->GetLayer())
				_vecObjs.push_back(pObj);
		}

		const vector<CGameObject*>& vecChilds = pObj->GetChildsObject();
		for (UINT i = 0; i < vecChilds.size(); ++i) {
			que.push_front(vecChilds[i]);
		}
	}
}

void CScene::_UnRegisterAllObjects()
{
	for (UINT i = 0; i < MAX_SIZE_LAYER; ++i)
		m_arrLayer[i]->m_vecObj.clear();
}

bool CScene::SaveToScene(FILE* _pFile)
{
	CObject::SaveToScene(_pFile);

	// Layer 정보
 	for (UINT i = 0; i < MAX_SIZE_LAYER; ++i)
		m_arrLayer[i]->SaveToScene(_pFile);

	return true;
}

bool CScene::LoadFromScene(FILE* _pFile)
{
	CObject::LoadFromScene(_pFile);

	for (UINT i = 0; i < MAX_SIZE_LAYER; ++i)
		m_arrLayer[i]->LoadFromScene(_pFile);
	GetRootGameObjects(m_vecRootObjs);
	return true;
}