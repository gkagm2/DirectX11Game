#pragma once
#include "CComponent.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CKeyManager.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CTime"

#include "CMeshRenderer.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CGameObject.h"

class CScript : public CComponent
{
public:
	virtual void FinalUpdate() final {}

public:
	CScript();
	virtual ~CScript() override;
};