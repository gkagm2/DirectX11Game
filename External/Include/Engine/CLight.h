#pragma once
#include "Ptr.h"
#include "CComponent.h"
class CMesh;
class CMaterial;
class CLight : public CComponent
{


public:
	CLONE(CLight);
	CLight();
	CLight(E_ComponentType _eType);
	virtual ~CLight() override;
};