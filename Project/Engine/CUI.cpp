#include "pch.h"
#include "CUI.h"
#include "CCore.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CUIManager.h"
#include "CRectTransform.h"
#include "CComponent.h"

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
    Vector3 minPos = GetMin();
    Vector3 maxPos = GetMax();
    if (_vMousePosition.x >= GetMin().x && _vMousePosition.x <= GetMax().x &&
        _vMousePosition.y >= GetMin().y && _vMousePosition.y <= GetMax().y) {
        return true;
    }
    return false;
}

// TODO : Min, Max 구하기 구현
Vector3 CUI::GetMin()
{
    // TODO (Jang) : 해야됨
    //CCamera* pUICam = GetGameObject()->GetComponentInParent

    return Vector3();
}
Vector3 CUI::GetMax()
{
    //switch (m_ePivotState) {
    //case E_UIPivot::leftTop:
    //{
    //    Vector3 maxVec = GetFinalPosition();
    //    CTexture* pTexture = GetTexture();
    //    if (nullptr != pTexture) {
    //        //maxVec.x += pTexture->GetWidth();
    //        //maxVec.y += pTexture->GetHeight();
    //        maxVec.x += GetScale().x;
    //        maxVec.y += GetScale().y;
    //    }
    //    else
    //        maxVec += GetScale();
    //    return maxVec;
    //}
    //case E_UIPivot::left:
    //    break;
    //case E_UIPivot::leftBottom:
    //    break;
    //case E_UIPivot::top:
    //    break;
    //case E_UIPivot::center:
    //{
    //    Vector3 maxVec = GetFinalPosition();
    //    CTexture* pTexture = GetTexture();
    //    if (nullptr != pTexture) {
    //        /*maxVec.x += pTexture->GetWidth() * 0.5f;
    //        maxVec.y += pTexture->GetHeight() * 0.5f;*/
    //        maxVec.x += GetScale().x * 0.5f;
    //        maxVec.y += GetScale().y * 0.5f;
    //    }
    //    else
    //        maxVec += GetScale() * 0.5f;
    //    return maxVec;
    //}
    //    break;
    //case E_UIPivot::rightTop:
    //    break;
    //case E_UIPivot::right:
    //    break;
    //case E_UIPivot::rightBottom:
    //    break;
    //}
    return Vector3();
}