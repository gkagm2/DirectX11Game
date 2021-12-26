#pragma once
#include "CComponent.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CKeyManager.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CTimeManager.h"
#include "CMouseManager.h"
#include "CMouseEvent.h"

#include "CMeshRenderer.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CLight2D.h"
#include "CGameObject.h"

enum class E_ScriptParam {
	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	TEX,
	PREFAB,
	GAMEOBJ, // game object
	END,
};

struct TScriptParam {
	tstring strName;
	E_ScriptParam eParam;
	void* pData;
};

class CScript : public CComponent, public ICollision2DInterface, public ICollision3DInterface
{
private:
	UINT m_iScriptType;
	vector<TScriptParam> m_vecParam; // 엔진 UI에 보여주기 위하여 파람을 생성함.

public:
	virtual void FinalUpdate() final {}

	virtual void OnCollisionEnter2D(CCollider2D* _pOther) {}
	virtual void OnCollisionStay2D(CCollider2D* _pOther) {}
	virtual void OnCollisionExit2D(CCollider2D* _pOther) {}

	virtual void OnCollisionEnter(CCollider3D* _pOther) {}
	virtual void OnCollisionStay(CCollider3D* _pOther) {}
	virtual void OnCollisionExit(CCollider3D* _pOther) {}

	UINT GetScriptType() { return m_iScriptType; }

protected:
	void AddParam(const TScriptParam& _tParam) { m_vecParam.push_back(_tParam); }
public:
	const vector<TScriptParam>& GetParams_Cst() { return m_vecParam; }
	vector<TScriptParam>& GetParams() { return m_vecParam; }

public:
	virtual bool SaveToScene(FILE* _pFile) override { return true; }
	virtual bool LoadFromScene(FILE* _pFile) override { return true; }

public:
	virtual CScript* Clone() = 0;
	CScript(UINT _iType);
	virtual ~CScript() override;
};