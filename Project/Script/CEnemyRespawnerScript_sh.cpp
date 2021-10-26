#include "pch.h"
#include "CEnemyRespawnerScript_sh.h"
#include "CEnemyScript_sh.h"
#include <Engine\CCore.h>
#include "CGameManagerScript_sh.h"
#include <Engine\CCollider2D.h>

CEnemyRespawnerScript_sh::CEnemyRespawnerScript_sh() :
	CScript((UINT)SCRIPT_TYPE::ENEMYRESPAWNERSCRIPT_SH),
	m_vRespawnPos{},
	m_fRespawnCoolTime(0.f),
	m_fRespawnMaxCoolTime(2.f)
{
}

CEnemyRespawnerScript_sh::~CEnemyRespawnerScript_sh()
{
}

void CEnemyRespawnerScript_sh::Awake()
{
}

void CEnemyRespawnerScript_sh::Start()
{
	CreateEnemy();
}

void CEnemyRespawnerScript_sh::Update()
{
	CGameManagerScript_sh* pGameMgr = (CGameManagerScript_sh*)FIND_GameObject(STR_OBJ_NAME_GameManager)->GetComponent<CGameManagerScript_sh>();
	if (E_GameState_sh::GameOver == pGameMgr->GetGameState())
		return;

	m_fRespawnCoolTime += DT;
	if (m_fRespawnCoolTime > m_fRespawnMaxCoolTime) {
		CreateEnemy();
		m_fRespawnCoolTime = 0.f;
	}
}

void CEnemyRespawnerScript_sh::CreateEnemy()
{
	SharedPtr<CTexture> pTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	CGameObject* pEnemy = new CGameObject();
	pEnemy->AddComponent<CTransform>();
	pEnemy->AddComponent<CMeshRenderer>();
	pEnemy->AddComponent<CEnemyScript_sh>();
	pEnemy->AddComponent<CCollider2D>();

	pEnemy->MeshRenderer()->SetMaterial(pMtrl);
	pEnemy->MeshRenderer()->SetMesh(pMesh);

	float fMiddleX = 0.f;
	float fMiddleY = 9.f;

	m_vRespawnPos.x = fMiddleX - (rand() % (int)9.f);
	m_vRespawnPos.y = fMiddleY;

	pEnemy->Transform()->SetLocalPosition(m_vRespawnPos);

	pEnemy->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));

	CObject::CreateGameObjectEvn(pEnemy, (UINT)E_Layer::Enemy);
}