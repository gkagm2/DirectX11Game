#include "pch.h"
#include "CObject.h"

#include "CEventManager.h"

#include "CScript.h"
#include "CGameObject.h"

CObject::CObject()
{
}

CObject::~CObject()
{
}

void CObject::CreateGameObjectEvn(CGameObject* _pTargetObj, E_Layer _eLayer)
{
    _pTargetObj->Transform()->SetLocalPosition(_pTargetObj->Transform()->GetLocalPosition());

    TEvent even = {};
    even.eType = E_EventType::Create_Object;
    even.lparam = (DWORD_PTR)_pTargetObj;
    even.wparam = (int)_eLayer;

    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::DestroyGameObjectEvn(CGameObject* _pTargetObj)
{
    TEvent even = {};
    even.eType = E_EventType::Destroy_Object;
    even.lparam = (DWORD_PTR)_pTargetObj;
    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::AddChildGameObjectEvn(CGameObject* _pParent, CGameObject* _pChild)
{
    TEvent even = {};
    even.eType = E_EventType::Add_Child;
    even.lparam = (DWORD_PTR)_pParent;
    even.wparam = (DWORD_PTR)_pChild;
    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::UnlinkParentGameObjectEvn(CGameObject* _pChildObj)
{
    TEvent even = {};
    even.eType = E_EventType::Unlink_Parent;
    even.lparam = (DWORD_PTR)_pChildObj;
    CEventManager::GetInstance()->AddEvent(even);
}