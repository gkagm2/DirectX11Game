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
	m_vecObj.clear();
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
		// ��ġ���� ������Ʈ�� ��ü ���� ��� ������ ���� �� ������ ���� ���� ������Ʈ�� FinalUpdate���� ȣ��.
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
	assert(!(m_eLayer == _pObj->GetLayer()) && _T("������Ʈ�� �̹� ���� ���� ���̾�� �ٽ� �־����ϴ�."));


	// _pObj�� �ٸ� Layer�� �ֻ��� �θ�� �����־��� ���
	if (0 <= (int)_pObj->GetLayer() && (int)_pObj->GetLayer() < (int)E_Layer::End) {
		m_pOwnScene->GetLayer(_pObj->GetLayer())->_ResignGameObject(_pObj);
	}

	m_vecRootObj.push_back(_pObj);

	list<CGameObject*> que;
	que.push_back(_pObj);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();

		// �ڽ� ������Ʈ�� ���� ���̾� �Ҽ����� ����
		// �ڽ� ������Ʈ�� ���� ���̾ ����(but, ���Ҽ��� ������ ��� �θ� ����� ��)
		if (_bChangeChildLayer || E_Layer::End == pObj->GetLayer())
			pObj->_SetLayer(m_eLayer);

		for (UINT i = 0; i < pObj->m_vecChildObj.size(); ++i)
			que.push_back(pObj->m_vecChildObj[i]);
	}
}

void CLayer::_ResignGameObject(CGameObject* _pObj)
{
	// Root������Ʈ���� ���� ���Ϳ� ������ ����
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

		// �θ� �����ؼ� �ڽ� ��� ���̾�� �����ȴ�.
		pObj->_SetLayer(E_Layer::End);
		
		for (UINT i = 0; i < pObj->m_vecChildObj.size(); ++i)
			que.push_back(pObj->m_vecChildObj[i]);
	}
}

bool CLayer::SaveToScene(FILE* _pFile)
{
	CObject::SaveToScene(_pFile);

	// �ֻ��� �θ� ������Ʈ ����
	UINT iRootObjCount = (UINT)m_vecRootObj.size();
	fwrite(&iRootObjCount, sizeof(UINT), 1, _pFile);
	//FWrite(iRootObjCount, _pFile);

	for (UINT i = 0; i < m_vecRootObj.size(); ++i)
		m_vecRootObj[i]->SaveToScene(_pFile);

	return true;
}

bool CLayer::LoadFromScene(FILE* _pFile)
{
	CObject::LoadFromScene(_pFile);

	// �ֻ��� �θ� ������Ʈ �б�
	UINT iRootObjCount = 0;
	//FRead(iRootObjCount, _pFile);
	fread(&iRootObjCount, sizeof(UINT), 1, _pFile);

	CGameObject* pObj = nullptr;
	for (UINT i = 0; i < iRootObjCount; ++i) {
		pObj = new CGameObject;
		pObj->LoadFromScene(_pFile, 0);
		AddGameObject(pObj, false);
	}

	return true;
}