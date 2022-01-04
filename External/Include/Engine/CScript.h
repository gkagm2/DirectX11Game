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
#include "CRenderManager.h"

#include "CMeshRenderer.h"
#include "CTransform.h"
#include "CRectTransform.h"
#include "CRigidbody2D.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

#include "CCamera.h"
#include "CLight2D.h"
#include "CGameObject.h"
#include "CCursor.h"

// UI
#include "CCanvasRenderer.h"
#include "CRectTransform.h"
#include "CTextUI.h"
#include "CButtonUI.h"
#include "CImageUI.h"

enum class E_ScriptParam {
	INT,
	BOOL,
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
	void** ppData; // 포인터를 위한것
	TScriptParam() :
		strName(_T("")), eParam(E_ScriptParam::END), pData(nullptr), ppData(nullptr)
	{}
	TScriptParam(const tstring& _strName, E_ScriptParam _eParam, void* _pData, void** _ppData) :
		strName(_strName), eParam(_eParam), pData(_pData), ppData(_ppData)
	{}
	TScriptParam(const tstring& _strName, E_ScriptParam _eParam, void* _pData) :
		strName(_strName), eParam(_eParam), pData(_pData), ppData(nullptr)
	{}
	TScriptParam(const tstring& _strName, E_ScriptParam _eParam, void** _ppData) :
		strName(_strName), eParam(_eParam), pData(nullptr), ppData(_ppData) 
	{}
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