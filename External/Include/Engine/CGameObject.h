#pragma once
#include "CObject.h"
#include "Interfaces.h"
class CComponent;
class CTransform;
class CMeshRenderer;
class CCamera;
class CScript;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CLight2D;
class CTileMap;
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

class CGameObject : public CObject, ILifeCycleInterface
{
private:
	CComponent* m_arrComponent[(UINT)E_ComponentType::End];
	CGameObject* m_pParentObj;
	vector<CGameObject*> m_vecChildObj;
	vector<CScript*> m_vecScript;

	UINT m_iLayer;
	UINT m_iTag;

	bool m_bDead;
	bool m_bActive;

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
	CGameObject* FindGameObjectSameLine(const tstring& _strObjName);

	template<typename TYPE>
	CGameObject* FindGameObjectInChilds();

	CGameObject* FindGameObjectInParent(const tstring& _strObjName);

	template<typename TYPE>
	CGameObject* FindGameObjectInParent();

	template<typename TYPE>
	TYPE* FindComponentInChilds();
	template<typename TYPE>
	TYPE* FindComponentInParent();
	
	CGameObject* GetRootObject();

private:
	void _SetLayer(UINT _iLayer) { m_iLayer = _iLayer; }
	void _SetDead();
	void _AddChildGameObject(CGameObject* _pChildObj, bool _IsSaveLoad = false);
	void _RegisterLayer();

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

	const vector<CScript*>& GetScripts() { return m_vecScript; }// 추가되어있는 스크립트 컴포넌트들을 가져온다.
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
};

template<typename TYPE>
inline TYPE* CGameObject::AddComponent()
{
	TYPE* pComponent = new TYPE();
	CComponent* pComp = dynamic_cast<CComponent*>(pComponent);
	if (nullptr == pComp) {
		delete pComponent;
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
		delete pComponent;
		pComponent = dynamic_cast<TYPE*>(m_arrComponent[(UINT)eType]);
		return pComponent;
	}

	// 오직 하나만 렌더링할 수 있는 종류의 컴포넌트인지 체크
	bool bIsOnlyOneRenderComponent = _IsOnlyOnePossibleRenderComponent(eType);
	if (bIsOnlyOneRenderComponent) {
		for (UINT i = ONLY_ONE_POSSIBLE_RENDERING_START_IDX; i < ONLY_ONE_POSSIBLE_RENDERING_END_IDX; ++i) {
			if (eType == (E_ComponentType)i)
				continue;
			if (_IsExistComponent((E_ComponentType)i)) {
				assert(nullptr && _T("이미 렌더링하는 다른 컴포넌트가 존재함"));
				if (pComponent)
					delete pComponent;
				return nullptr;
			}
		}
	}


	m_arrComponent[(UINT)eType] = pComp;
	pComp->m_pGameObj = this;

 	return pComponent;
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

	// Script Type일 경우    
	for (UINT i = 0; i < (UINT)m_vecScript.size(); ++i) {
		if (typeid(TYPE) == typeid(*m_vecScript[i]))
			return (TYPE*)m_vecScript[i];
		// 부모 클래스에 존재하는지 확인
		TYPE* pType = dynamic_cast<TYPE*>(m_vecScript[i]);
		if (pType)
			return (TYPE*)m_vecScript[i];
	}

	return nullptr;
}

template<typename TYPE>
inline CGameObject* CGameObject::FindGameObjectInChilds()
{
	CGameObject* pFindObject = nullptr;
	list<CGameObject*> que;
	const vector<CGameObject*>& vecChilds = GetChildsObject();
	for (UINT i = 0; i < vecChilds.size(); ++i)
		que.push_back(vecChilds[i]);

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
inline CGameObject* CGameObject::FindGameObjectInParent()
{
	CGameObject* pParent = GetParentObject();
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
inline TYPE* CGameObject::FindComponentInChilds()
{
	TYPE* pFindComType = nullptr;
	list<CGameObject*> que;
	const vector<CGameObject*>& vecChilds = GetChildsObject();
	for (UINT i = 0; i < vecChilds.size(); ++i)
		que.push_back(vecChilds[i]);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();
		TYPE* pComponent = GetComponent<TYPE>();
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
inline TYPE* CGameObject::FindComponentInParent()
{
	CGameObject* pParent = GetParentObject();

	while (nullptr != pParent) {
		TYPE* pCom = pParent->GetComponent<TYPE>();
		if (pCom)
			return pCom;
		pParent = pParent->GetParentObject();
	}
	return nullptr;
}
