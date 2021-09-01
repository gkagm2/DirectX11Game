#include "pch.h"
#include "CUI.h"
#include "CCore.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CUIManager.h"
#include "CRectTransform.h"
#include "CComponent.h"
#include "CRenderManager.h"
//Test
#include "CCore.h"

CUI::CUI(E_ComponentType _eComponentType) :
    CBehaviour(_eComponentType),
    m_bIsOn(false),
    m_bIsDown(false)
{
    m_pColMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectLineMesh);
    m_pColMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_Collider2DMtrl);
    assert(m_pColMesh.Get());
    assert(m_pColMtrl.Get());
}

CUI::CUI(const CUI& _origin) :
    CBehaviour(_origin.m_eType),
    m_bIsOn(false),
    m_bIsDown(false),
    m_pColMesh(_origin.m_pColMesh),
    m_pColMtrl(_origin.m_pColMtrl)
{
}

CUI::~CUI()
{
}

void CUI::Update()
{
}

void CUI::LateUpdate()
{
   /* if (m_pParentUI)
        m_vFinalPosition =GetPosition() + m_pParentUI->GetFinalPosition();
    else
        m_vFinalPosition = GetPosition();

    for (UINT i = 0; i < m_vecChildUI.size(); ++i)
        m_vecChildUI[i]->LateUpdate();*/
}

void CUI::FinalUpdate()
{
    // UI 
}

void CUI::Render()
{
    if (nullptr == m_pColMesh || nullptr == m_pColMtrl)
        return;

    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.9f, 0.9f, 0.2f, 1.f)); // 노란색
    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.5f, 0.1f, 0.9f, 1.f)); // 보라색
    //Vector4(0.2f, 0.9f, 0.2, 1.f)); //green
    //Vector4(0.9f, 0.2f, 0.2f, 1.f)); // red


    //m_pColMesh->UpdateData();   // 메쉬 세팅
    //m_pColMtrl->UpdateData();	 // 메터리얼 세팅
    //m_pColMesh->Render();		 // 렌더링 

    //m_pColMtrl->Clear();		 // 메터리얼 레지스터 Clear  



    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.2f, 0.9f, 0.2, 1.f)); // greeen
    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.9f, 0.2f, 0.2f, 1.f)); // red
}

void CUI::OnPointerDown()
{
}
void CUI::OnPointerUp()
{
}
void CUI::OnPointerClick()
{
}

bool CUI::IsPointerOn(const Vector2& _vMousePosition)
{
    Vector2 mousePos = _vMousePosition;
    Vector2 minScreenPos = GetMin();
    Vector2 maxScreenPos = GetMax();
    if (_vMousePosition.x >= minScreenPos.x && _vMousePosition.x <= maxScreenPos.x &&
        _vMousePosition.y >= minScreenPos.y && _vMousePosition.y <= maxScreenPos.y) {
        return true;
    }
    return false;
}

// UI의 World 좌표의 min, max 구하기
Vector2 CUI::GetMin()
{
    // pixel 사이즈라고 생각하고 해야되는듯?
    CCamera* pUICam = CRenderManager::GetInstance()->GetUICamera();
    assert(pUICam && _T("UI Camera 없음"));
    // 중심점으로부터 width, height을 구해서 위치를 구함
    assert(RectTransform() && _T("RectTransform 없음"));
    Vector3 vPos = RectTransform()->GetPosition();
    float fHalfWidth = fabsf(vPos.x - RectTransform()->GetWidth()) * 0.5f;
    float fHalfHeight = fabsf(vPos.y - RectTransform()->GetHeight()) * 0.5;

    Vector3 vMin;
    vMin.x = vPos.x - fHalfWidth;
    vMin.y = vPos.y + fHalfHeight;
    vMin.z = vPos.z;

    Vector2 vScreenMin = pUICam->GetWorldToScreen2DPosition(vMin);

    return vScreenMin;
}
Vector2 CUI::GetMax()
{
    // pixel 사이즈라고 생각하고 해야되는듯?
    CCamera* pUICam = CRenderManager::GetInstance()->GetUICamera();
    assert(pUICam && _T("UI Camera 없음"));
    // 중심점으로부터 width, height을 구해서 위치를 구함
    assert(RectTransform() && _T("RectTransform 없음"));
    Vector3 vPos = RectTransform()->GetPosition();
    float fHalfWidth = fabsf(vPos.x - RectTransform()->GetWidth()) * 0.5f;
    float fHalfHeight = fabsf(vPos.y - RectTransform()->GetHeight()) * 0.5;

    Vector3 vMax;
    vMax.x = vPos.x + fHalfWidth;
    vMax.y = vPos.y - fHalfHeight;
    vMax.z = vPos.z;

    Vector2 vScreenMax = pUICam->GetWorldToScreen2DPosition(vMax);

    return vScreenMax;
}