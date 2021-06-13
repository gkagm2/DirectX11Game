#pragma once

class CGameObject;
class CObject
{
public:
	virtual void UpdateData() {}

	static void CreateGameObjectEvn(CGameObject* _pTargetObj, E_Layer _eLayer = E_Layer::End);
	static void DestroyGameObjectEvn(CGameObject* _pTargetObj);
	static void AddChildGameObjectEvn(CGameObject* _pParent, CGameObject* _pChild);
	static void UnlinkParentGameObjectEvn(CGameObject* _pChildObj);

public:
	// TODO (Jang) : Clone은 순수 가상함수로 만들기
	virtual CObject* Clone() { return new CObject(*this); }
	CObject();
	virtual ~CObject();
};