#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CGameObject;
class CLayer : public CObject , ILifeCycleInterface
{
private:
	vector<CGameObject*> m_vecParentObj; // �ֻ��� �θ��
	vector<CGameObject*> m_vecObj;		 // ��� ������ư
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
	void AddGameObject(CGameObject* _pObj);
	vector<CGameObject*>& GetGameObjects() { return m_vecParentObj; }
	void RegisterGameObject(CGameObject* _pObj) { m_vecObj.push_back(_pObj); }

public:
	CLayer();
	virtual ~CLayer() override;

	friend class CScene;
};