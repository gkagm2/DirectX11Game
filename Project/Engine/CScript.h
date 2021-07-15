#pragma once
#include "CComponent.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CKeyManager.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CTimeManager.h"

#include "CMeshRenderer.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CLight2D.h"
#include "CGameObject.h"

class CScript : public CComponent
{
private:
	UINT m_iScriptType;
public:
	virtual void FinalUpdate() final {}

	UINT GetScriptType() { return m_iScriptType; }

public:
	virtual CScript* Clone() = 0;
	CScript(UINT _iType);
	virtual ~CScript() override;
};