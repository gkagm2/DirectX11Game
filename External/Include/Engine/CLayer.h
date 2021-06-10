#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CGameObject;
class CLayer : public CObject , ILifeCycleInterface
{
private:
	vector<CGameObject*> m_vecParentObj;
	int m_iLayerIdx;

public:
	virtual void Awake() final;
	virtual void Start() final;
	virtual void PrevUpdate() final;
	virtual void Update() final;
	virtual void LateUpdate() final;
	virtual void FinalUpdate() final;
	virtual void Render() final;

public:
	void AddGameObject(CGameObject* _pObj) { m_vecParentObj.push_back(_pObj); }
	vector<CGameObject*>& GetGameObjects() { return m_vecParentObj; }

public:
	CLayer();
	virtual ~CLayer() override;

	friend class CScene;
};