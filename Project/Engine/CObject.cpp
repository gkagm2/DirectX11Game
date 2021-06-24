#include "pch.h"
#include "CObject.h"

#include "CEventManager.h"

#include "CScript.h"
#include "CGameObject.h"
#include "Ptr.h"
#include "CPrefab.h"

UINT CObject::g_iNextID = 0;
CObject::CObject() :
    m_iID(g_iNextID++)
{
}

CObject::CObject(const CObject& _origin) :
    m_iID(g_iNextID++)
{
}

CObject::~CObject()
{
}

void CObject::CreateGameObjectEvn(CGameObject* _pTargetObj, E_Layer _eLayer)
{
    TEvent even = {};
    even.eType = E_EventType::Create_Object;
    even.lparam = (DWORD_PTR)_pTargetObj;
    even.wparam = (int)_eLayer;

    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::CreateGameObjectEvn(CGameObject* _pTargetObj, const Vector3& _vWorldPos, E_Layer _eLayer)
{
    _pTargetObj->Transform()->SetLocalPosition(_vWorldPos);
    CreateGameObjectEvn(_pTargetObj, _eLayer);
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

void CObject::InstantiateEvn(SharedPtr<CPrefab> _prefab, const Vector3& _vWorldPos, E_Layer _eLayer)
{
    CreateGameObjectEvn(_prefab->Instantiate(), _vWorldPos, _eLayer);
}