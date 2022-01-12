#pragma once
#include <Engine\CScript.h>


enum class E_ItemType_bu {
	Hp,
	Armor,

	Shotgun,
	MachineGun,
	GrenadeLauncher,
	FlameThrower,
	LaserGun,

	End
};

class CCharacter_bu;
class CItem_bu : public CScript
{
private:
	// �����ֱ�
	bool m_bIsLifeTimeOn;
	float m_fMaxLifeTime;
	float m_fLifeTime;

protected:
	CGameObject* m_pItemAnimObj;
	CAnimator2D* m_pItemAnim;

	CCharacter_bu* m_pTargetCharacter;

	tstring m_strItemName;

public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual void OnCollisionEnter2D(CCollider2D* _pOther);

protected:
	// ������ �ʱ�ȭ
	virtual void InitItem() {}
public:
	// �÷��̾ �Ծ��� ��� ����
	virtual void Interact(CCharacter_bu* _pTargetCharacter){}

public:
	CLONE(CItem_bu);
	CItem_bu();
	CItem_bu(UINT _iScriptType);
	CItem_bu(const CItem_bu& _origin);
	virtual ~CItem_bu() override;
};