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

	std::function<void()> m_pItemInitFunc;	// ������ �ʱ�ȭ
	std::function<void()> m_pItemFunc;		// �÷��̾ �Ծ��� ��� ����

protected:
	CGameObject* m_pItemAnimObj;
	CAnimator2D* m_pItemAnim;

	CCharacter_bu* m_pTargetCharacter;

public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual void OnCollisionEnter2D(CCollider2D* _pOther);

public:
	void Interactive(CCharacter_bu* _pTargetCharacter);

protected:
	void InitItem();

protected:
	template<typename T>
	void _SetItemCallBack(void(T::* _InitFunc)(), void(T::* _InteractiveFunc)(), T* _pObj) {
		m_pItemInitFunc = std::bind(_InitFunc, _pObj);
		m_pItemFunc = std::bind(_InteractiveFunc, _pObj);
	}
public:
	CLONE(CItem_bu);
	CItem_bu();
	CItem_bu(UINT _iScriptType);
	CItem_bu(const CItem_bu& _origin);
	virtual ~CItem_bu() override;
};