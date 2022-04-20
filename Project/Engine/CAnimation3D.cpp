#include "pch.h"
#include "CAnimation3D.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CAnimation3D::CAnimation3D()
{
}

CAnimation3D::~CAnimation3D()
{
}

void CAnimation3D::Clear()
{
	static const CConstBuffer* pBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Animation2D_Data);

	pBuffer->Clear();
}

void CAnimation3D::FinalUpdate()
{
}

void CAnimation3D::UpdateData()
{
}

void CAnimation3D::Create(TAnimation3DDesc& _tAnimation2DDesc)
{
}

void CAnimation3D::Create(const vector<TAnimation3DDesc>& _vecAnimation2DDesc)
{
}

bool CAnimation3D::SaveToScene(FILE* _pFile)
{
	return false;
}

bool CAnimation3D::LoadFromScene(FILE* _pFile)
{
	return false;
}
