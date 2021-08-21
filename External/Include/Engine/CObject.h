#pragma once
class CGameObject;
class CPrefab;
class CScript;
template<typename T> class SharedPtr;

class CObject //: public ISaveLoadSceneInterface
{
private:
	static UINT g_iNextID;

private:
	UINT m_iID;
	tstring m_strName;

public:
	virtual void UpdateData() {}

	static void CreateGameObjectEvn(CGameObject* _pTargetObj, UINT _iLayer = MAX_SIZE_LAYER);
	static void CreateGameObjectEvn(CGameObject* _pTargetObj, const Vector3& _vWorldPos, UINT _iLayer = MAX_SIZE_LAYER);
	static void DestroyGameObjectEvn(CGameObject* _pTargetObj);
	static void DestroyObjectEvn(CObject* _pTargetObj);
	static void DestroyScriptEvn(CGameObject* _pTargetObj, CScript* _deleteScript);

	static void AddChildGameObjectEvn(CGameObject* _pParent, CGameObject* _pChild);
	static void UnlinkParentGameObjectEvn(CGameObject* _pChildObj);
	static void InstantiateEvn(SharedPtr<CPrefab> _prefab, const Vector3& _vWorldPos, UINT _iLayer = 0);
	static void ChangeLayerEvn(CGameObject* _pTargetObj, UINT _iLayer);

	static void ChangeStateEvn(); // ���°� �ٲ��

public:
	UINT GetID() { return m_iID; }
	const tstring& GetName() { return m_strName; }
	void SetName(const tstring& _strName) { m_strName = _strName; }

public:
	virtual bool SaveToScene(FILE* _pFile);
	virtual bool LoadFromScene(FILE* _pFile);

public:
	virtual CObject* Clone() = 0;
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();
};