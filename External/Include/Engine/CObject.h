#pragma once

class CGameObject;
class CObject
{
private:
	static UINT g_iNextID;

private:
	UINT m_iID;

public:
	virtual void UpdateData() {}

	static void CreateGameObjectEvn(CGameObject* _pTargetObj, E_Layer _eLayer = E_Layer::End);
	static void DestroyGameObjectEvn(CGameObject* _pTargetObj);
	static void AddChildGameObjectEvn(CGameObject* _pParent, CGameObject* _pChild);
	static void UnlinkParentGameObjectEvn(CGameObject* _pChildObj);

public:
	UINT GetID() { return m_iID; }

public:
	// TODO (Jang) : Clone�� ���� �����Լ��� �����
	virtual CObject* Clone() { return new CObject(*this); }
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();
};