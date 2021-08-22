#pragma once
#include <Engine\CScript.h>

class CGameManager_ca;
class CObjectDragAndDropScript_ca;
class CManagerContainer_ca : public CScript
{
private:
	static unique_ptr<CManagerContainer_ca> m_pInstance;
public:
	CManagerContainer_ca* GetInstance() {
		if (nullptr == m_pInstance)
			m_pInstance = std::make_unique<CManagerContainer_ca>();
		return m_pInstance.get();
	}

public:
	virtual void Awake() override;

	// Container list
private:
	CGameManager_ca* m_pGameManager;
	CObjectDragAndDropScript_ca* m_pObjDragAndDrop;
public:
	CGameManager_ca* GameManager() { return m_pGameManager; }
	CObjectDragAndDropScript_ca* ObjectDragAndDrop() {
		return m_pObjDragAndDrop;	}

public:
	CLONE(CManagerContainer_ca);
	CManagerContainer_ca();
	virtual ~CManagerContainer_ca() override;
};