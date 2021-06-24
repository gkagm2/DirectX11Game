#include "pch.h"
#include "CEnemyRespawnerScript_sh.h"
#include "CEnemyScript_sh.h"
#include "CCore.h"
#include "CGameManagerScript_sh.h"
#include "CCollider2DRect.h"

CEnemyRespawnerScript_sh::CEnemyRespawnerScript_sh() :
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
	CGameManagerScript_sh* pGameMgr = (CGameManagerScript_sh*)FIND_GameObject(STR_OBJ_NAME_GameManager)->GetComponent<CScript>();
	if (E_GameState_sh::GameOver == pGameMgr->GetGameState())
		return;

	m_fRespawnCoolTime += DeltaTime;
	if (m_fRespawnCoolTime > m_fRespawnMaxCoolTime) {
		CreateEnemy();
		m_fRespawnCoolTime = 0.f;
	}
}

void CEnemyRespawnerScript_sh::CreateEnemy()
{
	SharedPtr<CTexture> pTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Box);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	CGameObject* pEnemy = new CGameObject();
	pEnemy->AddComponent<CTransform>();
	pEnemy->AddComponent<CMeshRenderer>();
	pEnemy->AddComponent<CEnemyScript_sh>();
	pEnemy->AddComponent<CCollider2DRect>();

	pEnemy->MeshRenderer()->SetMaterial(pMtrl);
	pEnemy->MeshRenderer()->SetMesh(pMesh);

	Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();
	float fMiddleX = vResolution.x * 0.5f;
	float fMiddleY = vResolution.y * 0.5f;
	m_vRespawnPos.x = fMiddleX - (rand() % (int)vResolution.x);
	m_vRespawnPos.y = fMiddleY;

	pEnemy->Transform()->SetLocalPosition(m_vRespawnPos);

	pEnemy->Transform()->SetLocalScale(Vector3(pTexture->GetDimension().x, pTexture->GetDimension().y, 1.f));

	CObject::CreateGameObjectEvn(pEnemy, E_Layer::Enemy);
}