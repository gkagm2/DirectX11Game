#include "pch.h"
#include "CGameManagerScript_sh.h"

CGameManagerScript_sh::CGameManagerScript_sh() :
	m_eState(E_GameState_sh::Lobby)
{
}

CGameManagerScript_sh::~CGameManagerScript_sh()
{
}

void CGameManagerScript_sh::Awake()
{
	GetGameObject()->SetName(STR_OBJ_NAME_GameManager);
}

void CGameManagerScript_sh::Start()
{
}

void CGameManagerScript_sh::Update()
{
}