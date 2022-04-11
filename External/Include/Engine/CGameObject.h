#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CComponent;
class CTransform;
class CMeshRenderer;
class CCamera;
class CBoundingBox;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CAnimator3D;
class CLight2D;
class CLight3D;
class CLight;
class CTileMap;
class CSkybox;
class CParticleSystem;
class CRigidbody2D;
class CRectTransform;
class CSpriteRenderer;
class CCanvasRenderer;
class CTextUI;
class CImageUI;
class CButtonUI;
class CUI;
class CBehaviour;
class CAudioSource;
class CDecal;
class CTerrain;

class CScript;

class CGameObject : public CObject, ILifeCycleInterface
{
private:
	CComponent* m_arrComponent[(UINT)E_ComponentType::End];
	CGameObject* m_pParentObj;
	vector<CGameObject*> m_vecChildObj;
	vector<CScript*> m_vecScript;
	CComponent* m_pRenderComponenet;

	UINT m_iLayer;
	UINT m_iTag;

	bool m_bDead;
	bool m_bActive;
	int m_iLocalAddress;
	bool m_bDynamicShadow;		// 동적 그림자 생성
	bool m_bUseFrustumCulling;	// 카메라 절두체 컬링 영향

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;

public:
	// Components
	GET_COMPONENT_COMPOTABLE

public:
	void RegisterAsPrefab(const tstring& _strName = _T(""));

public:
	UINT GetLayer() { return m_iLayer; }
	void SetTag(UINT _iTag, bool _bChangeChilds = false);
	UINT GetTag() { return m_iTag; }
	bool IsDead() { return m_bDead; }

	void SetActive(bool _bActive, bool _bWithChilds = false);
	bool IsActive() { return m_bActive; }

	CGameObject* GetParentObject() { return m_pParentObj; }
	vector<CGameObject*>& GetChildsObject() { return m_vecChildObj; }
	CGameObject* FindGameObjectInChilds(const tstring& _strObjName);
	bool IsExistGameObjectInChilds(CGameObject* _pObj, bool _bIncludeMine = false);
	bool IsExistGameObjectInParent(CGameObject* _pObj, bool _bIncludeMine = false);
	bool IsExistGameObjectInTree(CGameObject* _pObj);
	CGameObject* FindGameObjectSameLine(const tstring& _strObjName);

	template<typename TYPE>
	CGameObject* FindGameObjectInChilds(bool _bIncludeMine = false); // 컴포넌트를 이용하여 찾기

	CGameObject* FindGameObjectInParent(const tstring& _strObjName);

	template<typename TYPE>
	CGameObject* FindGameObjectInParent(bool _bIncludeMine = false); // 컴포넌트를 이용하여 찾기

	template<typename TYPE>
	TYPE* FindComponentInChilds(bool _bIncludeMine = false);
	template<typename TYPE>
	TYPE* FindComponentInParent(bool _bIncludeMine = false);
	
	CGameObject* GetRootObject();

public:
	bool IsDynamicShadow() { return m_bDynamicShadow; }
	void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
	bool IsFrustumCulling() { return m_bUseFrustumCulling; }
	void SetFrustumCulling(bool _bSet) { m_bUseFrustumCulling = _bSet; }

private:
	void _SetLayer(UINT _iLayer) { m_iLayer = _iLayer; }
	void _SetDead();
	void _AddChildGameObject(CGameObject* _pChildObj, bool _IsSaveLoad = false);
	void _RegisterLayer();
	void _SetLocalAddress(int _iAddress) { m_iLocalAddress = _iAddress; }

	vector<CGameObject*>& _GetChildsObjectRef() { return m_vecChildObj; }

	void _UnlinkParentGameObject(bool _IsSaveLoad = false);
	bool _IsAncestorGameObject(CGameObject* _pObj);

	//오직 하나만 렌더링 가능한 컴포넌트인지 체크
	bool _IsOnlyOnePossibleRenderComponent(E_ComponentType _eComponentType);
	bool _IsExistComponent(E_ComponentType _eComponentType) { return m_arrComponent[(UINT)_eComponentType] ? true : false; }

public:
	CComponent* CreateComponent(E_ComponentType _eType);
private:
	void _DestroyComponent(E_ComponentType _eType);
	void _DestroyScript(CScript* pScript);
	
public:
	template<typename TYPE>
	TYPE* AddComponent();

	CComponent* AddComponent(CComponent* _pComponent);

	template<typename TYPE>
	TYPE* GetComponent();
	CComponent* GetComponent(E_ComponentType _eType);
	template<typename TYPE>
	TYPE* GetComponentScript(UINT _iScriptType);
	template<typename TYPE>
	TYPE* GetComponentScript();

	const vector<CScript*>& GetScripts() { return m_vecScript; }// 추가되어있는 스크립트 컴포넌트들을 가져온다.

	tstring GetLocalAddressTotal();
	int GetLocalAddress() { return m_iLocalAddress; }
	CGameObject* FindGameObjectFromLocalAddress(const tstring& _strLocalAddress);
	void ChangeLocalAddressToIntTotal(vector<int>& _vecOut, const tstring& _strLocalAddress);

private:
	vector<CScript*>& _GetScripts() { return m_vecScript; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	bool LoadFromScene(FILE* _pFile, int _iDepth); // Depth가 0이 최상위 오브젝트

public:
	CLONE(CGameObject);
	CGameObject();
	CGameObject(const CGameObject& _origin);
	virtual ~CGameObject() override;

	friend class CEventManager;
	friend class CLayer;
	friend class CCollider2D;
	friend class CCollider3D;
	friend class CUI;
	friend class CScene;
};

template<typename TYPE>
inline TYPE* CGameObject::AddComponent()
{
	TYPE* pComponent = new TYPE();
	CComponent* pComp = dynamic_cast<CComponent*>(pComponent);
	if (nullptr == pComp) {
		SAFE_DELETE(pComponent);
		assert(nullptr && _T("컴포넌트 타입이 아님."));
		return nullptr;
	}

	E_ComponentType eType = pComp->GetComponentType();

	if (E_ComponentType::Script == pComp->GetComponentType()) {
		m_vecScript.push_back((CScript*)pComp);
		pComp->m_pGameObj = this;
		return pComponent;
	}

	// 이미 존재하면
	if (_IsExistComponent(eType)) {
		SAFE_DELETE(pComponent);
		pComponent = dynamic_cast<TYPE*>(m_arrComponent[(UINT)eType]);
		return pComponent;
	}

	// 오직 하나만 렌더링할 수 있는 종류의 컴포넌트인지 체크
	bool bIsOnlyOneRenderComponent = _IsOnlyOnePossibleRenderComponent(eType);
	if (bIsOnlyOneRenderComponent) {
		/*if (_IsExistComponent((E_ComponentType)pComponent->GetComponentType())) {
			assert(nullptr && _T("이미 렌더링하는 다른 컴포넌트가 존재함"));
			SAFE_DELETE(pComponent);
			return nullptr;
		}*/
		m_pRenderComponenet = pComponent;
	}

	m_arrComponent[(UINT)eType] = pComp;
	pComp->m_pGameObj = this;

 	return pComponent;
}

template<typename TYPE>
inline CGameObject* CGameObject::FindGameObjectInChilds(bool _bIncludeMine)
{
	CGameObject* pFindObject = nullptr;
	list<CGameObject*> que;
	if (_bIncludeMine)
		que.push_back(this);
	else {
		const vector<CGameObject*>& vecChilds = GetChildsObject();
		for (UINT i = 0; i < vecChilds.size(); ++i)
			que.push_back(vecChilds[i]);
	}

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();
		TYPE* pComponent = GetComponent<TYPE>();
		if (pComponent) {
			pFindObject = pObj;
			break;
		}
		const vector<CGameObject*>& childs = pObj->GetChildsObject();
		for (UINT i = 0; i < childs.size(); ++i)
			que.push_back(childs[i]);
	}

	return pFindObject;
}

template<typename TYPE>
inline CGameObject* CGameObject::FindGameObjectInParent(bool _bIncludeMine)
{
	CGameObject* pParent = nullptr;
	if (_bIncludeMine)
		pParent = this;
	else
		pParent = GetParentObject();
	while (nullptr != pParent) {
		TYPE* pCom =  pParent->GetComponent<TYPE>();
		if (pCom) {
			break;
		}
		pParent = pParent->GetParentObject();
	}
	return pParent;
}

template<typename TYPE>
inline TYPE* CGameObject::FindComponentInChilds(bool _bIncludeMine)
{
	TYPE* pFindComType = nullptr;
	list<CGameObject*> que;
	if (_bIncludeMine) {
		que.push_back(this);
	}
	else {
		const vector<CGameObject*>& vecChilds = GetChildsObject();
		for (UINT i = 0; i < vecChilds.size(); ++i)
			que.push_back(vecChilds[i]);
	}

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();
		TYPE* pComponent = pObj->GetComponent<TYPE>();
		if (pComponent) {
			pFindComType = pComponent;
			break;
		}
		const vector<CGameObject*>& childs = pObj->GetChildsObject();
		for (UINT i = 0; i < childs.size(); ++i)
			que.push_back(childs[i]);
	}

	return pFindComType;
}

template<typename TYPE>
inline TYPE* CGameObject::FindComponentInParent(bool _bIncludeMine)
{
	CGameObject* pParent = nullptr;
	if (_bIncludeMine)
		pParent = this;
	else
		pParent = GetParentObject();

	while (nullptr != pParent) {
		TYPE* pCom = pParent->GetComponent<TYPE>();
		if (pCom)
			return pCom;
		pParent = pParent->GetParentObject();
	}
	return nullptr;
}

template<typename TYPE>
inline TYPE* CGameObject::GetComponent()
{

	 for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		TYPE* pComponent = dynamic_cast<TYPE*>(m_arrComponent[i]);
		if (nullptr != pComponent) {
			return pComponent;
		}
	 }

	//// Script Type일 경우    
	TYPE* pComponent = GetComponentScript<TYPE>();
	return pComponent;
}

template<typename TYPE>
inline TYPE* CGameObject::GetComponentScript()
 {
	 for (UINT i = 0; i < (UINT)m_vecScript.size(); ++i) {
		 if (typeid(TYPE) == typeid(*m_vecScript[i]))
			 return (TYPE*)m_vecScript[i];
		 TYPE* pComponent = dynamic_cast<TYPE*>(m_vecScript[i]);
		 // 부모 클래스에 존재하는지 확인
		 if (nullptr != pComponent) {
			 return pComponent;
		 }
	 }
	 return nullptr;
 }