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

void CObject::CreateGameObject(CGameObject* _pTargetObj, E_Layer _eLayer)
{
    _pTargetObj->Transform()->SetLocalPosition(_pTargetObj->Transform()->GetLocalPosition());

    TEvent even = {};
    even.eType = E_EventType::Create_Object;
    even.lparam = (DWORD_PTR)_pTargetObj;
    if (E_Layer::End == _eLayer)
        _eLayer = _pTargetObj->GetLayer();
    even.wparam = (int)_eLayer;

    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::DestroyGameObject(CGameObject* _pTargetObj)
{
    TEvent even = {};
    even.eType = E_EventType::Destroy_Object;
    even.lparam = (DWORD_PTR)_pTargetObj;
    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::AddChildGameObject(CGameObject* _pParent, CGameObject* _pChild)
{
    TEvent even = {};
    even.eType = E_EventType::Add_Child;
    even.lparam = (DWORD_PTR)_pParent;
    even.wparam = (DWORD_PTR)_pChild;
    CEventManager::GetInstance()->AddEvent(even);
}