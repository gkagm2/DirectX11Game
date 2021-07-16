#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CGameObject;
class CScene;
class CLayer : public CObject , ILifeCycleInterface
{
private:
	CScene* m_pOwnScene;
	vector<CGameObject*> m_vecRootObj; // �ֻ��� �θ��
	vector<CGameObject*> m_vecObj;	// ��� ������Ʈ ( �������� ���� ����)
	E_Layer m_eLayer;

public:
	virtual void Awake() final;
	virtual void Start() final;
	virtual void PrevUpdate() final;
	virtual void Update() final;
	virtual void LateUpdate() final;
	virtual void FinalUpdate() final;
	virtual void Render() final;

public:
	void AddGameObject(CGameObject* _pObj, bool _bChangeChildLayer);
	vector<CGameObject*>& GetGameObjects() { return m_vecObj; }
	void RegisterGameObject(CGameObject* _pObj) { m_vecObj.push_back(_pObj); }
	vector<CGameObject*>& GetRootGameObjects() { return m_vecRootObj; }

private:
	// �ڽ� ������Ʈ�� ���̾ �����ϸ� �ֻ��� �θ�� ��Ͻ� ���
	void _RegisterInRootGameObject(CGameObject* _ChildObj) { m_vecRootObj.push_back(_ChildObj); }

	// �Ķ���ͷ� ���� ������Ʈ�� ������Ʈ�� �ڽ� ������Ʈ���� ���̾ ����
	void _ResignGameObject(CGameObject* _pObj);

public:
	virtual bool SaveToScene(FILE* _pFile);
	virtual bool LoadFromScene(FILE* _pFile);

private:
	CLONE_DISABLE(CLayer);

public:
	CLayer();
	virtual ~CLayer() override;

	friend class CScene;
	friend class CEventManager;
};