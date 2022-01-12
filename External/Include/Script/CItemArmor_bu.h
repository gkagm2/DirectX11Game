#pragma once
#include "CItem_bu.h"
class CItemArmor_bu : public CItem_bu
{
public:
	enum class E_ItemArmorType_bu {
		Armor1,
		Armor2,
		Armor3,
		Armor4,
		End
	};
private:
	float m_fArmor;
	E_ItemArmorType_bu m_eArmorType;

	static tstring strDescription;
private:
	virtual void InitItem() override;
	virtual void Interact(CCharacter_bu* _pTargetCharacter) override;

public:
	virtual void Start() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;


public:
	CLONE(CItemArmor_bu);
	CItemArmor_bu();
	CItemArmor_bu(const CItemArmor_bu& _origin);
	virtual ~CItemArmor_bu() override;
};