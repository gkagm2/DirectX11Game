#pragma once
#include "CSingleton.h"
class CGameObject;
class CMaterial;
class CObjectManager : public CSingleton<CObjectManager>
{
	SINGLETON(CObjectManager);
public:
	CMaterial* CreateEmptyMaterial();
	CGameObject* CreateEmptyGameObject(UINT _iLayer = 0);
	CGameObject* CreateCamera2DGameObject(UINT _iLayer = 0);
	CGameObject* CreateParticleSystemGameObject(UINT _iLayer = 0);
	CGameObject* CreateLight2D(UINT _iLayer = 0);
	CGameObject* Create2DRectGameObject(UINT _iLayer = 0);	

	CGameObject* CreateCubeGameObject(UINT _iLayer = 0);
	CGameObject* CreateSphereGameobject(UINT _iLayer = 0);

public:
	CGameObject* CreateTextUI();
	CGameObject* CreateImageUI();
	CGameObject* CreateButtonUI();

private:
	CGameObject* _CreateUIGameObject();
	CGameObject* _CreateCanvas();
	CGameObject* _CreateDefaultUICamera();
	tstring _CreateObjectName(const tstring& _strObjDefaultName, UINT& id);
};