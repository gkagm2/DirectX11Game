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

class CScript : public CComponent, public ICollision2DInterface, public ICollision3DInterface
{
private:
	UINT m_iScriptType;
public:
	virtual void FinalUpdate() final {}

	virtual void OnCollisionEnter2D(CCollider2D* _pOther) override {}
	virtual void OnCollisionStay2D(CCollider2D* _pOther) override {}
	virtual void OnCollisionExit2D(CCollider2D* _pOther) override {}

	virtual void OnCollisionEnter(CCollider3D* _pOther) override {}
	virtual void OnCollisionStay(CCollider3D* _pOther) override {}
	virtual void OnCollisionExit(CCollider3D* _pOther) override {}

	UINT GetScriptType() { return m_iScriptType; }

public:
	virtual bool SaveToScene(FILE* _pFile) override { return true; }
	virtual bool LoadFromScene(FILE* _pFile) override { return true; }

public:
	virtual CScript* Clone() = 0;
	CScript(UINT _iType);
	virtual ~CScript() override;
};