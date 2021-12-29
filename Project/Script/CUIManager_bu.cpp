#include "pch.h"
#include "CUIManager_bu.h"

CUIManager_bu* CUIManager_bu::pInst = nullptr;

CUIManager_bu::CUIManager_bu() :
	CScript((UINT)SCRIPT_TYPE::UIMANAGER_BU)
{
}

CUIManager_bu::~CUIManager_bu() {

}

CUIManager_bu* CUIManager_bu::GetInstance() {
	if (!pInst)
		pInst = this;
	return pInst;
}