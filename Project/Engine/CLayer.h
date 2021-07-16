#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CGameObject;
class CScene;
class CLayer : public CObject , ILifeCycleInterface
{
private:
	CScene* m_pOwnScene;
	vector<CGameObject*> m_vecRootObj; // 최상위 부모들
	vector<CGameObject*> m_vecObj;	// 모든 오브젝트 ( 렌더링을 위해 존재)
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
	// 자식 오브젝트가 레이어를 유지하며 최상위 부모로 등록시 사용
	void _RegisterInRootGameObject(CGameObject* _ChildObj) { m_vecRootObj.push_back(_ChildObj); }

	// 파라미터로 받은 오브젝트와 오브젝트의 자식 오브젝트들의 레이어를 해제
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