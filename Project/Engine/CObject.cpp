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
    m_iID(g_iNextID++),
    m_strName(_origin.m_strName)
{
}

CObject::~CObject()
{
}

void CObject::CreateGameObjectEvn(CGameObject* _pTargetObj, UINT _iLayer)
{
    TEvent even = {};
    even.eType = E_EventType::Create_GameObject;
    even.lparam = (DWORD_PTR)_pTargetObj;
    even.wparam = (DWORD_PTR)_iLayer;

    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::CreateGameObjectEvn(CGameObject* _pTargetObj, const Vector3& _vWorldPos, UINT _iLayer)
{
    _pTargetObj->Transform()->SetLocalPosition(_vWorldPos);
    CreateGameObjectEvn(_pTargetObj, _iLayer);
}

void CObject::DestroyGameObjectEvn(CGameObject* _pTargetObj)
{
    TEvent even = {};
    even.eType = E_EventType::Destroy_GameObject;
    even.lparam = (DWORD_PTR)_pTargetObj;
    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::DestroyObjectEvn(CObject* _pTargetObj)
{
    TEvent even = {};
    even.eType = E_EventType::Destroy_Object;
    even.lparam = (DWORD_PTR)_pTargetObj;
    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::DestroyScriptEvn(CGameObject* _pTargetObj, CScript* _deleteScript)
{
    TEvent even = {};
    even.eType = E_EventType::Destroy_Script;
    even.lparam = (DWORD_PTR)_pTargetObj;
    even.wparam = (DWORD_PTR)_deleteScript;
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

CGameObject* CObject::InstantiateEvn(SharedPtr<CPrefab> _prefab, const Vector3& _vWorldPos, UINT _iLayer)
{
    CGameObject* pObj = _prefab->Instantiate();
    CreateGameObjectEvn(pObj, _vWorldPos, _iLayer);
    return pObj;
}

void CObject::ChangeLayerEvn(CGameObject* _pTargetObj, UINT _iLayer)
{
    TEvent even = {};
    even.eType = E_EventType::Change_GameObject_And_Childs_Layer;
    even.lparam = (DWORD_PTR)_pTargetObj;
    even.wparam = (DWORD_PTR)_iLayer;
    CEventManager::GetInstance()->AddEvent(even);
}

void CObject::ChangeStateEvn()
{
    TEvent even = {};
    even.eType = E_EventType::Change_State;
    CEventManager::GetInstance()->AddEvent(even);
}

bool CObject::SaveToScene(FILE* _pFile)
{
    SaveStringToFile(m_strName, _pFile);
    return true;
}
bool CObject::LoadFromScene(FILE* _pFile)
{
    LoadStringFromFile(m_strName, _pFile);
    return true;
}