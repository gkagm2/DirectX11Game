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
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CKeyManager.h"
//Test
#include "CCore.h"

CUI::CUI(E_ComponentType _eComponentType) :
    CBehaviour(_eComponentType),
    m_bActiveClickEvent(true),
    m_bIsOn(false),
    m_bIsDown(false),
    m_vOffsetPosition{Vector3(0.f,0.f,0.f)},
    m_vOffsetScale(Vector3(1.f,1.f,1.f)),
    m_PointerDownCallBack{ nullptr },
    m_PointerUpCallBack{ nullptr },
    m_ClickCallBack{nullptr}
{
    m_pColMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectLineMesh);
    m_pColMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_Collider2DNoneColliedMtrl);
    assert(m_pColMesh.Get());
    assert(m_pColMtrl.Get());
}

CUI::CUI(const CUI& _origin) :
    CBehaviour(_origin.m_eType),
    m_bActiveClickEvent(_origin.m_bActiveClickEvent),
    m_bIsOn(false),
    m_bIsDown(false),
    m_pColMesh(_origin.m_pColMesh),
    m_pColMtrl(_origin.m_pColMtrl),
    m_PointerDownCallBack{ nullptr },
    m_PointerUpCallBack{ nullptr },
    m_ClickCallBack{nullptr}
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
    // 충돌체 크기 * 충돌체 이동(물체 크기로 미리 나눔) *  (물체 크기 * 물체 회전 * 물체 이동) == 충돌체의 World Matrix
    //                                                  (           World Matrix       )
    // Collider Material 
 
    //Vector2 vOffsetPosition = m_vOffsetPosition / RectTransform()->GetLocalScale_RectTR().XY();

    //Matrix matTrans = XMMatrixTranslation(vOffsetPosition.x, vOffsetPosition.y, 0.f);

    //RectTransform()->GetWorldMatrix_NoParentScale();
    //Vector3 vScale = RectTransform()->GetLocalScale_RectTR();
    //Matrix matScale = XMMatrixScaling(vScale.x, vScale.y, 1.f);

    //m_matColWorld = matScale * matTrans * Transform()->GetWorldMatrix_NoParentScale();
    //m_matColWorld._43 = 0.f; //2D이므로 0 (3d면?)
}

void CUI::Render()
{
    if (nullptr == m_pColMesh || nullptr == m_pColMtrl)
        return;

    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.9f, 0.9f, 0.2f, 1.f)); // 노란색
    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.5f, 0.1f, 0.9f, 1.f)); // 보라색
    //Vector4(0.2f, 0.9f, 0.2, 1.f)); //green
    //Vector4(0.9f, 0.2f, 0.2f, 1.f)); // red

    // 충돌체의 정보 업데이트
    //UpdateData();

    //m_pColMesh->UpdateData();   // 메쉬 세팅
    //m_pColMtrl->UpdateData();	 // 메터리얼 세팅
    //m_pColMesh->Render();		 // 렌더링 

    //m_pColMtrl->Clear();		 // 메터리얼 레지스터 Clear  



    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.2f, 0.9f, 0.2, 1.f)); // greeen
    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.9f, 0.2f, 0.2f, 1.f)); // red
}

void CUI::UpdateData()
{
    static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::RectTransform);
    g_rectTransform.matWorld = m_matColWorld;

    pCB->SetData(&g_rectTransform);
    pCB->UpdateData(E_ShaderStage::Vertex);
}

void CUI::OnPointerDown()
{
    if (m_PointerDownCallBack)
        m_PointerDownCallBack();
}
void CUI::OnPointerUp()
{
    if (m_PointerUpCallBack)
        m_PointerUpCallBack();
}
void CUI::OnPointerClick()
{
    if (m_ClickCallBack)
        m_ClickCallBack();
}

bool CUI::SaveToScene(FILE* _pFile)
{
    CBehaviour::SaveToScene(_pFile);
    FWrite(m_bActiveClickEvent, _pFile);
    return true;
}

bool CUI::LoadFromScene(FILE* _pFile)
{
    CBehaviour::LoadFromScene(_pFile);
    FRead(m_bActiveClickEvent, _pFile);
    return true;
}
