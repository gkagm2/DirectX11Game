#include "pch.h"
#include "CFrustum.h"

/*
   Cube
				4-------5
			    |       |
	0-------1	|       |
	|       |	|       |
	|       |	7-------6
	|       |
	3-------2
	
*/

const Vector3 CFrustum::g_arrProjPos[8] = {};

CFrustum::CFrustum() :
	m_pOwner{ nullptr },
	m_arrFace{}
{
}

CFrustum::~CFrustum()
{
}

void CFrustum::UpdateData()
{
}

void CFrustum::FinalUpdate()
{
}

bool CFrustum::CheckPoint(const Vector3& _vPos)
{
	return true;
}

bool CFrustum::CheckSphere(const Vector3& _vCenter, float _fRadius)
{
	return true;
}