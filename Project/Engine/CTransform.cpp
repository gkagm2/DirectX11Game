#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

// Test
#include "CKeyManager.h"
#include "CTimeManager.h"


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
	Vector3 vPos = GetLocalPosition();
	Vector3 vScale = GetLocalScale();

	float fSpeed = 50.f;
	if (InputKeyHold(E_Key::A)) {
		for (int i = 0; i < 8; ++i) {
			SetLocalPosition(Vector3(vPos.x - DeltaTime * fSpeed, vPos.y, vPos.z));
		}
	}
	if (InputKeyHold(E_Key::S)) {
		for (int i = 0; i < 8; ++i) {
			SetLocalPosition(Vector3(vPos.x, vPos.y - DeltaTime * fSpeed, vPos.z));
		}
	}
	if (InputKeyHold(E_Key::D)) {
		for (int i = 0; i < 8; ++i) {
			SetLocalPosition(Vector3(vPos.x + DeltaTime * fSpeed, vPos.y, vPos.z));
		}
	}
	if (InputKeyHold(E_Key::W)) {
		for (int i = 0; i < 8; ++i) {
			SetLocalPosition(Vector3(vPos.x, vPos.y + DeltaTime * fSpeed, vPos.z));
		}
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		for (int i = 0; i < 8; ++i) {
			SetLocalScale(Vector3(vScale.x + DeltaTime * fSpeed, vScale.y + DeltaTime * fSpeed, vScale.z));
		}
	}
	if (InputKeyHold(E_Key::LEFT)) {
		for (int i = 0; i < 8; ++i) {
			SetLocalScale(Vector3(vScale.x - DeltaTime * fSpeed, vScale.y - DeltaTime * fSpeed, vScale.z));
		}
	}

	// 크기 행렬
	Matrix matScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	// 회전 행렬
	Matrix matRotX = XMMatrixRotationX(m_vLocalRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vLocalRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vLocalRotation.z);

	Matrix matRot = matRotX * matRotY * matRotZ;

	// 이동 행렬
	Matrix matTrans = XMMatrixTranslation(m_vLocalPosition.x, m_vLocalPosition.y, m_vLocalPosition.z);

	m_matWorld = matScale * matRot * matTrans;
}

void CTransform::UpdateData()
{
	// b0 레지스터에 상수버퍼 바인딩
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::transform);

	g_transform.matWorld = m_matWorld;

	pCB->SetData(&g_transform);
	pCB->UpdateData(E_ShaderStage::Vertex);
}
