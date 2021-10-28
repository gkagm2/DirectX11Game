#include "pch.h"
#include <Engine\CCore.h>

#include "CGameManagerScript_sh.h"

#include "CPlayerScript_sh.h"
#include "CBulletScript_sh.h"
#include <Engine\CCore.h>

CPlayerScript_sh::CPlayerScript_sh() :
	CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT_SH),
	m_fHp(10.f),
	m_fBulletDamage(2.f),
	m_fMoveSpeed(15.f)
{
}

CPlayerScript_sh::~CPlayerScript_sh()
{
}

void CPlayerScript_sh::Start()
{
	m_fHp = 10.f;
	m_fBulletDamage = 2.f;
	m_fMoveSpeed = 15.f;



	GetGameObject()->SetName(_T("Player"));

	m_pSharedMtrl = MeshRenderer()->GetSharedMaterial();
	m_pCloneMtrl = MeshRenderer()->GetCloneMaterial();
	/*m_pMissilePrefab = CResourceManager::GetInstance()->LoadRes<CPrefab>(_T("Bullet"), _T("prefab\\bullet.pref"));*/
}

void CPlayerScript_sh::Update()
{
	/*CGameManagerScript_sh* pGameMgr = (CGameManagerScript_sh*)FIND_GameObject(STR_OBJ_NAME_GameManager)->GetComponent<CScript>();
	if (E_GameState_sh::GameOver == pGameMgr->GetGameState())
		return;*/

	Move();

	/*if (InputKeyPress(E_Key::LBUTTON)) {
		Vector2 vMousePos = MousePosition;
		Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();
		vResolution *= 0.5f;
		vMousePos -= vResolution;
		Shoot(Vector2(vMousePos.x, vMousePos.y));
	}*/
	if (InputKeyPress(E_Key::SPACE)) {
		// up vec
		Shoot(Vector2(0.f, 1.f));
	}

	// TODO (Jang) : 삭제하셈. (메터리얼 색상 변경 테스트)
	if (InputKeyPress(E_Key::Z)) {
		static int a = 0;

		if (a == 1) {
			a = 0;
			MeshRenderer()->SetMaterial(m_pSharedMtrl);
		}
		else {
			a = 1;
			m_pCloneMtrl->SetData(E_ShaderParam::Int_0, &a);
			MeshRenderer()->SetMaterial(m_pCloneMtrl);
		}
	}
}

void CPlayerScript_sh::Shoot(const Vector2& _vTargetPos)
{
	Vector3 vPlayerPos = Transform()->GetLocalPosition();
	Vector3 vPlayerScale = Transform()->GetLocalScale();
	Vector3 vMuzzlePos = vPlayerPos;
	if (m_pMissilePrefab.Get()) {
		CGameObject* pObj = InstantiateEvn(m_pMissilePrefab, vPlayerPos, (UINT)E_Layer::Bullet);
		pObj->GetComponent<CBulletScript_sh>()->SetOwnerObject(GetGameObject());
		pObj->GetComponent<CBulletScript_sh>()->SetDirection(Vector3(0.f, 1.f, 0.f));

		//CGameObject* pBulletObj = new CGameObject;
		//pBulletObj->AddComponent<CTransform>();
		//pBulletObj->AddComponent<CMeshRenderer>();
		//pBulletObj->AddComponent<CBulletScript_sh>();

		//Vector3 vPlayerPos = Transform()->GetLocalPosition();
		//Vector3 vPlayerScale = Transform()->GetLocalScale();
		//Vector3 vMuzzlePos = vPlayerPos;
		////vMuzzlePos.y += vPlayerScale.y * 0.5f;

		//pBulletObj->Transform()->SetLocalPosition(vMuzzlePos);
		//pBulletObj->Transform()->SetLocalScale(vPlayerScale * 0.3f);

		//pBulletObj->MeshRenderer()->SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash));
		//pBulletObj->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl));

		//CBulletScript_sh* pBullet = pBulletObj->GetComponent<CBulletScript_sh>();

		///*Vector3 vDirPos = _vTargetPos - vPlayerPos;
		//vDirPos.Normalize();*/
		//Vector3 vDirPos = Vector3(0.f, 1.f, 0.f);

		//pBullet->SetDirection(vDirPos);
		//pBullet->SetBulletSpeed(800.f);

		//CreateGameObjectEvn(pBulletObj, 4);
	}
  	
}

void CPlayerScript_sh::Move()
{
	Vector3 vDir{0,0,0};

	Vector3 vPosition = Transform()->GetLocalPosition();

	if (InputKeyHold(E_Key::A)) {
		vDir.x = -1.f;
	}
		
	if (InputKeyHold(E_Key::D)) {
		vDir.x = 1.f;
	}

	if (InputKeyHold(E_Key::W)) {
		vDir.y = 1.f;
	}

	if (InputKeyHold(E_Key::S)) {
		vDir.y = -1.f;
	}

	
	float z = Transform()->GetLocalRotation().z;
	if (InputKeyHold(E_Key::LCtrl))
		Transform()->SetLocalRotation(Vector3(0.f, 0.f,  z + 15 * DT));

	vDir.Normalize();
	vPosition = vPosition + vDir * m_fMoveSpeed * DT;
	float fMiddleX = 0.f;
	float fRange = (float)(CCore::GetInstance()->GetWindowResolution().x * 0.5f - Transform()->GetLocalScale().x);
	/*if (vPosition.x > fRange + fMiddleX)
		vPosition.x = fRange + fMiddleX;
	if (vPosition.x < -fRange + fMiddleX)
		vPosition.x = -fRange + fMiddleX;*/
	Transform()->SetLocalPosition(vPosition);
}

bool CPlayerScript_sh::SaveToScene(FILE* _pFile)
{
	FWrite(m_fHp, _pFile);
	FWrite(m_fBulletDamage, _pFile);
	FWrite(m_fMoveSpeed, _pFile);
	SaveResourceToFile(m_pMissilePrefab, _pFile);
	return true;
}

bool CPlayerScript_sh::LoadFromScene(FILE* _pFile)
{
	FRead(m_fHp, _pFile);
	FRead(m_fBulletDamage, _pFile);
	FRead(m_fMoveSpeed, _pFile);
	LoadResourceFromFile(m_pMissilePrefab, _pFile);
	return true;
}