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
    // �浹ü ũ�� * �浹ü �̵�(��ü ũ��� �̸� ����) *  (��ü ũ�� * ��ü ȸ�� * ��ü �̵�) == �浹ü�� World Matrix
    //                                                  (           World Matrix       )
    // Collider Material 
 
    //Vector2 vOffsetPosition = m_vOffsetPosition / RectTransform()->GetLocalScale_RectTR().XY();

    //Matrix matTrans = XMMatrixTranslation(vOffsetPosition.x, vOffsetPosition.y, 0.f);

    //RectTransform()->GetWorldMatrix_NoParentScale();
    //Vector3 vScale = RectTransform()->GetLocalScale_RectTR();
    //Matrix matScale = XMMatrixScaling(vScale.x, vScale.y, 1.f);

    //m_matColWorld = matScale * matTrans * Transform()->GetWorldMatrix_NoParentScale();
    //m_matColWorld._43 = 0.f; //2D�̹Ƿ� 0 (3d��?)
}

void CUI::Render()
{
    if (nullptr == m_pColMesh || nullptr == m_pColMtrl)
        return;

    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.9f, 0.9f, 0.2f, 1.f)); // �����
    //m_pColMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.5f, 0.1f, 0.9f, 1.f)); // �����
    //Vector4(0.2f, 0.9f, 0.2, 1.f)); //green
    //Vector4(0.9f, 0.2f, 0.2f, 1.f)); // red

    // �浹ü�� ���� ������Ʈ
    //UpdateData();

    //m_pColMesh->UpdateData();   // �޽� ����
    //m_pColMtrl->UpdateData();	 // ���͸��� ����
    //m_pColMesh->Render();		 // ������ 

    //m_pColMtrl->Clear();		 // ���͸��� �������� Clear  



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
