#pragma once
#include "CComponent.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CKeyManager.h"
#include "CTime"

class CScript : public CComponent
{
public:
	virtual void FinalUpdate() final {}

public:
	CScript();
	virtual ~CScript() override;
};