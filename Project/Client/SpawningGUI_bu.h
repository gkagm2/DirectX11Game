#pragma once
#include "GUI.h"
class SpawningGUI_bu : public GUI
{
private:
	int m_iCurCategoryItemIdx;

	// Cate : Category
	vector<string> m_CateList;

	vector<string> m_CateObjList;
	int m_iCurObjItemIdx;

	vector<string> m_CateActorsList;
	int m_iCurActorsItemIdx;

	vector<string> m_CatePickupsList;
	int m_iCurPickupsItemIdx;

	vector<string> m_itemHpTypeList;
	int m_iCurItemHpTypeIdx;
	vector<string> m_itemArmorTypeList;
	int m_iCurItemArmorTypeIdx;

	vector<string> m_CateDoorAndSwitchList;
	int m_iCurDoorAndSwitchItemIdx;

	vector<string> m_enemyWeaponTypeList;
	int m_iCurEnemyWeaponTypeIdx;

	vector<string> m_particleTypeList;
	int m_iCurParticleTypeIdx;


	vector<string> m_Actors_EnemyList;


	SharedPtr<CPrefab> m_pSpotLightPrefab;
	SharedPtr<CPrefab> m_pPortalPrefab;

	SharedPtr<CPrefab> m_pPlayerPrefab;
	SharedPtr<CPrefab> m_pEnemyPrefab;
	SharedPtr<CPrefab> m_pDoorPrefab;
	SharedPtr<CPrefab> m_pSwitch1Prefab;
	SharedPtr<CPrefab> m_pSwitch2Prefab;
	SharedPtr<CPrefab> m_pExitDoorPrefab;

	SharedPtr<CPrefab> m_pBulletPickupPrefab;
	SharedPtr<CPrefab> m_pHpPickupPrefab;
	SharedPtr<CPrefab> m_pArmorPickupPrefab;
	SharedPtr<CPrefab> m_pWeaponPickupPrefab;

	SharedPtr<CPrefab> m_pFogParticlePrefab;

	std::function<CGameObject*()> m_pCreateFunc;

	CGameObject* m_pTargetObj;

	CGameObject* m_pInsertStage1;
	CGameObject* m_pInsertStage2;
	CGameObject* m_pInsertStage3;

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void _Clear();

	CGameObject* _CreatePlayer();
	CGameObject* _CreateEnemy();
	CGameObject* _CreateDoorAndSwitch();
	CGameObject* _CreatePickupsItem();
	CGameObject* _CreateObject();
	CGameObject* _CreateParticle();

	CGameObject* _GetClickedObj(const Vector3& _vWorldPos);


public:
	SpawningGUI_bu();
	virtual ~SpawningGUI_bu() override;
};
