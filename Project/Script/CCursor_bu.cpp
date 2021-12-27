#include "pch.h"
#include "CCursor_bu.h"

CCursor_bu::CCursor_bu() :
	CCursorScript(SCRIPT_TYPE::CURSOR_BU)
{
}

CCursor_bu::~CCursor_bu()
{
}

void CCursor_bu::Start()
{
#ifdef _BUTCHER_GAME

	CTexture* pTex = nullptr;

	tstring strPath{};

	TCursorInfo info;
	
	strPath = _T("texture\\Butcher\\Mouse\\Cursor.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Cursor_crosshair"), strPath).Get();
	assert(pTex);
	info.pCursorTex = pTex;
	info.vScale = Vector3(40.f, 40.f, 1.f);
	AddCursor(info);

	strPath = _T("texture\\Butcher\\Mouse\\Chainsaw.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Chainsaw_crosshair"), strPath).Get();
	assert(pTex);
	info.pCursorTex = pTex;
	info.vScale = Vector3(40.f, 40.f, 1.f);
	AddCursor(info);


	strPath = _T("texture\\Butcher\\Mouse\\Shotgun.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Shotgun_crosshair"), strPath).Get();
	assert(pTex);
	info.pCursorTex = pTex;
	info.vScale = Vector3(40.f, 40.f, 1.f);
	AddCursor(info);

	strPath = _T("texture\\Butcher\\Mouse\\Machinegun.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Machinegun_crosshair"), strPath).Get();
	assert(pTex);
	info.pCursorTex = pTex;
	info.vScale = Vector3(40.f, 40.f, 1.f);
	AddCursor(info);

	strPath = _T("texture\\Butcher\\Mouse\\Flamethrower.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Flamethrower_crosshair"), strPath).Get();
	assert(pTex);
	info.pCursorTex = pTex;
	info.vScale = Vector3(40.f, 40.f, 1.f);
	AddCursor(info);

	strPath = _T("texture\\Butcher\\Mouse\\GrenadeLauncher.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("GrenadeLauncher_crosshair"), strPath).Get();
	assert(pTex);
	info.pCursorTex = pTex;
	info.vScale = Vector3(40.f, 40.f, 1.f);
	AddCursor(info);

	strPath = _T("texture\\Butcher\\Mouse\\Railgun.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Railgun_crosshair"), strPath).Get();
	assert(pTex);
	info.pCursorTex = pTex;
	info.vScale = Vector3(40.f, 40.f, 1.f);
	AddCursor(info);

	strPath = _T("texture\\Butcher\\Mouse\\PoliceCarRockets.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("PoliceCarRockets_crosshair"), strPath).Get();
	assert(pTex);
	info.pCursorTex = pTex;
	info.vScale = Vector3(40.f, 40.f, 1.f);
	AddCursor(info);

#endif
}