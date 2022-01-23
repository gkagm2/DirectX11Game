#include "pch.h"
#include "SpawningGUI_bu.h"
#include <Engine\CKeyManager.h>
#include <Engine\CCamera.h>
#include <Engine\CRenderManager.h>
#include <Engine\CCamera.h>
#include <Engine\CResourceManager.h>
#include "ParamGUI.h"

#include <Script\CWeapon_bu.h>


SpawningGUI_bu::SpawningGUI_bu()
{
}

SpawningGUI_bu::~SpawningGUI_bu()
{
}


void SpawningGUI_bu::Init()
{
	SetName(STR_GUI_Spawning_bu);

	m_CateList.push_back("objects");
	m_CateList.push_back("actors");
	m_CateList.push_back("pickups");
	m_CateList.push_back("doorAndSwitch");

	//m_CateObjList.push_back("destruct object");
	m_CateObjList.push_back("spot light");
	m_CateObjList.push_back("portal");


	// TODO(Jang ) : 해야됨
	m_CateList.push_back("corpses");
	m_CateList.push_back("boss");

	m_CateActorsList.push_back("player");
	m_CateActorsList.push_back("enemy");

	m_CatePickupsList.push_back("Shotgun Bullet");
	m_CatePickupsList.push_back("Machinegun Bullet");
	m_CatePickupsList.push_back("Flame Thrower Bullet");
	m_CatePickupsList.push_back("Grenade Launcher Bullet");
	m_CatePickupsList.push_back("Laser Gun Bullet");
	m_CatePickupsList.push_back("Hp");
	m_CatePickupsList.push_back("Armor");
	m_CatePickupsList.push_back("Shotgun");
	m_CatePickupsList.push_back("Machinegun");
	m_CatePickupsList.push_back("Flame Thrower");
	m_CatePickupsList.push_back("Grenade Launcher");
	m_CatePickupsList.push_back("Laser Gun");

	m_CateDoorAndSwitchList.push_back("door");
	m_CateDoorAndSwitchList.push_back("elevator1");
	m_CateDoorAndSwitchList.push_back("elevator2");
	m_CateDoorAndSwitchList.push_back("exitDoor");

	m_itemHpTypeList.push_back("Level1");
	m_itemHpTypeList.push_back("Level2");
	m_itemHpTypeList.push_back("Level3");
	m_itemHpTypeList.push_back("Level4");

	m_itemArmorTypeList.push_back("Level1");
	m_itemArmorTypeList.push_back("Level2");
	m_itemArmorTypeList.push_back("Level3");
	m_itemArmorTypeList.push_back("Level4");

	m_enemyWeaponTypeList.push_back("Chainsaw");
	m_enemyWeaponTypeList.push_back("Shotgun");
	m_enemyWeaponTypeList.push_back("MachineGun");
	m_enemyWeaponTypeList.push_back("FlameThrower");
	m_enemyWeaponTypeList.push_back("GrenadeLauncher");
	m_enemyWeaponTypeList.push_back("LaserGun");

	SetActive(false);
}

void SpawningGUI_bu::Update()
{
	if (!m_bGUIOpen) {
		return;
	}

	if (E_SceneMode::Play == CSceneManager::GetInstance()->GetSceneMode()) {
		_Clear();
		return;
	}

	if (ImGui::Begin(STR_GUI_Spawning_bu, &m_bGUIOpen)) {
		if (!m_bGUIOpen) {
			_Clear();
			ImGui::End();
			return;
		}

		if (!m_pInsertStage1)
			m_pInsertStage1 = FIND_GameObject(_T("Stage1Objs"));
		if (!m_pInsertStage2)
			m_pInsertStage2 = FIND_GameObject(_T("Stage2Objs"));
		if (!m_pInsertStage3)
			m_pInsertStage3 = FIND_GameObject(_T("Stage3Objs"));
		static int insertStage = 1;
		ImGui::InputInt("Insert Stage (1~3)", &insertStage);


		CCamera* pMainCam = CRenderManager::GetInstance()->GetMainCamera();
		if (E_ProjectionType::Perspective == pMainCam->GetProjectionType()) {
			pMainCam->SetProjectionType(E_ProjectionType::Orthographic);
		}
		Vector2 vMousePos = MousePosition;
		Vector3 vWorldPos = pMainCam->GetScreenToWorld2DPosition(vMousePos);
		vWorldPos.z = 0.f;

		ImGui::Text("Ctrl : Create/Clone Spawn points");
		ImGui::Text("Alt : Delete Spawn point");
		bool bCreate = false;
		bool bDelete = false;

		if (!m_CateList.empty()) {
			ParamGUI::Render_ComboBox("Category", &m_iCurCategoryItemIdx, m_CateList);
			string categoryName = m_CateList[m_iCurCategoryItemIdx];

			// 오브젝트 드래그 기능 추가해서 옮기도록 합세.
			if ("objects" == categoryName) {
				ParamGUI::Render_ComboBox("Objects", &m_iCurObjItemIdx, m_CateObjList);
				if (!m_CateObjList.empty()) {
					m_pCreateFunc = std::bind(&SpawningGUI_bu::_CreateObject, this);
				}
			}
			else if ("actors" == categoryName) {
				ParamGUI::Render_ComboBox("Actors", &m_iCurActorsItemIdx, m_CateActorsList);
				if (!m_CateActorsList.empty()) {
					string actorName = "";
					actorName = m_CateActorsList[m_iCurActorsItemIdx];
					if ("player" == actorName) {
						m_pCreateFunc = std::bind(&SpawningGUI_bu::_CreatePlayer, this);
					}
					else if ("enemy" == actorName) {
						ParamGUI::Render_ComboBox("Weapon", &m_iCurEnemyWeaponTypeIdx, m_enemyWeaponTypeList);
						m_pCreateFunc = std::bind(&SpawningGUI_bu::_CreateEnemy, this);
					}
				}

			}
			else if ("pickups" == categoryName) {
				ParamGUI::Render_ComboBox("Pickups", &m_iCurPickupsItemIdx, m_CatePickupsList);
				if (!m_CatePickupsList.empty()) {
					m_pCreateFunc = std::bind(&SpawningGUI_bu::_CreatePickupsItem, this);
				}
				string itemName = m_CatePickupsList[m_iCurPickupsItemIdx];
				if ("Hp" == itemName) {
					ParamGUI::Render_ComboBox("Level", &m_iCurItemHpTypeIdx, m_itemHpTypeList);
				}
				else if ("Armor" == itemName) {
					ParamGUI::Render_ComboBox("Level", &m_iCurItemArmorTypeIdx, m_itemArmorTypeList);
				}
				
			}
			else if ("doorAndSwitch" == categoryName) {
				ParamGUI::Render_ComboBox("door And Switch", &m_iCurDoorAndSwitchItemIdx, m_CateDoorAndSwitchList);
				if (!m_CateDoorAndSwitchList.empty()) {
					m_pCreateFunc = std::bind(&SpawningGUI_bu::_CreateDoorAndSwitch, this);
				}
			}
		}
		
		if (InputKeyPress(E_Key::LBUTTON)) {
			if (m_pTargetObj)
				m_pTargetObj = nullptr;
			m_pTargetObj = _GetClickedObj(vWorldPos);
		}

		if (InputKeyHold(E_Key::LBUTTON)) {
			if (m_pTargetObj)
				m_pTargetObj->Transform()->SetLocalPosition(vWorldPos);
		}

		// Create
		if (InputKeyHold(E_Key::LCtrl) && InputKeyPress(E_Key::LBUTTON)) {
			bCreate = true;
		}

		// Delete
		else if (InputKeyHold(E_Key::LAlt) && InputKeyPress(E_Key::LBUTTON)) {
			bDelete = true;
		}

		if (bCreate) {
			Vector2 vWorldPos2D = vWorldPos.XY();
			if (m_pCreateFunc) {
				CGameObject* pNewObj = m_pCreateFunc();
				if (pNewObj) {
					switch (insertStage) {
					case 1:
						if(m_pInsertStage1)
							CObject::AddChildGameObjectEvn(m_pInsertStage1, pNewObj);
						break;
					case 2:
						if(m_pInsertStage2)
							CObject::AddChildGameObjectEvn(m_pInsertStage2, pNewObj);
						break;
					case 3:
						if(m_pInsertStage3)
							CObject::AddChildGameObjectEvn(m_pInsertStage3, pNewObj);
						break;
					default:
						break;
					}
				}
			}
		}

		if (bDelete) {
			// 클릭했는지 영역을 알아내야 함.
			CGameObject* pDelObj = _GetClickedObj(vWorldPos);
			if (pDelObj) {
				CObject::DestroyGameObjectEvn(pDelObj);
				m_pTargetObj = nullptr;
			}
		}

		ImGui::End();
	}
}

void SpawningGUI_bu::_Clear()
{
}

CGameObject* SpawningGUI_bu::_CreatePlayer()
{
	if (nullptr == m_pPlayerPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Player_bu.pref");
		m_pPlayerPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pPlayerPrefab)
		return nullptr;

	UINT iLayer = (UINT)E_Layer::Object;
	m_pTargetObj = CObject::InstantiateEvn(m_pPlayerPrefab, Vector3::Zero, iLayer);
	m_pPlayerPrefab->SetName(_T("Player"));
	
	return m_pTargetObj;
}

CGameObject* SpawningGUI_bu::_CreateEnemy()
{
	if (nullptr == m_pEnemyPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Enemy_bu.pref");
		m_pEnemyPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pEnemyPrefab)
		return nullptr;

	UINT iLayer = (UINT)E_Layer::Object;
	m_pTargetObj = CObject::InstantiateEvn(m_pEnemyPrefab, Vector3::Zero, iLayer);
	CWeapon_bu* pCom = m_pTargetObj->GetComponent<CWeapon_bu>();
	E_WeaponType_bu eType = (E_WeaponType_bu)m_iCurEnemyWeaponTypeIdx;
	pCom->SetUseableWeapon(eType, true);
	pCom->ChangeWeapon(eType);

	return m_pTargetObj;
}

CGameObject* SpawningGUI_bu::_CreateDoorAndSwitch()
{
	if (nullptr == m_pDoorPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Door_bu.pref");
		m_pDoorPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pDoorPrefab) {
		assert(nullptr);
		return nullptr;
	}
		

	if (nullptr == m_pSwitch1Prefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Elevator1_bu.pref");
		m_pSwitch1Prefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pSwitch1Prefab) {
		assert(nullptr);
		return nullptr;
	}

	if (nullptr == m_pSwitch2Prefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Elevator2_bu.pref");
		m_pSwitch2Prefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pSwitch2Prefab) {
		assert(nullptr);
		return nullptr;
	}

	if (nullptr == m_pExitDoorPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("ExitDoor_bu.pref");
		m_pExitDoorPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pExitDoorPrefab) {
		assert(nullptr);
		return nullptr;
	}

	UINT iLayer = (UINT)E_Layer::Object;
	string itemName = m_CateDoorAndSwitchList[m_iCurDoorAndSwitchItemIdx];

	if ("door" == itemName) {
		m_pTargetObj = CObject::InstantiateEvn(m_pDoorPrefab, Vector3::Zero, iLayer);
	}
	else if ("elevator1" == itemName) {
		m_pTargetObj = CObject::InstantiateEvn(m_pSwitch1Prefab, Vector3::Zero, iLayer);
	}
	else if ("elevator2" == itemName) {
		m_pTargetObj = CObject::InstantiateEvn(m_pSwitch2Prefab, Vector3::Zero, iLayer);
	}
	else if ("exitDoor" == itemName) {
		m_pTargetObj = CObject::InstantiateEvn(m_pExitDoorPrefab, Vector3::Zero, iLayer);
	}
	return m_pTargetObj;
}

#include <Script\CItemArmor_bu.h>
#include <Script\CItemHp_bu.h>
#include <Script\CItemBullet_bu.h>
#include <Script\CItemWeapon_bu.h>
#include <Script\CItem_bu.h>
CGameObject* SpawningGUI_bu::_CreatePickupsItem()
{
	UINT iLayer = (UINT)E_Layer::Object;

	string itemName = m_CatePickupsList[m_iCurPickupsItemIdx];
	bool bIsCreateBulletPickupItem = false;
	bool bIsCreateWeaponPickupItem = false;

	if ("Shotgun Bullet" == itemName ||
		"Machinegun Bullet" == itemName ||
		"Flame Thrower Bullet" == itemName ||
		"Grenade Launcher Bullet" == itemName ||
		"Laser Gun Bullet" == itemName)
		bIsCreateBulletPickupItem = true;
	else if ("Hp" == itemName) {
		if (nullptr == m_pHpPickupPrefab) {
			tstring path = STR_FILE_PATH_Prefab;
			tstring totalPath = path + _T("ItemPick_Hp_bu.pref");
			m_pHpPickupPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
		}
		if (nullptr == m_pHpPickupPrefab)
			return nullptr;
		m_pTargetObj = CObject::InstantiateEvn(m_pHpPickupPrefab, Vector3::Zero, iLayer);
		auto* pItemHp = m_pTargetObj->GetComponent<CItemHp_bu>();
		pItemHp->SetItemType((CItemHp_bu::E_ItemHpType_bu)m_iCurItemHpTypeIdx);
	}
	else if ("Armor" == itemName) {
		if (nullptr == m_pArmorPickupPrefab) {
			tstring path = STR_FILE_PATH_Prefab;
			tstring totalPath = path + _T("ItemPick_Armor_bu.pref");
			m_pArmorPickupPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
		}
		if (nullptr == m_pArmorPickupPrefab)
			return nullptr;

		m_pTargetObj = CObject::InstantiateEvn(m_pArmorPickupPrefab, Vector3::Zero, iLayer);
		auto* pItemArmor = m_pTargetObj->GetComponent<CItemArmor_bu>();
		pItemArmor->SetItemType((CItemArmor_bu::E_ItemArmorType_bu)m_iCurItemArmorTypeIdx);
	}

	else if ("Shotgun" == itemName ||
		"Machinegun" == itemName ||
		"Flame Thrower" == itemName ||
		"Grenade Launcher" ||
		"Laser Gun" == itemName) {
		bIsCreateWeaponPickupItem = true;
	}

	if (bIsCreateBulletPickupItem) {
		if (nullptr == m_pBulletPickupPrefab) {
			tstring path = STR_FILE_PATH_Prefab;
			tstring totalPath = path + _T("ItemPick_Bullet_bu.pref");
			m_pBulletPickupPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
		}
		if (nullptr == m_pBulletPickupPrefab)
			return nullptr;

		m_pTargetObj = CObject::InstantiateEvn(m_pBulletPickupPrefab, Vector3::Zero, iLayer);
		CItemBullet_bu* pBullet = m_pTargetObj->GetComponent<CItemBullet_bu>();
		
		if ("Shotgun Bullet" == itemName)
			pBullet->SetItemType(E_WeaponType_bu::Shotgun);
		else if ("Machinegun Bullet" == itemName)
			pBullet->SetItemType(E_WeaponType_bu::MachineGun);
		else if ("Flame Thrower Bullet" == itemName)
			pBullet->SetItemType(E_WeaponType_bu::FlameThrower);
		else if ("Grenade Launcher Bullet" == itemName)
			pBullet->SetItemType(E_WeaponType_bu::GrenadeLauncher);
		else if ("Laser Gun Bullet" == itemName)
			pBullet->SetItemType(E_WeaponType_bu::LaserGun);
		else
			assert(nullptr);
	}


	if (bIsCreateWeaponPickupItem) {
		if (nullptr == m_pWeaponPickupPrefab) {
			tstring path = STR_FILE_PATH_Prefab;
			tstring totalPath = path + _T("ItemPick_Weapon_bu.pref");
			m_pWeaponPickupPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
		}
		if (nullptr == m_pWeaponPickupPrefab)
			return nullptr;

		m_pTargetObj = CObject::InstantiateEvn(m_pWeaponPickupPrefab, Vector3::Zero, iLayer);
		CItemWeapon_bu* pWeapon = m_pTargetObj->GetComponent<CItemWeapon_bu>();
		if ("Shotgun" == itemName)
			pWeapon->SetItemType(E_WeaponType_bu::Shotgun);
		else if ("Machinegun" == itemName)
			pWeapon->SetItemType(E_WeaponType_bu::MachineGun);
		else if ("Flame Thrower" == itemName)
			pWeapon->SetItemType(E_WeaponType_bu::FlameThrower);
		else if ("Grenade Launcher" == itemName)
			pWeapon->SetItemType(E_WeaponType_bu::GrenadeLauncher);
		else if ("Laser Gun" == itemName)
			pWeapon->SetItemType(E_WeaponType_bu::LaserGun);
		else
			assert(nullptr);
	}

	return m_pTargetObj;
}

CGameObject* SpawningGUI_bu::_CreateObject()
{
	UINT iLayer = (UINT)E_Layer::Object;
	if (nullptr == m_pSpotLightPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("SpotLight_bu.pref");
		m_pSpotLightPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
		if (nullptr == m_pSpotLightPrefab) {
			assert(nullptr);
			return nullptr;
		}
	}

	if (nullptr == m_pPortalPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Portal_bu.pref");
		m_pPortalPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
		if (nullptr == m_pPortalPrefab) {
			assert(nullptr);
			return nullptr;
		}
	}

	string objName = m_CateObjList[m_iCurObjItemIdx];

	if ("spot light" == objName)
		m_pTargetObj = CObject::InstantiateEvn(m_pSpotLightPrefab, Vector3::Zero, iLayer);
	else if ("portal" == objName)
		m_pTargetObj = CObject::InstantiateEvn(m_pPortalPrefab, Vector3::Zero, iLayer);
	return m_pTargetObj;
}

CGameObject* SpawningGUI_bu::_GetClickedObj(const Vector3& _vWorldPos)
{
	bool isCollision = false;
	int idx = 0;

	CGameObject* pHitObj = nullptr;
	vector<CGameObject*> gameObjs;
	UINT iLayer = (UINT)E_Layer::Object;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(gameObjs, iLayer);

	queue<CGameObject*> que;
	for (size_t i = 0; i < gameObjs.size(); ++i)
		que.push(gameObjs[i]);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop();

		if (pObj->Collider2D()) {
			CCollider2D* pCol = pObj->Collider2D();
			isCollision = CCollisionManager::GetInstance()->IsCollision(pCol, _vWorldPos);

			if (isCollision) {
				pHitObj = pObj;
				break;
			}
		}
		vector<CGameObject*>& chidls = pObj->GetChildsObject();
		for (size_t j = 0; j < chidls.size(); ++j)
			que.push(chidls[j]);
	}
	if (isCollision)
		return pHitObj;

	return nullptr;
}
