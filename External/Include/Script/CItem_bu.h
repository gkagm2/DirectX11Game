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

class CItem_bu : public CScript
{
private:
	E_ItemType_bu m_eItemType;
	
	// 생명주기
	bool m_bIsLifeTimeOn;
	float m_fMaxLifeTime;
	float m_fLifeTime;

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;
	virtual void OnCollisionEnter2D(CCollider2D* _pOther);
	
public:
	E_ItemType_bu GetItemType() { return m_eItemType; }
	void SetItemType(E_ItemType_bu _eType) { m_eItemType = _eType; }

public:
	void GainItem(); // 아이템을 얻을 경우 실행하는 함수
		


public:
	CLONE(CItem_bu);
	CItem_bu();
	virtual ~CItem_bu() override;
};