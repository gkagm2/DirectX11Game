#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"


CTransform::CTransform() :
	CComponent(E_ComponentType::Transform),
	m_vLocalPosition{},
	m_vLocalScale{1.f, 1.f, 1.f},
	m_vLocalRotation{},
	m_matWorld{}
{
}

CTransform::~CTransform()
{
}

void CTransform::LateUpdate()
{
	UpdateData();
}

void CTransform::UpdateData()
{
	// b0 레지스터에 상수버퍼 바인딩
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::transform);
	pCB->SetData(&m_vLocalPosition);
	pCB->UpdateData(E_ShaderStage::Vertex);
}
