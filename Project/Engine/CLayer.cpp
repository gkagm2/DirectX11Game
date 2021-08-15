#include "pch.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CScene.h"

CLayer::CLayer() :
	m_pOwnScene(nullptr),
	m_eLayer(E_Layer::Default)
{
}

CLayer::~CLayer()
{
	Safe_Delete_Vector(m_vecRootObj);
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

void CLayer::Render()
{
	for (UINT i = 0; i < m_vecObj.size(); ++i)
		m_vecObj[i]->Render();
}

void CLayer::AddGameObject(CGameObject* _pObj, bool _bChangeChildLayer)
{
	assert(!(m_eLayer == _pObj->GetLayer()) && _T("오브젝트를 이미 속한 동일 레이어로 다시 넣었습니다."));


	// _pObj가 다른 Layer의 최상위 부모로 속해있었던 경우
	if (0 <= (int)_pObj->GetLayer() && (int)_pObj->GetLayer() < (int)E_Layer::End) {
		m_pOwnScene->GetLayer(_pObj->GetLayer())->_ResignGameObject(_pObj);
	}

	m_vecRootObj.push_back(_pObj);

	list<CGameObject*> que;
	que.push_back(_pObj);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();

		// 자식 오브젝트도 현재 레이어 소속으로 변경
		// 자식 오브젝트는 기존 레이어를 유지(but, 무소속인 상태인 경우 부모를 따라게 함)
		if (_bChangeChildLayer || E_Layer::End == pObj->GetLayer())
			pObj->_SetLayer(m_eLayer);

		for (UINT i = 0; i < pObj->m_vecChildObj.size(); ++i)
			que.push_back(pObj->m_vecChildObj[i]);
	}
}

void CLayer::_UnRegisterInRootGameObject(CGameObject* _pRootObj)
{
	vector<CGameObject*>::iterator iter = m_vecRootObj.begin();
	for (; iter != m_vecRootObj.end(); ++iter) {
		if (_pRootObj == *iter) { // 최상위 오브젝트를 찾았으면
			m_vecRootObj.erase(iter); //  삭제
			return;
		}
	}
	//assert(nullptr);
}

void CLayer::_ResignGameObject(CGameObject* _pObj)
{
	// Root오브젝트들을 담은 벡터에 있으면 삭제
	auto iter = m_vecRootObj.begin();
	for (; iter != m_vecRootObj.end(); ++iter) {
		if ((*iter) == _pObj) {
			m_vecRootObj.erase(iter);
			break;
		}
	}

	list<CGameObject*> que;
	que.push_back(_pObj);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();

		// 부모 포함해서 자식 모두 레이어에서 해제된다.
		pObj->_SetLayer(E_Layer::End);
		
		for (UINT i = 0; i < pObj->m_vecChildObj.size(); ++i)
			que.push_back(pObj->m_vecChildObj[i]);
	}
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