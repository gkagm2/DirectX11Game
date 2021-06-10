#include "pch.h"
#include "CShootingGameManagerScript.h"

CShootingGameManager::CShootingGameManager() :
	m_eState(E_GameState_Shooting::Lobby)
{
}

CShootingGameManager::~CShootingGameManager()
{
}

void CShootingGameManager::Awake()
{
}

void CShootingGameManager::Start()
{
	SetName(STR_SHOOTING_OBJ_NAME_ShootingGameManager);
}

void CShootingGameManager::Update()
{
}