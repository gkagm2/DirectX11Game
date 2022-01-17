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

	m_CateObjList.push_back("destruct object");

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
	m_CateDoorAndSwitchList.push_back("switch");

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
	if (ImGui::Begin(STR_GUI_Spawning_bu, &m_bGUIOpen)) {
		if (!m_bGUIOpen) {
			_Clear();
			ImGui::End();
			return;
		}

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
					string name = m_CateObjList[m_iCurCategoryItemIdx];
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
			if(m_pCreateFunc)
				m_pCreateFunc();
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

void SpawningGUI_bu::_CreatePlayer()
{
	if (nullptr == m_pPlayerPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Player_bu.pref");
		m_pPlayerPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pPlayerPrefab)
		return;

	UINT iLayer = (UINT)E_Layer::Object;
	CObject::InstantiateEvn(m_pPlayerPrefab, Vector3::Zero, iLayer);
	m_pPlayerPrefab->SetName(_T("Player"));
}

void SpawningGUI_bu::_CreateEnemy()
{
	if (nullptr == m_pEnemyPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Enemy_bu.pref");
		m_pEnemyPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pEnemyPrefab)
		return;

	UINT iLayer = (UINT)E_Layer::Object;
	m_pTargetObj = CObject::InstantiateEvn(m_pEnemyPrefab, Vector3::Zero, iLayer);
	auto* pCom = m_pTargetObj->GetComponent< CWeapon_bu>();
	E_WeaponType_bu eType = (E_WeaponType_bu)m_iCurEnemyWeaponTypeIdx;
	pCom->SetUseableWeapon(eType, true);
	pCom->ChangeWeapon(eType);
}

void SpawningGUI_bu::_CreateDoorAndSwitch()
{
	if (nullptr == m_pDoorPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Door_bu.pref");
		m_pDoorPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pDoorPrefab)
		return;

	if (nullptr == m_pSwitchPrefab) {
		tstring path = STR_FILE_PATH_Prefab;
		tstring totalPath = path + _T("Swtich_bu.pref");
		m_pSwitchPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(totalPath);
	}
	if (nullptr == m_pSwitchPrefab)
		return;

	UINT iLayer = (UINT)E_Layer::Object;
	string itemName = m_CateDoorAndSwitchList[m_iCurDoorAndSwitchItemIdx];

	if ("door" == itemName) {
		m_pTargetObj = CObject::InstantiateEvn(m_pDoorPrefab, Vector3::Zero, iLayer);
	}
	else if ("switch" == itemName) {
		m_pTargetObj = CObject::InstantiateEvn(m_pSwitchPrefab, Vector3::Zero, iLayer);
	}
}

#include <Script\CItemArmor_bu.h>
#include <Script\CItemHp_bu.h>
#include <Script\CItemBullet_bu.h>
#include <Script\CItemWeapon_bu.h>
#include <Script\CItem_bu.h>
void SpawningGUI_bu::_CreatePickupsItem()
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
			return;
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
			return;
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
			return;

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
			return;

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

	
}

CGameObject* SpawningGUI_bu::_GetClickedObj(const Vector3& _vWorldPos)
{
	bool isCollision = false;
	int idx = 0;

	vector<CGameObject*> gameObjs;
	UINT iLayer = (UINT)E_Layer::Object;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(gameObjs, iLayer);
	for (int i = 0; i < gameObjs.size(); ++i) {
		CGameObject* pObj = gameObjs[i];
		if (pObj->Collider2D()) {
			CCollider2D* pCol = pObj->Collider2D();

			isCollision = CCollisionManager::GetInstance()->IsCollision(pCol, _vWorldPos);

			if (isCollision) {
				idx = i;
				break;
			}
		}
	}
	if (isCollision)
		return gameObjs[idx];

	return nullptr;
}
