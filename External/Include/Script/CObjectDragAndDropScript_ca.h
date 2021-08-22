#pragma once
#include <Engine\CScript.h>
class CModuleScript_ca;
class CObjectDragAndDropScript_ca : public CScript {
private:
	CGameObject* m_pTargetObj;

	CModuleScript_ca* m_pModuleScript;
public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual void OnCollisionEnter2D(CCollider2D* _pOther);

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CObjectDragAndDropScript_ca);
	CObjectDragAndDropScript_ca();
	virtual ~CObjectDragAndDropScript_ca() override;
};