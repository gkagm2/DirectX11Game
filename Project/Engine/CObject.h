#pragma once

class CGameObject;
class CObject
{
public:
	virtual void UpdateData() {}

	static void CreateGameObject(CGameObject* _pTargetObj, E_Layer _eLayer = E_Layer::End);
	static void DestroyGameObject(CGameObject* _pTargetObj);
	static void AddChildGameObject(CGameObject* _pParent, CGameObject* _pChild);

public:
	CObject();
	virtual ~CObject();
};