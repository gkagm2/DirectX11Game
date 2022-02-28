#include "pch.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CScene.h"

CLayer::CLayer() :
	m_pOwnScene(nullptr),
	m_iLayer(0)
{
}

CLayer::~CLayer()
{
	//Safe_Delete_Vector(m_vecRootObj);
}

void CLayer::Awake()
{
	for (UINT i = 0; i < m_vecRootObj.size(); ++i)
		m_vecRootObj[i]->Awake();

}

void CLayer::Start()
{
	for (UINT i = 0; i < m_vecRootObj.size(); ++i)
		m_vecRootObj[i]->Start();
}

void CLayer::PrevUpdate()
{
	for (UINT i = 0; i < m_vecRootObj.size(); ++i)
		m_vecRootObj[i]->PrevUpdate();
}

void CLayer::Update()
{
	for (UINT i = 0; i < m_vecRootObj.size(); ++i)
		m_vecRootObj[i]->Update();
}

void CLayer::LateUpdate()
{
	for (UINT i = 0; i < m_vecRootObj.size(); ++i)
		m_vecRootObj[i]->LateUpdate();
}

void CLayer::FinalUpdate()
{
	auto iter = m_vecRootObj.begin();
	while (iter != m_vecRootObj.end()) {
		// 위치정보 업데이트와 전체 벡터 등록 문제가 있을 수 있으니 삭제 예정 오브젝트도 FinalUpdate까지 호출.
		(*iter)->FinalUpdate();

		if ((*iter)->IsDead())
			iter = m_vecRootObj.erase(iter);
		else
			++iter;
	}
}

void CLayer::AddGameObject(CGameObject* _pObj, bool _bChangeChildLayer)
{
	assert(!(m_iLayer == _pObj->GetLayer()) && _T("오브젝트를 이미 속한 동일 레이어로 다시 넣었습니다."));

	// _pObj가 다른 Layer의 최상위 부모로 속해있었던 경우
	if (0 <= (int)_pObj->GetLayer() && (int)_pObj->GetLayer() < MAX_SIZE_LAYER) {
		m_pOwnScene->GetLayer(_pObj->GetLayer())->_ResignGameObject(_pObj);
	}

	m_vecRootObj.push_back(_pObj);

	vector<CGameObject*> vecRoots;
	m_pOwnScene->GetRootGameObjects(vecRoots);
	int iSize = (int)vecRoots.size();
	_pObj->_SetLocalAddress(iSize - 1);

	list<CGameObject*> que;
	que.push_back(_pObj);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();

		// 자식 오브젝트도 현재 레이어 소속으로 변경
		// 자식 오브젝트는 기존 레이어를 유지(but, 무소속인 상태인 경우 부모를 따라게 함)
		if (_bChangeChildLayer || MAX_SIZE_LAYER == pObj->GetLayer())
			pObj->_SetLayer(m_iLayer);

		for (UINT i = 0; i < pObj->m_vecChildObj.size(); ++i)
			que.push_back(pObj->m_vecChildObj[i]);
	}
}

void CLayer::_RegisterInRootGameObject(CGameObject* _pChildObj) {
	// 존재여부 확인 후 집어넣기
#pragma region 한곳에서만 사용하므로 할 필요 없음
			//vector<CGameObject*>::iterator iter = m_vecRootObj.begin();
//for (; iter != m_vecRootObj.end(); ++iter) {
//	if (*iter == _pChildObj) {
//		return; // 이미 존재하면 끝
//	}
//}

// 존재하지 않을 경우 삽입
//if(iter == m_vecRootObj.end())
//	m_vecRootObj.push_back(_pChildObj);  
#pragma endregion
	m_vecRootObj.push_back(_pChildObj);
	vector<CGameObject*> vecRoots;
	m_pOwnScene->GetRootGameObjects(vecRoots);
	int iSize = (int)vecRoots.size();
	_pChildObj->_SetLocalAddress(iSize - 1);
}

void CLayer::_UnRegisterInRootGameObject(CGameObject* _pRootObj)
{
	vector<CGameObject*>::iterator iter = m_vecRootObj.begin();
	for (; iter != m_vecRootObj.end(); ++iter) {
		if (_pRootObj == *iter) { // 최상위 오브젝트를 찾았으면
			m_vecRootObj.erase(iter); //  삭제

			vector<CGameObject*> vecRoots;
			CSceneManager::GetInstance()->GetCurScene()->GetRootGameObjects(vecRoots);
			for (size_t i = 0; i < vecRoots.size(); ++i)
				vecRoots[i]->_SetLocalAddress((int)i);

			return;
		}
	}

	// assert(nullptr && _T("최상위 오브젝트를 찾지 못함"));
}

// DFS를 이용하여 자식부터 바꿈
void CLayer::_ResignRecursive(CGameObject* _pObj) {
	const vector<CGameObject*>& vecChilds = _pObj->GetChildsObject();

	// 자식들 모두가 End로 초기화 되었으면
	for (UINT i = 0; i < vecChilds.size(); ++i) {
		if (MAX_SIZE_LAYER != vecChilds[i]->GetLayer()) {
			_ResignRecursive(vecChilds[i]); // 자식껄로 재귀함수 호출
		}
	}

	// 부모가 존재하면
	if (_pObj->GetParentObject()) {
		// 부모와 레이어가 같지 않다면 Layer의 Root vector에 존재한다는 의미이므로 레이어 할당을 해제한다.
		if (_pObj->GetParentObject()->GetLayer() != _pObj->GetLayer()) {
			CLayer* pLayer = CSceneManager::GetInstance()->GetCurScene()->GetLayer(_pObj->GetLayer());
			vector<CGameObject*>& vecRootObj = pLayer->GetRootGameObjects();

			// Root오브젝트들을 담고 최상위 일때 벡터에 있으면 삭제
			auto iter = vecRootObj.begin();
			for (; iter != vecRootObj.end(); ++iter) {
				if ((*iter) == _pObj) {
					vecRootObj.erase(iter);
					break;
				}
			}
			_pObj->_SetLayer(MAX_SIZE_LAYER);
		}
	}
	else { // 부모가 존재하지 않으면
		// Root오브젝트들을 담고있는 Layer의 vector에서 삭제
		CLayer* pLayer = CSceneManager::GetInstance()->GetCurScene()->GetLayer(_pObj->GetLayer());
		vector<CGameObject*>& vecRootObj = pLayer->GetRootGameObjects();
		auto iter = vecRootObj.begin();
		for (; iter != vecRootObj.end(); ++iter) {
			if ((*iter) == _pObj) {
				vecRootObj.erase(iter);
				break;
			}
		}
	}
	_pObj->_SetLayer(MAX_SIZE_LAYER);
}

void CLayer::_ResignGameObject(CGameObject* _pObj)
{
	_ResignRecursive(_pObj);
	/*
	CGameObject* pStartObj = _pObj;

	list<CGameObject*> que;
	que.push_back(_pObj);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();

		// 부모 포함해서 자식 모두 레이어에서 해제된다.
		pObj->_SetLayer(MAX_SIZE_LAYER);
		
		for (UINT i = 0; i < pObj->m_vecChildObj.size(); ++i)
			que.push_back(pObj->m_vecChildObj[i]);
	}
	*/
}

bool CLayer::SaveToScene(FILE* _pFile)
{
	CObject::SaveToScene(_pFile);

	// 최상위 부모 오브젝트 저장
	UINT iRootObjCount = (UINT)m_vecRootObj.size();
	FWrite(iRootObjCount, _pFile);

	for (UINT i = 0; i < m_vecRootObj.size(); ++i)
		m_vecRootObj[i]->SaveToScene(_pFile);

	return true;
}

bool CLayer::LoadFromScene(FILE* _pFile)
{
	CObject::LoadFromScene(_pFile);

	// 최상위 부모 오브젝트 읽기
	UINT iRootObjCount = 0;
	FRead(iRootObjCount, _pFile);

	CGameObject* pObj = nullptr;
	for (UINT i = 0; i < iRootObjCount; ++i) {
		pObj = new CGameObject;
		pObj->LoadFromScene(_pFile, 0);
		AddGameObject(pObj, false);
	}

	return true;
}