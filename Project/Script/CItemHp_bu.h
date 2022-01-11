#pragma once
#include "CItem_bu.h"
class CItemHp_bu :public CItem_bu
{
public:
	enum class E_ItemHpType_bu {
		Hp1,
		Hp2,
		Hp3,
		Hp4,
		End
	};

private:
	float m_fHp;
	E_ItemHpType_bu m_eHpType;

	static tstring strDescription;

private:
	void ItemInitHp();
	void ItemHp();

public:
	virtual void Start() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CItemHp_bu);
	CItemHp_bu();
	CItemHp_bu(const CItemHp_bu& _origin);
	virtual ~CItemHp_bu() override;
};