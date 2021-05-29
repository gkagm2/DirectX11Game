#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"


CTransform::CTransform() :
	CComponent(E_ComponentType::transform)
{
}

CTransform::~CTransform()
{
}

void CTransform::Update()
{
	// b0 �������Ϳ� ������� ���ε�
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::transform);
	pCB->SetData(&m_vLocalPosition);
	pCB->UpdateData(E_ShaderStage::vertex);
}

void CTransform::LateUpdate()
{
	Update();
}