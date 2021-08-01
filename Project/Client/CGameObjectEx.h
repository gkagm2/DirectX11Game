#pragma once
#include <Engine\CGameObject.h>
class CGameObjectEx : public CGameObject
{
public:
	virtual void FinalUpdate() override;

public:
	CGameObjectEx();
	virtual ~CGameObjectEx() override;
};