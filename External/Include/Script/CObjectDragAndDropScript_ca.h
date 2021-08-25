#pragma once
#include <Engine\CScript.h>
class CModuleScript_ca;
class CObjectDragAndDropScript_ca : public CScript {
private:
	CGameObject* m_pTargetObj;
	CGameObject* m_pNearestModuleObj;
	bool m_bIsConnectableOtherObj;

	CModuleScript_ca* m_pModuleScript;

private:
	float m_fMaxSearchObjDistance; // 근처 오브젝트를 찾기 위한 최대 거리
	float m_fMaxConnectableDistance; // 모듈끼리 연결 가능한 최대 거리

public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual void OnCollisionEnter2D(CCollider2D* _pOther);

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void FindNearbyModuleObjects( vector<CGameObject*> _vecObjects, const Vector3& _vPosition, float _fMaxDistance);

	CGameObject* FindNearestModuleObject(const Vector3& _vPosition);

public:
	CLONE(CObjectDragAndDropScript_ca);
	CObjectDragAndDropScript_ca();
	virtual ~CObjectDragAndDropScript_ca() override;
};