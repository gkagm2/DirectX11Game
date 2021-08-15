#pragma once
#include <Engine\CScript.h>
class CObjectDragAndDropScript_ca : public CScript {
private:
	//CGameObject* pTargetObj;
public:
	virtual void Start() override;
	virtual void Update() override;
public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CObjectDragAndDropScript_ca);
	CObjectDragAndDropScript_ca();
	virtual ~CObjectDragAndDropScript_ca() override;
};